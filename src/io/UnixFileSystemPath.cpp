#include <base/io/UnixFileSystemPath.h>


#if defined(OS_MAC) || defined(OS_LINUX)

#include <algorithm>
#include <tuple>

#define sep std::string("/")


namespace base::io {
    bool UnixFileSystemPath::isAbsolutePath(const std::string& path) {
        return path.rfind(sep, 0) == 0;
    }

    bool UnixFileSystemPath::isRelativePath(const std::string& path) {
        return !isAbsolutePath(path);
    }

    std::pair<std::string, std::string> 
        UnixFileSystemPath::split(const std::string& p) 
    {
        size_t i = p.rfind(sep) + 1;

        std::string head = p.substr(0, i);
        std::string tail = p.substr(i);

        if (!head.empty() && head != std::string(head.length(), sep[0])) {
            size_t endpos = head.find_last_not_of(sep);

            std::string::npos != endpos
                ? head = head.substr(0, endpos + 1)
                : head = "";

        }

        return std::make_pair(head, tail);
    }

    std::string UnixFileSystemPath::directoryName(const std::string& path) {
        size_t i = path.rfind(sep) + 1;
        std::string head = path.substr(0, i);

        if (!head.empty() && head != std::string(head.length(), sep[0])) {
            size_t endpos = head.find_last_not_of(sep);

            std::string::npos != endpos
                ? head = head.substr(0, endpos + 1)
                : head = "";
        }

        return head;
    }

    std::string UnixFileSystemPath::baseName(const std::string& path) {
        size_t i = path.rfind(sep) + 1;
        return path.substr(i);
    }
} // namespace base::io

#endif