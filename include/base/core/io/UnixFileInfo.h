#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h>

__BASE_IO_NAMESPACE_BEGIN

#if defined(OS_MAC) || defined(OS_LINUX)

class UnixFileInfo {
public:
    UnixFileInfo(const std::string& file);

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

    NODISCARD QDir dir() const;
    NODISCARD QDir absoluteDir() const;

    NODISCARD bool isReadable() const;
    NODISCARD bool isWritable() const;

    NODISCARD bool isExecutable() const;

    NODISCARD bool isHidden() const;
    NODISCARD bool isNativePath() const;

    NODISCARD bool isRelative() const;
    NODISCARD inline bool isAbsolute() const { return !isRelative(); }

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
};

#endif 

__BASE_IO_NAMESPACE_END
