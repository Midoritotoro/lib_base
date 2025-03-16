#include <base/core/io/WindowsDirectory.h>


__BASE_IO_NAMESPACE_BEGIN

WindowsDirectory::WindowsDirectory(const std::string& path) {

}

WindowsDirectory::~WindowsDirectory() {

}

void WindowsDirectory::setDirectoryName(const std::string& path) {

}

std::string WindowsDirectory::directoryName() const noexcept {
	return "";
}

std::string WindowsDirectory::absolutePath() const noexcept {
	return "";
}

void WindowsDirectory::mkdir(const std::string& path) {

}

bool WindowsDirectory::exists(const std::string& path) noexcept {
	return false;
}

bool WindowsDirectory::exists() const noexcept {
	return false;
}

int64 WindowsDirectory::size() const noexcept {
	return 0;
}

__BASE_IO_NAMESPACE_END
