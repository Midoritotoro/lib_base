#include <base/io/File.h>

#include <base/system/SystemTools.h>

#if defined(OS_WIN)
	#include <base/io/WindowsFileEngine.h>
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/io/UnixFileEngine.h>
#endif


static FILE* fileOpenHelper(
	char const* filename,
	char const* mode)
{
	FILE* file = nullptr;
#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	wchar_t wMode[64];
	wchar_t wFilename[1024];
	if (ConvertUnicodeToWChar(wFilename, ARRAY_SIZE(wFilename), filename) == 0)
		return nullptr;

	if (ConvertUnicodeToWChar(wMode, ARRAY_SIZE(wFilename), mode) == 0)
		return nullptr;

#if defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != _wfopen_s(&file, wFilename, wMode))
		file = 0;
#else
	file = _wfopen(wFilename, wMode);
#endif

#elif defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != fopen_s(&file, filename, mode))
		file = 0;
#else
	file = fopen(filename, mode);
#endif
	return file;
}

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
		return false;
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
	//	SystemAssert(_desc != nullptr, "base::system::File: ������� ������� ���������� �����, ������ nullptr", false);
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
//		SystemAssert(_desc != nullptr, "base::system::File::open: �� ������� ������� ����. ��������� ������������ ����", false);

		return true;
	}

	bool File::open(
		const std::string& path,
		const char* format)
	{
		_desc = utility::FileOpen(path.c_str(), format);
	//	SystemAssert(_desc != nullptr, "base::system::File::open: �� ������� ������� ����. ��������� ������������ ����", false);

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
	//	SystemAssert(_desc != nullptr, "base::system::File::open: �� ������� ������� ����. ��������� ������������ ����", false);
		return fseek(_desc, position, SEEK_CUR);
	}

	bool File::rewind(FilePositions position) {
		//SystemAssert(_desc != nullptr, "base::system::File::open: �� ������� ������� ����. ��������� ������������ ����", false);
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