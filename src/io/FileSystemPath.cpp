#include <base/io/FileSystemPath.h>
#include <base/system/SystemString.h>

#include <algorithm>
#include <tuple>


#if defined(OS_WIN)
    #include <base/io/WindowsFileSystemPath.h>
    using PlatformFileSystemPath = base::io::WindowsFileSystemPath;
#elif defined(OS_MAC) || defined(OS_LINUX)
    #include <base/io/UnixFileSystemPath.h>
    using PlatformFileSystemPath = base::io::UnixFileSystemPath;
#endif


namespace base::io {
	bool FileSystemPath::isAbsolutePath(const std::string& path) {
        return PlatformFileSystemPath::isAbsolutePath(path);
    }

    bool FileSystemPath::isRelativePath(const std::string& path) {
        return PlatformFileSystemPath::isAbsolutePath(path);
    }

    std::pair<std::string, std::string>
        FileSystemPath::split(const std::string& path)
    {
        return PlatformFileSystemPath::split(path);
    }

    std::string FileSystemPath::directoryName(const std::string& path) {
        return PlatformFileSystemPath::directoryName(path);
    }

    std::string FileSystemPath::baseName(const std::string& path) {
        return PlatformFileSystemPath::baseName(path);
    }
} // namespace base::io
