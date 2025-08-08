#pragma once

#include <base/media/ffmpeg/audio/AbstractAudioReader.h>

//#include <al.h>
//#include <alc.h>

/** Unsigned 8-bit mono buffer format. */
#define AL_FORMAT_MONO8                          0x1100
/** Signed 16-bit mono buffer format. */
#define AL_FORMAT_MONO16                         0x1101
/** Unsigned 8-bit stereo buffer format. */
#define AL_FORMAT_STEREO8                        0x1102
/** Signed 16-bit stereo buffer format. */
#define AL_FORMAT_STEREO16                       0x1103


namespace base::media::ffmpeg::audio {
	class AbstractAudioFFmpegReader : public AbstractAudioReader {
	public:
		AbstractAudioFFmpegReader(const QByteArray& data);

		void dropFramesTill(int64_t samples) override;
		int64_t startReadingQueuedFrames(float newSpeed) override;

		int samplesFrequency() override {
			return _swrDstRate;
		}

		int sampleSize() override {
			return _outputSampleSize;
		}

		int format() override {
			return _outputFormat;
		}

		~AbstractAudioFFmpegReader();

	protected:
		bool initUsingContext(AVCodecContext* context, float speed);
		base_nodiscard ReadResult readFromReadyContext(
			AVCodecContext* context);

		base_nodiscard ReadResult replaceFrameAndRead(FramePointer frame);

	private:
		base_nodiscard ReadResult readFromReadyFrame();
		base_nodiscard ReadResult readOrBufferForFilter(
			AVFrame* frame,
			int64_t samplesOverride);
		bool frameHasDesiredFormat(AVFrame* frame) const;
		bool initResampleForFrame();
		bool initResampleUsingFormat();
		bool ensureResampleSpaceAvailable(int samples);

		bool changeSpeedFilter(float speed);
		void createSpeedFilter(float speed);

		void enqueueNormalFrame(
			AVFrame* frame,
			int64_t samples = 0);
		void enqueueFramesFinished();
		base_nodiscard auto fillFrameFromQueued()
			-> std::variant<EnqueuedFrame*, ReadError>;

		FramePointer _frame;
		FramePointer _resampledFrame;
		FramePointer _filteredFrame;
		int _resampledFrameCapacity = 0;

		int64_t _framesQueuedSamples = 0;
		std::deque<EnqueuedFrame> _framesQueued;

		int _framesQueuedIndex = -1;

		int _outputFormat = AL_FORMAT_STEREO16;
		int _outputChannels = 2;
		int _outputSampleSize = 2 * sizeof(quint16);

		SwrContext* _swrContext = nullptr;

		int _swrSrcRate = 0;
		AVSampleFormat _swrSrcSampleFormat = AV_SAMPLE_FMT_NONE;

		const int _swrDstRate = 48000;
		AVSampleFormat _swrDstSampleFormat = AV_SAMPLE_FMT_S16;

#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
		AVChannelLayout _swrSrcChannelLayout = AV_CHANNEL_LAYOUT_STEREO;
		AVChannelLayout _swrDstChannelLayout = AV_CHANNEL_LAYOUT_STEREO;
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
		uint64_t _swrSrcChannelLayout = 0;
		uint64_t _swrDstChannelLayout = AV_CH_LAYOUT_STEREO;
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT

		AVFilterGraph* _filterGraph = nullptr;
		float _filterSpeed = 1.;
		AVFilterContext* _filterSrc = nullptr;
		AVFilterContext* _atempo = nullptr;
		AVFilterContext* _filterSink = nullptr;

	};

} // namespace base::media::ffmpeg::audio