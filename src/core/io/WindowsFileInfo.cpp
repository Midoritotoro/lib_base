#include <base/core/io/WindowsFileInfo.h>

#include <base/core/io/IOConfig.h>

#include <array>


#ifdef OS_WIN 

__BASE_IO_NAMESPACE_BEGIN

// Path constants

#define WIN_ALTSEP      "/"
#define WIN_ALTSEP_LEN  2

#define WIN_SEPS        "\\/"

#define WIN_SEP         "//"
#define WIN_SEP_LEN     3
    
#define WIN_COLON       ":"

#define WIN_UNC_PREFIX  "\\\\?\\UNC\\"


static constexpr NODISCARD bool IsSlash(const char* ch) {
    return ch == "/" || ch == "\\";
}

static constexpr NODISCARD bool IsValidIterator(size_t iter) {
    return (iter != std::string::npos);
}

static constexpr auto windowsExecutableFormats = std::array{
    ".com",
    ".cmd",
    ".bat",
    ".exe"
};

WindowsFileInfo::WindowsFileInfo(const std::string& path):
	_path(path)
{
	IOAssert(_path.empty(), "base::io::WindowsFileInfo::WindowsFileInfo: Path cannot be empty. ", UNUSED(0));
	IOAssert(GetFileAttributesExA(
        _path.c_str(), 
        GetFileExMaxInfoLevel, &_data) != 0, 
        "base::io::WindowsFileInfo::WindowsFileInfo: Cannot get file attributes. ", UNUSED(0));
}

std::string WindowsFileInfo::absoluteFilePath() const {

}

std::string WindowsFileInfo::canonicalFilePath() const {

}

std::string WindowsFileInfo::absolutePath() const {

}

std::string WindowsFileInfo::canonicalPath() const {

}

std::string WindowsFileInfo::path() const {
	return _path;
}

bool WindowsFileInfo::isRelative() const {
    const auto first = _path.data();
    const auto last = _path.data() + _path.size();

    
}

bool WindowsFileInfo::isAbsolute() const {
	return !isRelative();
}

bool WindowsFileInfo::makeAbsolute() {

}

bool WindowsFileInfo::exists() const {
    return (_data.dwFileAttributes &~ INVALID_FILE_ATTRIBUTES);
}

bool WindowsFileInfo::exists(const std::string& path) {
    WIN32_FILE_ATTRIBUTE_DATA attrs;
    IOAssert(GetFileAttributesExA(path.c_str(),
        GetFileExInfoStandard, &attrs) != 0,
        "base::io::WindowsFileInfo::WindowsFileInfo: Cannot get file attributes. ", false);

    return (attrs.dwFileAttributes &~ INVALID_FILE_ATTRIBUTES);
}

void WindowsFileInfo::refresh() {

}

std::string WindowsFileInfo::filePath() const {
    return _path;
}

std::string WindowsFileInfo::fileName() const {
    const auto lastSeparator = _path.find_last_of(WIN_SEPS, 0);
    const auto suffixStart = _path.find_last_of(".", 0) + 1;

    if (IsValidIterator(suffixStart) == false || IsValidIterator(lastSeparator) == false)
        return "";

    return _path.substr(lastSeparator, _path.size() - suffixStart);
}

std::string WindowsFileInfo::bundleName() const {
	return "";
}

std::string WindowsFileInfo::baseName() const {
    return splitPath(_path).second;
}

std::string WindowsFileInfo::completeBaseName() const {
    return "";
}

std::string WindowsFileInfo::completeSuffix() const {

}

std::string WindowsFileInfo::suffix() const {
    const auto suffixStart = _path.find_last_of(".", 0) + 1;
    
    if (suffixStart == std::string::npos)
        return "";

    return _path.substr(suffixStart, _path.size() - suffixStart);
}

// "D:/dir/file.cpp"

Directory WindowsFileInfo::dir() const {
    return Directory(splitPath(_path).first);
}

Directory WindowsFileInfo::absoluteDir() const {

}

bool WindowsFileInfo::isReadable() const {
    
}

bool WindowsFileInfo::isWritable() const {

}

bool WindowsFileInfo::isExecutable() const {
    const auto _Suffix = suffix();

    for (int i = 0; i < windowsExecutableFormats.size(); ++i)
        if (_Suffix == windowsExecutableFormats[i])
            return true;

    return false;
}

bool WindowsFileInfo::isHidden() const {
	return (_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
}

bool WindowsFileInfo::isNativePath() const {

}

bool WindowsFileInfo::isFile() const {
	return (_data.dwFileAttributes &~ FILE_ATTRIBUTE_DIRECTORY);
}

bool WindowsFileInfo::isDir() const {
	return !isFile();
}

bool WindowsFileInfo::isBundle() const {
	return false;
}

bool WindowsFileInfo::isSymLink() const {

}

bool WindowsFileInfo::isSymbolicLink() const {

}

bool WindowsFileInfo::isShortcut() const {

}

bool WindowsFileInfo::isAlias() const {
 
}

bool WindowsFileInfo::isJunction() const {
    
}

bool WindowsFileInfo::isRoot() const {
   
}

int64 WindowsFileInfo::size() const {
    if (_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        return 0;

    int64 _Size = 0;

    _Size = _data.nFileSizeHigh;
    _Size <<= 32;

    _Size += _data.nFileSizeLow;

    return _Size;
}

WindowsFileInfo::PathParts
	WindowsFileInfo::splitRoot(const std::string& path) const
{
    std::string tempPath = path;
    std::string normp = tempPath.replace(
        path.find(WIN_ALTSEP), 
        WIN_ALTSEP_LEN, WIN_SEP);

    if (!normp.empty() && normp[0] == WIN_SEP[0]) {
        if (normp.length() > 1 && normp[1] == WIN_SEP[0]) {
            size_t start = (normp.length() >= 8 && normp.substr(0, 8)
                .rfind(WIN_UNC_PREFIX, 0) == 0) ? 8 : 2;

            size_t index = normp.find(WIN_SEP, start);

            if (index == std::string::npos)
                return std::make_tuple(path, "", "");

            size_t index2 = normp.find(WIN_SEP, index + 1);

            if (index2 == std::string::npos)
                return std::make_tuple(path, "", "");

            return std::make_tuple(path.substr(0, index2),
                path.substr(index2, 1), path.substr(index2 + 1));
        }
        else
            return std::make_tuple("", path.substr(0, 1), path.substr(1));
    }

    else if (normp.length() > 1 && normp[1] == WIN_COLON[0])

        if (normp.length() > 2 && normp[2] == WIN_SEP[0])
            return std::make_tuple(path.substr(0, 2), path.substr(2, 1), path.substr(3));
        else
            return std::make_tuple(path.substr(0, 2), "", path.substr(2));

    else
        return std::make_tuple("", "", path);
}

WindowsFileInfo::PathHeadTail
	WindowsFileInfo::splitPath(const std::string& _path) const
{
    std::tuple<std::string, std::string, std::string> 
        root_parts = splitRoot(_path);

    std::string path = std::get<2>(root_parts);

    size_t i = path.length();

    while (i > 0 && std::string(WIN_SEPS).find(path[i - 1]) == std::string::npos)
        --i;

    std::string head = path.substr(0, i);
    std::string tail = path.substr(i);

    size_t endpos = head.find_last_not_of(WIN_SEPS);

    std::string::npos != endpos
        ? head = head.substr(0, endpos + 1)
        : head = "";

    return std::make_pair(
        std::get<0>(root_parts) + 
        std::get<1>(root_parts) + head,
        tail);
}

__BASE_IO_NAMESPACE_END

#endif