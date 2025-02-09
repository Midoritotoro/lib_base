#include <base/media/ffmpeg/audio/AbstractAudioFFmpegReader.h>

extern "C" {
	#include <libavfilter/buffersink.h>
	#include <libavfilter/buffersrc.h>
	#include <libavutil/opt.h>
} // extern "C"

#ifdef min
#undef min
#endif // min

#ifdef max
#undef max
#endif // max


namespace base::media::ffmpeg::audio {
	AbstractAudioFFmpegReader::AbstractAudioFFmpegReader(const QByteArray& data):
		AbstractAudioReader(data)
		, _frame(MakeFramePointer()) {
		}

		void AbstractAudioFFmpegReader::dropFramesTill(int64_t samples) {
			const auto isAfter = [&](const EnqueuedFrame& frame) {
				return frame.position > samples;
				};
			const auto from = begin(_framesQueued);
			const auto after = std::find_if(_framesQueued.begin(), _framesQueued.end(), isAfter);
			if (from == after) {
				return;
			}
			const auto till = after - 1;
			const auto erasing = till - from;
			if (erasing > 0) {
				if (_framesQueuedIndex >= 0) {

					_framesQueuedIndex -= erasing;
				}
				_framesQueued.erase(from, till);
				if (_framesQueued.empty()) {
					_framesQueuedIndex = -1;
				}
			}
		}

		int64_t AbstractAudioFFmpegReader::startReadingQueuedFrames(float newSpeed) {
			changeSpeedFilter(newSpeed);
			if (_framesQueued.empty()) {
				_framesQueuedIndex = -1;
				return -1;
			}
			_framesQueuedIndex = 0;
			return _framesQueued.front().position;
		}

		bool AbstractAudioFFmpegReader::initUsingContext(
			AVCodecContext* context,
			float speed) {
			_swrSrcSampleFormat = context->sample_fmt;
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			const AVChannelLayout mono = AV_CHANNEL_LAYOUT_MONO;
			const AVChannelLayout stereo = AV_CHANNEL_LAYOUT_STEREO;
			const auto useMono = !av_channel_layout_compare(
				&context->ch_layout,
				&mono);
			const auto useStereo = !av_channel_layout_compare(
				&context->ch_layout,
				&stereo);
			const auto copyDstChannelLayout = [&] {
				av_channel_layout_copy(&_swrDstChannelLayout, &context->ch_layout);
				};
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			const auto layout = ComputeChannelLayout(
				context->channel_layout,
				context->channels);
			if (!layout) {
				LOG(("Audio Error: Unknown channel layout %1 for %2 channels."
					).arg(context->channel_layout
					).arg(context->channels
					));
				return false;
			}
			const auto useMono = (layout == AV_CH_LAYOUT_MONO);
			const auto useStereo = (layout == AV_CH_LAYOUT_STEREO);
			const auto copyDstChannelLayout = [&] {
				_swrDstChannelLayout = layout;
				};
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			if (useMono) {
				switch (_swrSrcSampleFormat) {
				case AV_SAMPLE_FMT_U8:
				case AV_SAMPLE_FMT_U8P:
					_swrDstSampleFormat = _swrSrcSampleFormat;
					copyDstChannelLayout();
					_outputChannels = 1;
					_outputSampleSize = 1;
					_outputFormat = AL_FORMAT_MONO8;
					break;
				case AV_SAMPLE_FMT_S16:
				case AV_SAMPLE_FMT_S16P:
					_swrDstSampleFormat = _swrSrcSampleFormat;
					copyDstChannelLayout();
					_outputChannels = 1;
					_outputSampleSize = sizeof(quint16);
					_outputFormat = AL_FORMAT_MONO16;
					break;
				}
			}
			else if (useStereo) {
				switch (_swrSrcSampleFormat) {
				case AV_SAMPLE_FMT_U8:
					_swrDstSampleFormat = _swrSrcSampleFormat;
					copyDstChannelLayout();
					_outputChannels = 2;
					_outputSampleSize = 2;
					_outputFormat = AL_FORMAT_STEREO8;
					break;
				case AV_SAMPLE_FMT_S16:
					_swrDstSampleFormat = _swrSrcSampleFormat;
					copyDstChannelLayout();
					_outputChannels = 2;
					_outputSampleSize = 2 * sizeof(quint16);
					_outputFormat = AL_FORMAT_STEREO16;
					break;
				}
			}

			createSpeedFilter(speed);

			return true;
		}

		auto AbstractAudioFFmpegReader::replaceFrameAndRead(
			FramePointer frame)
			-> ReadResult {
			_frame = std::move(frame);
			return readFromReadyFrame();
		}

		auto AbstractAudioFFmpegReader::readFromReadyContext(
			AVCodecContext* context)
			-> ReadResult {
			ReadResult result;
			if (_filterGraph) {
				AvErrorWrap error = av_buffersink_get_frame(
					_filterSink,
					_filteredFrame.get());
				if (!error) {
					if (!_filteredFrame->nb_samples) {
						result.errorCode = ReadError::Retry;
						return result;
					}
					result.bytes = QByteArray(
						reinterpret_cast<const char*>(
							_filteredFrame->extended_data[0]),
						_filteredFrame->nb_samples * _outputSampleSize);
					return result;
				}
				else if (error.code() == AVERROR_EOF) {
					result.errorCode = ReadError::EndOfFile;
					return result;
				}
				else if (error.code() != AVERROR(EAGAIN)) {

					result.errorCode = ReadError::Other;
					return result;
				}
			}
			using Enqueued = EnqueuedFrame*;
			const auto queueResult = fillFrameFromQueued();
			auto queueError = std::get_if<ReadError>(&queueResult);

			if (queueError && *queueError == ReadError::RetryNotQueued) {
				result.errorCode = ReadError::RetryNotQueued;
				return result;
			}
			if (const auto enqueued = std::get_if<Enqueued>(&queueResult)) {
				const auto raw = (*enqueued)->frame.get();

				return readOrBufferForFilter(raw, (*enqueued)->samples);
			}

			queueError = std::get_if<ReadError>(&queueResult);
			AvErrorWrap error = (queueError && *queueError == ReadError::EndOfFile)
				? AVERROR_EOF
				: avcodec_receive_frame(context, _frame.get());
			if (!error) {
				return readFromReadyFrame();
			}

			if (error.code() == AVERROR_EOF) {
				enqueueFramesFinished();
				if (!_filterGraph) {
					result.errorCode = ReadError::EndOfFile;
					return result;
				}
				AvErrorWrap error = av_buffersrc_add_frame(_filterSrc, nullptr);
				if (!error) {
					result.errorCode = ReadError::Retry;
					return result;
				}

				result.errorCode = ReadError::Other;
				return result;
			}
			else if (error.code() != AVERROR(EAGAIN)) {
				result.errorCode = ReadError::Other;
				return result;
			}

			result.errorCode = ReadError::Wait;
			return result;
		}

		auto AbstractAudioFFmpegReader::fillFrameFromQueued()
			-> std::variant<EnqueuedFrame*, ReadError> {
			if (_framesQueuedIndex == _framesQueued.size()) {
				_framesQueuedIndex = -1;
				return ReadError::RetryNotQueued;
			}
			else if (_framesQueuedIndex < 0) {
				return ReadError::Wait;
			}
			auto& queued = _framesQueued[_framesQueuedIndex];
			++_framesQueuedIndex;

			if (!queued.frame) {
				return ReadError::EndOfFile;
			}
			return &queued;
		}

		bool AbstractAudioFFmpegReader::frameHasDesiredFormat(
			AVFrame* frame) const {
			const auto sameChannelLayout = [&] {
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
				return !av_channel_layout_compare(
					&frame->ch_layout,
					&_swrDstChannelLayout);
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
				const auto frameChannelLayout = ComputeChannelLayout(
					frame->channel_layout,
					frame->channels);
				return (frameChannelLayout == _swrDstChannelLayout);
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
				};
			return true
				&& (frame->format == _swrDstSampleFormat)
				&& (frame->sample_rate == _swrDstRate)
				&& sameChannelLayout();
		}

		bool AbstractAudioFFmpegReader::initResampleForFrame() {
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			const auto bad = !_frame->ch_layout.nb_channels;
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			const auto frameChannelLayout = ComputeChannelLayout(
				_frame->channel_layout,
				_frame->channels);
			const auto bad = !frameChannelLayout;
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			if (bad) {

				return false;
			}
			else if (_frame->format == -1) {

				return false;
			}
			else if (_swrContext) {
				const auto sameChannelLayout = [&] {
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
					return !av_channel_layout_compare(
						&_frame->ch_layout,
						&_swrSrcChannelLayout);
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
					return (frameChannelLayout == _swrSrcChannelLayout);
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
					};
				if (true
					&& (_frame->format == _swrSrcSampleFormat)
					&& (_frame->sample_rate == _swrSrcRate)
					&& sameChannelLayout()) {
					return true;
				}
				swr_close(_swrContext);
			}

			_swrSrcSampleFormat = static_cast<AVSampleFormat>(_frame->format);
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			av_channel_layout_copy(&_swrSrcChannelLayout, &_frame->ch_layout);
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			_swrSrcChannelLayout = frameChannelLayout;
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			_swrSrcRate = _frame->sample_rate;
			return initResampleUsingFormat();
		}

		bool AbstractAudioFFmpegReader::initResampleUsingFormat() {
			AvErrorWrap error = 0;
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			error = swr_alloc_set_opts2(
				&_swrContext,
				&_swrDstChannelLayout,
				_swrDstSampleFormat,
				_swrDstRate,
				&_swrSrcChannelLayout,
				_swrSrcSampleFormat,
				_swrSrcRate,
				0,
				nullptr);
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			_swrContext = swr_alloc_set_opts(
				_swrContext,
				_swrDstChannelLayout,
				_swrDstSampleFormat,
				_swrDstRate,
				_swrSrcChannelLayout,
				_swrSrcSampleFormat,
				_swrSrcRate,
				0,
				nullptr);
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			if (error || !_swrContext) {

				return false;
			}
			else if (AvErrorWrap error = swr_init(_swrContext)) {

				return false;
			}
			_resampledFrame = nullptr;
			_resampledFrameCapacity = 0;
			return true;
		}

		bool AbstractAudioFFmpegReader::ensureResampleSpaceAvailable(int samples) {
			const auto enlarge = (_resampledFrameCapacity < samples);
			if (!_resampledFrame) {
				_resampledFrame = MakeFramePointer();
			}
			else if (enlarge || !av_frame_is_writable(_resampledFrame.get())) {
				av_frame_unref(_resampledFrame.get());
			}
			else {
				return true;
			}
			const auto allocate = std::max(samples, int(av_rescale_rnd(
				4096 / _outputSampleSize,
				_swrDstRate,
				_swrSrcRate,
				AV_ROUND_UP)));
			_resampledFrame->sample_rate = _swrDstRate;
			_resampledFrame->format = _swrDstSampleFormat;
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			av_channel_layout_copy(
				&_resampledFrame->ch_layout,
				&_swrDstChannelLayout);
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			_resampledFrame->channel_layout = _swrDstChannelLayout;
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			_resampledFrame->nb_samples = allocate;
			if (AvErrorWrap error = av_frame_get_buffer(_resampledFrame.get(), 0)) {

				return false;
			}
			_resampledFrameCapacity = allocate;
			return true;
		}

		bool AbstractAudioFFmpegReader::changeSpeedFilter(float speed) {
			speed = std::clamp(speed, 0.5f, 2.f);
			if (_filterSpeed == speed) {
				return false;
			}
			avfilter_graph_free(&_filterGraph);
			const auto guard = gsl::finally([&] {
				if (!_filterGraph) {
					_filteredFrame = nullptr;
					_filterSpeed = 1.;
				}
				});
			createSpeedFilter(speed);
			return true;
		}

		void AbstractAudioFFmpegReader::createSpeedFilter(float speed) {

			if (speed == 1.) {
				return;
			}
			const auto abuffer = avfilter_get_by_name("abuffer");
			const auto abuffersink = avfilter_get_by_name("abuffersink");
			const auto atempo = avfilter_get_by_name("atempo");
			if (!abuffer || !abuffersink || !atempo) {

				return;
			}

			auto graph = avfilter_graph_alloc();
			if (!graph) {

				return;
			}
			const auto guard = gsl::finally([&] {
				avfilter_graph_free(&graph);
				});

			_filterSrc = avfilter_graph_alloc_filter(graph, abuffer, "src");
			_atempo = avfilter_graph_alloc_filter(graph, atempo, "atempo");
			_filterSink = avfilter_graph_alloc_filter(graph, abuffersink, "sink");
			if (!_filterSrc || !atempo || !_filterSink) {

				return;
			}

			char layout[64] = { 0 };
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			av_channel_layout_describe(
				&_swrDstChannelLayout,
				layout,
				sizeof(layout));
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			av_get_channel_layout_string(
				layout,
				sizeof(layout),
				0,
				_swrDstChannelLayout);
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT

			av_opt_set(
				_filterSrc,
				"channel_layout",
				layout,
				AV_OPT_SEARCH_CHILDREN);
			av_opt_set_sample_fmt(
				_filterSrc,
				"sample_fmt",
				_swrDstSampleFormat,
				AV_OPT_SEARCH_CHILDREN);
			av_opt_set_q(
				_filterSrc,
				"time_base",
				AVRational{ 1, _swrDstRate },
				AV_OPT_SEARCH_CHILDREN);
			av_opt_set_int(
				_filterSrc,
				"sample_rate",
				_swrDstRate,
				AV_OPT_SEARCH_CHILDREN);
			av_opt_set_double(
				_atempo,
				"tempo",
				speed,
				AV_OPT_SEARCH_CHILDREN);

			AvErrorWrap error = 0;
			if ((error = avfilter_init_str(_filterSrc, nullptr))) {

				return;
			}
			else if ((error = avfilter_init_str(_atempo, nullptr))) {

				avfilter_graph_free(&graph);
				return;
			}
			else if ((error = avfilter_init_str(_filterSink, nullptr))) {

				avfilter_graph_free(&graph);
				return;
			}
			else if ((error = avfilter_link(_filterSrc, 0, _atempo, 0))) {

				avfilter_graph_free(&graph);
				return;
			}
			else if ((error = avfilter_link(_atempo, 0, _filterSink, 0))) {

				avfilter_graph_free(&graph);
				return;
			}
			else if ((error = avfilter_graph_config(graph, nullptr))) {

				return;
			}
			_filterGraph = std::exchange(graph, {});
			_filteredFrame = MakeFramePointer();
			_filterSpeed = speed;
		}

		void AbstractAudioFFmpegReader::enqueueNormalFrame(
			AVFrame* frame,
			int64_t samples) {
			if (_framesQueuedIndex >= 0) {
				return;
			}
			if (!samples) {
				samples = frame->nb_samples;
			}

			EnqueuedFrame enqueuedFrame;

			enqueuedFrame.position = startedAtSample() + _framesQueuedSamples;
			enqueuedFrame.samples = samples;
			enqueuedFrame.frame = DuplicateFramePointer(frame);

			_framesQueued.push_back(enqueuedFrame);
			_framesQueuedSamples += samples;
		}

		void AbstractAudioFFmpegReader::enqueueFramesFinished() {
			if (_framesQueuedIndex >= 0) {
				return;
			}
			EnqueuedFrame enqueuedFrame;
			enqueuedFrame.position = startedAtSample() + _framesQueuedSamples;

			_framesQueued.push_back(enqueuedFrame);
		}

		auto AbstractAudioFFmpegReader::readFromReadyFrame()
			-> ReadResult {
			ReadResult result;
			const auto raw = _frame.get();
			if (frameHasDesiredFormat(raw)) {
				if (!raw->nb_samples) {
					result.errorCode = ReadError::Retry;
					return result;
				}
				return readOrBufferForFilter(raw, raw->nb_samples);
			}
			else if (!initResampleForFrame()) {
				result.errorCode = ReadError::Other;
				return result;
			}

			const auto maxSamples = av_rescale_rnd(
				swr_get_delay(_swrContext, _swrSrcRate) + _frame->nb_samples,
				_swrDstRate,
				_swrSrcRate,
				AV_ROUND_UP);
			if (!ensureResampleSpaceAvailable(maxSamples)) {
				result.errorCode = ReadError::Other;
				return result;
			}
			const auto samples = swr_convert(
				_swrContext,
				(uint8_t**)_resampledFrame->extended_data,
				maxSamples,
				(const uint8_t**)_frame->extended_data,
				_frame->nb_samples);
			if (AvErrorWrap error = samples) {
				result.errorCode = ReadError::Other;
				return result;
			}
			else if (!samples) {
				result.errorCode = ReadError::Retry;
				return result;
			}
			return readOrBufferForFilter(_resampledFrame.get(), samples);
		}

		auto AbstractAudioFFmpegReader::readOrBufferForFilter(
			AVFrame* frame,
			int64_t samplesOverride)
			-> ReadResult {
			ReadResult result;
			enqueueNormalFrame(frame, samplesOverride);

			const auto was = frame->nb_samples;
			frame->nb_samples = samplesOverride;
			const auto guard = gsl::finally([&] {
				frame->nb_samples = was;
				});

			if (!_filterGraph) {
				result.bytes = QByteArray(
					reinterpret_cast<const char*>(frame->extended_data[0]),
					frame->nb_samples * _outputSampleSize);
				return result;
			}
			AvErrorWrap error = av_buffersrc_add_frame_flags(
				_filterSrc,
				frame,
				AV_BUFFERSRC_FLAG_KEEP_REF);
			if (error) {
				result.errorCode = ReadError::Other;
				return result;
			}
			result.errorCode = ReadError::Retry;
			return result;
		}

		AbstractAudioFFmpegReader::~AbstractAudioFFmpegReader() {
			if (_filterGraph) {
				avfilter_graph_free(&_filterGraph);
			}
			if (_swrContext) {
				swr_free(&_swrContext);
			}
		}

} // namespace base::media::ffmpeg::audio