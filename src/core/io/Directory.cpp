#include <base/core/io/Directory.h> 

#if defined(OS_WIN)
	#include <base/core/io/WindowsDirectory.h> 
	using PlatformDirectoryImplementation = base::io::WindowsDirectory;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/io/UnixDirectory.h> 
	using PlatformDirectoryImplementation = base::io::UnixDirectory;
#endif

__BASE_IO_NAMESPACE_BEGIN


Directory::Directory(const std::string& path) {
	_private = new PlatformDirectoryImplementation(path);
}

Directory::~Directory() {
	delete _private;
	_private = nullptr;
}

void Directory::setDirectoryName(const std::string& path) {
	_private->setDirectoryName(path);
}

std::string Directory::directoryName() const noexcept {
	return _private->directoryName();
}

std::string Directory::absolutePath() const noexcept {
	return _private->absolutePath();
}

void Directory::mkdir(const std::string& path) {
	return PlatformDirectoryImplementation::mkdir(path);
}

bool Directory::exists(const std::string& path) noexcept {
	return PlatformDirectoryImplementation::exists(path);
}

bool Directory::exists() const noexcept {
	return _private->exists();
}

int64 Directory::size() const noexcept {
	return _private->size();
}

__BASE_IO_NAMESPACE_END