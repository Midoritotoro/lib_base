#include <base/core/io/UnixFileEngine.h>

#if defined(base_os_mac) || defined(base_os_linux)

__BASE_IO_NAMESPACE_BEGIN

UnixFileEngine::UnixFileEngine()
{}

UnixFileEngine::UnixFileEngine(const std::string& path) :
	_path(path)
{}

UnixFileEngine::UnixFileEngine(
	not_null<FILE*> file,
	bool tryToExtractPathFromDescriptor
):
	_desc(file)
{
	if (tryToExtractPathFromDescriptor)
		_path = absolutePathFromDescriptor(_desc);
}

UnixFileEngine::UnixFileEngine(
	not_null<FILE*> file,
	const std::string& path
):
	_desc(file),
	_path(path)
{}

UnixFileEngine::~UnixFileEngine() {
	if (_desc != nullptr)
		fclose(_desc);
}

void UnixFileEngine::setFileName(const std::string& path) {
	_path = path;
}

void UnixFileEngine::setFileDescriptor(not_null<FILE*> file) {
	_desc = file;
}

FILE* UnixFileEngine::fileDescriptor() const noexcept {
	return _desc;
}

std::string UnixFileEngine::path() const noexcept {
	return _path;
}

bool UnixFileEngine::exists(const std::string& path) {

}

std::vector<std::string>
	UnixFileEngine::find(
		const std::string& path,
		const FileFilter& filter,
		bool recurse)
{

}

std::string UnixFileEngine::absolutePathFromDescriptor(FILE* descriptor) {
	static const char* err = "base::system::AbsolutePathFromDescriptor: Не удается извлечь путь из нулевого дескриптора файла. ";
	SystemAssert(descriptor != nullptr, err, "");

	int fd = _fileno(descriptor);
	SystemAssert(fd != -1, err, "");

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
}
} // namespace base::io

__BASE_IO_NAMESPACE_END

#endif