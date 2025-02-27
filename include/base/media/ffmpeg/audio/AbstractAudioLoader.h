#include <base/system/Time.h>
#include <base/media/ffmpeg/Utility.h>

#include <deque>

namespace base::media::ffmpeg::audio {

	class AbstractAudioLoader {
	public:
		AbstractAudioLoader(const QByteArray& data);
		virtual ~AbstractAudioLoader();

		virtual bool open(Time::time_t positionMs, float speed = 1.) = 0;
		virtual Time::time_t duration() = 0;

		virtual int samplesFrequency() = 0;
		virtual int sampleSize() = 0;

		virtual int format() = 0;

		virtual void dropFramesTill(int64_t samples) {
		}
		[[nodiscard]] virtual int64_t startReadingQueuedFrames(float newSpeed) {
			return 0;
		}

		[[nodiscard]] int bytesPerBuffer();

		enum class ReadError {
			Other,
			Retry,
			RetryNotQueued,
			Wait,
			EndOfFile,
		};

		struct ReadResult {
			QByteArray bytes = nullptr;
			ReadError errorCode;
		};
		
		virtual void enqueuePackets(std::deque<Packet>&& packets) {

		}
		virtual void setForceToBuffer(bool force) {

		}
		virtual bool forceToBuffer() const {
			return false;
		}

		void saveDecodedSamples(QByteArray* samples);
		void takeSavedDecodedSamples(QByteArray* samples);
		bool holdsSavedDecodedSamples() const;
		void dropDecodedSamples();

	protected:
		[[nodiscard]] virtual ReadResult readMore() = 0;

		bool _access = false;
		QByteArray _data;

		QFile _f;
		int _dataPos = 0;

	private:
		QByteArray _savedSamples;
		bool _holdsSavedSamples = false;

		int _bytesPerBuffer = 0;

	};
} // namespace FFmpeg