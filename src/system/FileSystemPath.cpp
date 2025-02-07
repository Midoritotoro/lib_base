#include <base/system/FileSystemPath.h>

#include <base/system/WindowsFileSystemPath.h>
#include <base/system/UnixFileSystemPath.h>

#include <base/system/SystemString.h>

#include <algorithm>
#include <tuple>


namespace base::system {
#if defined(OS_WIN)

	bool FileSystemPath::isAbsolutePath(const std::string& path) {
        return WindowsFileSystemPath::isAbsolutePath(path);
    }

    bool FileSystemPath::isRelativePath(const std::string& path) {
        return WindowsFileSystemPath::isAbsolutePath(path);
    }

    std::pair<std::string, std::string>
        FileSystemPath::split(const std::string& path)
    {
        return WindowsFileSystemPath::split(path);
    }

    std::string FileSystemPath::directoryName(const std::string& path) {
        return WindowsFileSystemPath::directoryName(path);
    }

    std::string FileSystemPath::baseName(const std::string& path) {
        return WindowsFileSystemPath::baseName(path);
    }

#elif defined(OS_MAC) || defined(OS_LINUX)

    bool FileSystemPath::isAbsolutePath(const std::string& path) {
        return UnixFileSystemPath::isAbsolutePath(path);
    }

    bool FileSystemPath::isRelativePath(const std::string& path) {
        return UnixFileSystemPath::isAbsolutePath(path);
    }

    std::pair<std::string, std::string>
        FileSystemPath::split(const std::string& path)
    {
        return UnixFileSystemPath::split(path);
    }

    std::string FileSystemPath::directoryName(const std::string& path) {
        return UnixFileSystemPath::directoryName(path);
    }

    std::string FileSystemPath::baseName(const std::string& path) {
        return UnixFileSystemPath::baseName(path);
    }

#endif
} // namespace base::system
