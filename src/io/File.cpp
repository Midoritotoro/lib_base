#include <base/io/File.h>

#include <base/system/SystemTools.h>

#if defined(OS_WIN)
	#include <base/io/WindowsFileEngine.h>
	using FileEngine = base::io::WindowsFileEngine;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/io/UnixFileEngine.h>
	using FileEngine = base::io::UnixFileEngine;
#endif


namespace base::io {
	File::File():
		_engine(new FileEngine())
	{}

	File::File(const std::string& path) :
		_engine(new FileEngine(path))
	{}

	File::File(
		not_null<FILE*> file,
		bool tryToExtractPathFromDescriptor
	):
		_engine(new FileEngine(file, tryToExtractPathFromDescriptor))
	{}

	File::File(
		not_null<FILE*> file,
		const std::string& path
	):
		_engine(new FileEngine(file, path))
	{}

	File::~File() {
		if (_engine != nullptr)
			delete _engine;
	}

	bool File::exists(const std::string& path) {
		return FileEngine::exists(path);
	}

	void File::find(
		const std::string& path,
		std::vector<std::string>& output,
		const FileFilter& filter,
		bool recurse)
	{
		FileEngine::find(path, output, filter, recurse);
	}

	void File::find(
		std::vector<std::string>& output,
		const FileFilter& filter,
		bool recurse)
	{
		FileEngine::find(output, filter, recurse);
	}

	void File::close() {
		_engine->close();
	}

	bool File::open(
		const std::string& path,
		FileOpenModes format)
	{
		return _engine->open(path, format);
	}

	bool File::open(
		const std::string& path,
		const char* format)
	{
		return _engine->open(path, format);
	}

	bool File::open(FileOpenModes format) {
		return _engine->open(_engine->path(), format);
	}

	bool File::open(const char* format) {
		return _engine->open(_engine->path(), format);
	}

	bool File::rename(const std::string& newFileName) {
		return _engine->rename(newFileName);
	}

	bool File::rename(
		const std::string& oldFileName,
		const std::string& newFileName)
	{
		return FileEngine::rename(oldFileName, newFileName);
	}

	bool File::rewind(int64 position) {
		return _engine->rewind(position);
	}

	bool File::rewind(FilePositions position) {
		return _engine->rewind(position);
	}

	void File::remove() {
		if (_engine->isOpened())
			close();

		_engine->remove();
	}

	void File::remove(const std::string& path) {
		FileEngine::remove(path);
	}

	bool File::write(
		const std::string& path,
		void* inBuffer,
		sizetype sizeInBytes,
		const char* mode)
	{
		return FileEngine::write(
			path, inBuffer, 
			sizeInBytes, mode);
	}

	sizetype File::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{
		return _engine->read(outBuffer, sizeInBytes);
	}

	ByteArray File::readAll() {
		return _engine->readAll();
	}

	sizetype File::fileSize() const noexcept {
		return FileEngine::fileSize(_engine->path());
	}

	sizetype File::fileSize(const std::string& path) {
		return FileEngine::fileSize(path);
	}

	FILE* File::fileDescriptor() const noexcept {
		return _engine->fileDescriptor();
	}
} // namespace base::io