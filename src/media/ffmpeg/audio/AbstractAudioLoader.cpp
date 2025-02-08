#include "AbstractAudioLoader.h"

 
namespace FFmpeg {

	AbstractAudioLoader::AbstractAudioLoader(const QByteArray& data) :
		_data(data)
	{
	}

	AbstractAudioLoader::~AbstractAudioLoader() {

	}

	void AbstractAudioLoader::saveDecodedSamples(QByteArray* samples) {
		samples->swap(_savedSamples);
		_holdsSavedSamples = true;
	}

	void AbstractAudioLoader::takeSavedDecodedSamples(
		QByteArray* samples) {
		samples->swap(_savedSamples);
		_holdsSavedSamples = false;
	}

	bool AbstractAudioLoader::holdsSavedDecodedSamples() const {
		return _holdsSavedSamples;
	}

	void AbstractAudioLoader::dropDecodedSamples() {
		_savedSamples = {};
		_holdsSavedSamples = false;
	}

	int AbstractAudioLoader::bytesPerBuffer() {
		if (!_bytesPerBuffer) {
			_bytesPerBuffer = samplesFrequency() * sampleSize();
		}
		return _bytesPerBuffer;
	}
} // FFmpeg