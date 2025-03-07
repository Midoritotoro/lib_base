#include <base/core/io/WindowsFileSystemPath.h>

#include <algorithm>
#include <tuple>

#define sep std::string("\\")


static std::tuple<std::string, std::string, std::string> splitroot(const std::string& p) {
    std::string altsep = "/";

    std::string colon = ":";
    std::string unc_prefix = "\\\\?\\UNC\\";

    std::string empty = "";

    std::string temp = p;
    std::string normp = temp.replace(p.find(altsep), altsep.length(), sep);

    if (!normp.empty() && normp[0] == sep[0]) {
        if (normp.length() > 1 && normp[1] == sep[0]) {
            size_t start = (normp.length() >= 8 && normp.substr(0, 8)
                .rfind(unc_prefix, 0) == 0) ? 8 : 2;

            size_t index = normp.find(sep, start);

            if (index == std::string::npos)
                return std::make_tuple(p, empty, empty);

            size_t index2 = normp.find(sep, index + 1);

            if (index2 == std::string::npos)
                return std::make_tuple(p, empty, empty);

            return std::make_tuple(p.substr(0, index2), 
                p.substr(index2, 1), p.substr(index2 + 1));
        }
        else
            return std::make_tuple(empty, p.substr(0, 1), p.substr(1));
    }

    else if (normp.length() > 1 && normp[1] == colon[0])

        if (normp.length() > 2 && normp[2] == sep[0])
            return std::make_tuple(p.substr(0, 2), p.substr(2, 1), p.substr(3));
        else
            return std::make_tuple(p.substr(0, 2), empty, p.substr(2));
  
    else
        return std::make_tuple(empty, empty, p);
}

__BASE_IO_NAMESPACE_BEGIN

bool WindowsFileSystemPath::isAbsolutePath(const std::string& path) {
    std::string altsep = "/";

    std::string colon_sep = ":\\";
    std::string double_sep = "\\\\";

    std::string s = path.substr(0, std::min((size_t)3, path.length()));

    std::transform(s.begin(), s.end(), s.begin(), [&altsep](unsigned char c) {
        return (altsep.find(c) != std::string::npos) ? sep[0] : c;
    });

    return (s.length() >= colon_sep.length() && s.substr(1, colon_sep.length() - 1) ==
        colon_sep.substr(1) && s[0] >= 'A' && s[0] <= 'Z') ||
        (s.length() >= double_sep.length() &&
        s.substr(0, double_sep.length()) == double_sep);
}

bool WindowsFileSystemPath::isRelativePath(const std::string& path) {
    return !isAbsolutePath(path);
}

std::pair<std::string, std::string>
    WindowsFileSystemPath::split(const std::string& p)
{
    std::string seps = "\\/";
    std::tuple<std::string, std::string, std::string> root_parts = splitroot(p);

    std::string d = std::get<0>(root_parts);
    std::string r = std::get<1>(root_parts);

    std::string path = std::get<2>(root_parts);

    size_t i = path.length();

    while (i > 0 && seps.find(path[i - 1]) == std::string::npos)
        --i;

    std::string head = path.substr(0, i);
    std::string tail = path.substr(i);

    size_t endpos = head.find_last_not_of(seps);

    std::string::npos != endpos
        ? head = head.substr(0, endpos + 1)
        : head = "";

    return std::make_pair(d + r + head, tail);
}

std::string WindowsFileSystemPath::directoryName(const std::string& path) {
    return split(path).first;
}

std::string WindowsFileSystemPath::baseName(const std::string& path) {
    return split(path).second;
}

__BASE_IO_NAMESPACE_END
