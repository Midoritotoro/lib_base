#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h>

#include <base/core/io/Directory.h>
#include <string>
#include <tuple>


__BASE_IO_NAMESPACE_BEGIN

#ifdef OS_WIN

class WindowsFileInfo {
public:
    WindowsFileInfo(const std::string& file);

    NODISCARD bool exists() const;
    static NODISCARD bool exists(const std::string& file);

    void refresh();

    NODISCARD std::string filePath() const;

    NODISCARD std::string absoluteFilePath() const;
    NODISCARD std::string canonicalFilePath() const;

    NODISCARD std::string fileName() const;
    NODISCARD std::string baseName() const;

    NODISCARD std::string completeBaseName() const;
    NODISCARD std::string suffix() const;

    NODISCARD std::string bundleName() const;
    NODISCARD std::string completeSuffix() const;

    NODISCARD std::string path() const;

    NODISCARD std::string absolutePath() const;
    NODISCARD std::string canonicalPath() const;

    NODISCARD Directory dir() const;
    NODISCARD Directory absoluteDir() const;

    NODISCARD bool isReadable() const;
    NODISCARD bool isWritable() const;

    NODISCARD bool isExecutable() const;

    NODISCARD bool isHidden() const;
    NODISCARD bool isNativePath() const;

    NODISCARD bool isRelative() const;
    NODISCARD bool isAbsolute() const;

    NODISCARD bool makeAbsolute();

    NODISCARD bool isFile() const;

    NODISCARD bool isDir() const;
    NODISCARD bool isSymLink() const;

    NODISCARD bool isSymbolicLink() const;
    NODISCARD bool isShortcut() const;

    NODISCARD bool isAlias() const;
    NODISCARD bool isJunction() const;

    NODISCARD bool isRoot() const;
    NODISCARD bool isBundle() const;

    NODISCARD int64 size() const;
private:
    using PathHeadTail = std::pair<std::string, std::string>;
    using PathParts    = std::tuple<std::string, std::string, std::string>;

    NODISCARD PathParts
        splitRoot(const std::string& path) const;

    NODISCARD PathHeadTail
        splitPath(const std::string& path) const;

    std::string _path = "";
    WIN32_FILE_ATTRIBUTE_DATA _data;
};

#endif 


__BASE_IO_NAMESPACE_END
