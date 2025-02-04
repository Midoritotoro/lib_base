#include <base/system/File.h>
#include <base/Assert.h>

#if defined(OS_MAC) || defined(OS_LINUX)
	#include <unistd.h>
	#include <fcntl.h>
#elif defined(OS_WIN)
	#include <io.h> // Для _get_osfhandle
#endif


#include <limits>  // Для PATH_MAX

#ifdef LIB_BASE_SYSTEM_NO_FAILURE
	#define SystemAssert						AssertReturn
#else
	#define SystemAssert(cond, mes, retval)		(AssertValidationCondition(\
		cond,\
		mes,\
		SOURCE_FILE_BASENAME,\
		__LINE__))
#endif

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)

extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(
	unsigned int cp, unsigned long flags,
	const char* str, int cbmb, wchar_t* widestr, int cchwide);

extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(
	unsigned int cp, unsigned long flags, const wchar_t* widestr,
	int cchwide, char* str, int cbmb, const char* defchar, int* used_default);

#endif

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)

int ConvertWCharToUnicode(
	char* buffer,
	size_t bufferlen,
	const wchar_t* input)
{
	return WideCharToMultiByte(
		CP_UTF8, 0, input, -1,
		buffer, (int)bufferlen, NULL, NULL);
}

int ConvertUnicodeToWChar(
	wchar_t* buffer,
	size_t bufferlen,
	const char* input)
{
	return MultiByteToWideChar(
		CP_UTF8, 0, input, -1,
		buffer, (int)bufferlen);
}

#endif

static FILE* fileOpenHelper(
	char const* filename,
	char const* mode)
{
	FILE* f = nullptr;
#if defined(_WIN32) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	wchar_t wMode[64];
	wchar_t wFilename[1024];
	if (ConvertUnicodeToWChar(wFilename, ARRAY_SIZE(wFilename), filename) == 0)
		return nullptr;

	if (ConvertUnicodeToWChar(wMode, ARRAY_SIZE(wFilename), mode) == 0)
		return nullptr;

#if defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != _wfopen_s(&f, wFilename, wMode))
		f = 0;
#else
	f = _wfopen(wFilename, wMode);
#endif

#elif defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != fopen_s(&f, filename, mode))
		f = 0;
#else
	f = fopen(filename, mode);
#endif
	return f;
}




namespace base::system {
	namespace {
		[[nodiscard]] std::string AbsolutePathFromDescriptor(FILE* descriptor) {
			static const char* err = "base::system::AbsolutePathFromDescriptor: Не удается извлечь путь из нулевого дескриптора файла. ";
			SystemAssert(descriptor != nullptr, err, "");

			int fd = _fileno(descriptor);
			SystemAssert(fd != -1, err, "");

#if defined(OS_WIN)
			HANDLE handle = (HANDLE)_get_osfhandle(fd); // Получаем HANDLE
			SystemAssert(handle != INVALID_HANDLE_VALUE, err, "");

			std::vector<wchar_t> buffer(MAX_PATH);
			DWORD length = GetFinalPathNameByHandleW(
				handle, buffer.data(), MAX_PATH,
				FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

			SystemAssert(length != 0, err, "");

			if (length > MAX_PATH) {
				buffer.resize(length);
				length = GetFinalPathNameByHandleW(handle, buffer.data(), length, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

				SystemAssert(length != 0, err, "");
			}

			std::wstring wpath(buffer.data());

			SystemAssert(!wpath.empty(), err, "");

			int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wpath[0], (int)wpath.size(), NULL, 0, NULL, NULL);
			std::string path(size_needed, 0);

			WideCharToMultiByte(CP_UTF8, 0, &wpath[0], (int)wpath.size(), &path[0], size_needed, NULL, NULL);

			return path;
#elif defined(OS_MAC) || defined(OS_LINUX)
			char path[PATH_MAX];
#if defined(OS_MAC)
			SystemAssert(fcntl(fd, F_GETPATH, path) != -1, err, "");
#else
			char proc_path[64];
			snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);

			ssize_t len = readlink(proc_path, path, sizeof(path) - 1);
			SystemAssert(len != -1, err, "");

			path[len] = '\0';
#endif

			return std::string(path);
#endif
			return "";
		}


	} // namespace 
	File::File()
	{}

	File::File(const char* path):
		_path(path)
	{}

	File::File(const std::string& path) :
		_path(path)
	{}

	File::File(not_null<FILE*> file):
		_desc(file),
		_path(AbsolutePathFromDescriptor(file))
	{}

	File::~File() {
		fclose(_desc);
	}

	bool File::exists(const char* path) {

	}

	bool File::exists(const std::string& path) {

	}

	std::vector<std::string>
		File::find(
			const char* path,
			const FileFilter& filter)
	{
		return {};
	}

	std::vector<std::string>
		File::find(
			const std::string& path,
			const FileFilter& filter)
	{
		return {};
	}

	bool File::close() {
		return fclose(_desc) == 0;
	}

	bool File::open(
		const std::string& path,
		Formats format)
	{
		return false;
	}

	bool File::open(
		const char* path,
		Formats format)
	{
		char mode[4];
		/*switch (format) {
			[[likely]] case Format::Read:
				fileOpenHelper(path, "r");
				break;
			}
		}*/

	}


	sizetype File::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{

	}

	sizetype File::fileSize() const noexcept {
		return 0;
	}
} // namespace base::system