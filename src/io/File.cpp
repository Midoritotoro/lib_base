#include <base/io/File.h>

#include <base/system/SystemTools.h>

#if defined(OS_WIN)
	#include <base/io/WindowsFileEngine.h>
	/* using FileEngine = base::io::WindowsFileEngine; */
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/io/UnixFileEngine.h>
	/* using FileEngine = base::io::UnixFileEngine; */
#endif


static [[nodiscard]] base::io::AbstractFileEngine* CreateFileEngine() {
	#if defined(OS_WIN)
		return new base::io::WindowsFileEngine();
	#elif defined(OS_LINUX) || defined(OS_MAC)
		return new base::system::UnixFileEngine();
	#endif
		return nullptr;
}

static [[nodiscard]] base::io::AbstractFileEngine*
	CreateFileEngine(const std::string& path) 
{
	#if defined(OS_WIN)
		return new base::io::WindowsFileEngine(path);
	#elif defined(OS_LINUX) || defined(OS_MAC)
		return new base::io::UnixFileEngine(path);
	#endif
		return nullptr;
}

static [[nodiscard]] base::io::AbstractFileEngine* 
	CreateFileEngine(
		not_null<FILE*> file,
		bool tryToExtractPathFromDescriptor)
{
	#if defined(OS_WIN)
		return new base::io::WindowsFileEngine(file, tryToExtractPathFromDescriptor);
	#elif defined(OS_LINUX) || defined(OS_MAC)
		return new base::io::UnixFileEngine(file, tryToExtractPathFromDescriptor);
	#endif
		return nullptr;
}

static [[nodiscard]] base::io::AbstractFileEngine* 
	CreateFileEngine(
		const std::string& path,
		not_null<FILE*> file,
		bool tryToExtractPathFromDescriptor)
{
	#if defined(OS_WIN)
		return new base::io::WindowsFileEngine(file);
	#elif defined(OS_LINUX) || defined(OS_MAC)
		return new base::io::UnixFileEngine(file);
	#endif
		return nullptr;
}


namespace base::io {
	File::File()
	{}

	File::File(const std::string& path) :
		_engine(CreateFileEngine(path))
	{}

	File::File(
		not_null<FILE*> file,
		bool tryToExtractPathFromDescriptor
	):
		_engine(CreateFileEngine(file, tryToExtractPathFromDescriptor))
	{}

	File::File(
		not_null<FILE*> file,
		const std::string& path
	):
		_engine(CreateFileEngine(path, file, false))
	{}

	File::~File() {
		if (_engine != nullptr)
			delete _engine;
	}

	bool File::exists(const std::string& path) {
		return _engine->exists(path);
	}

	std::vector<std::string>
		File::find(
			const std::string& path,
			const FileFilter& filter,
			bool recurse)
	{
		return {};
	}

	bool File::close() {
	//	SystemAssert(_desc != nullptr, "base::system::File: Попытка закрыть дескриптор файла, равный nullptr", false);
		return fclose(_desc) == 0;
	}

	bool File::open(
		const std::string& path,
		FileOpenModes format)
	{
		std::string mode;

		switch (format) {
		[[likely]] case FileOpenMode::Read:
			mode = "r";
			break;
		case FileOpenMode::Write:
			mode = "w";
			break;
		case FileOpenMode::Append:
			mode = "a";
			break;
		case FileOpenMode::ReadEx:
			mode = "r+";
			break;
		case FileOpenMode::WriteEx:
			mode = "r+";
			break;
		case FileOpenMode::AppendEx:
			mode = "r+";
			break;
		default:
			AssertUnreachable();
		}

		if (format & FileOpenMode::Binary)
			if (mode.size() == 1)
				mode.append("b");
			else if (mode.size() == 2) {
				mode[1] = 'b';
				mode[2] = '+';
			}

		_desc = fileOpenHelper(path.c_str(), mode.c_str());
//		SystemAssert(_desc != nullptr, "base::system::File::open: Не удалось открыть файл. Проверьте корректность пути", false);

		return true;
	}

	bool File::open(
		const std::string& path,
		const char* format)
	{
		_desc = utility::FileOpen(path.c_str(), format);
	//	SystemAssert(_desc != nullptr, "base::system::File::open: Не удалось открыть файл. Проверьте корректность пути", false);

		return true;
	}

	bool File::rename(const std::string& newFileName) {
		return false;
	}

	bool File::rename(
		const std::string& oldFileName,
		const std::string& newFileName)
	{
		return false;
	}

	bool File::rewind(sizetype position) {
	//	SystemAssert(_desc != nullptr, "base::system::File::open: Не удалось открыть файл. Проверьте корректность пути", false);
		return fseek(_desc, position, SEEK_CUR);
	}

	bool File::rewind(FilePositions position) {
		//SystemAssert(_desc != nullptr, "base::system::File::open: Не удалось открыть файл. Проверьте корректность пути", false);
		int result = 0;

		switch (position) {
			case FilePosition::FileBegin:
				result = fseek(_desc, SEEK_SET, SEEK_CUR);
				break;
			case FilePosition::FileEnd:
				result = fseek(_desc, SEEK_END, SEEK_CUR);
				break;
			default:
				AssertUnreachable();
		}

		return result == 0;
	}

	void File::remove() {

	}

	void File::remove(const std::string& path) {

	}

	sizetype File::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{
		return _engine->read(outBuffer, sizeInBytes);
	}

	sizetype File::fileSize() const noexcept {
		return 0;
	}
} // namespace base::io