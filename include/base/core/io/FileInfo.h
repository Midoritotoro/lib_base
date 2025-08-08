#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h>

#include <base/core/io/Directory.h>
#include <string>

#if defined(base_os_windows) 
	#include <base/core/io/WindowsFileInfo.h>
	using PlatformFileInfoImplementation = base::io::WindowsFileInfo;
#elif defined(base_os_mac) || defined(base_os_linux)
	#include <base/core/io/UnixFileInfo.h>
	using PlatformFileInfoImplementation = base::io::UnixFileInfo;
#endif

__BASE_IO_NAMESPACE_BEGIN

class FileInfo {
public:
    FileInfo(const std::string& file);

    base_nodiscard bool exists() const;
    static base_nodiscard bool exists(const std::string& file);

    void refresh();

    base_nodiscard std::string filePath() const;

    base_nodiscard std::string absoluteFilePath() const;
    base_nodiscard std::string canonicalFilePath() const;

    base_nodiscard std::string fileName() const;
    base_nodiscard std::string baseName() const;

    base_nodiscard std::string completeBaseName() const;
    base_nodiscard std::string suffix() const;

    base_nodiscard std::string bundleName() const;
    base_nodiscard std::string completeSuffix() const;

    base_nodiscard std::string path() const;

    base_nodiscard std::string absolutePath() const;
    base_nodiscard std::string canonicalPath() const;

    base_nodiscard Directory dir() const;
    base_nodiscard Directory absoluteDir() const;

    base_nodiscard bool isReadable() const;
    base_nodiscard bool isWritable() const;

    base_nodiscard bool isExecutable() const;

    base_nodiscard bool isHidden() const;
    base_nodiscard bool isNativePath() const;

    base_nodiscard bool isRelative() const;
    base_nodiscard inline bool isAbsolute() const { return !isRelative(); }

    base_nodiscard bool makeAbsolute();

    base_nodiscard bool isFile() const;

    base_nodiscard bool isDir() const;
    base_nodiscard bool isSymLink() const;

    base_nodiscard bool isSymbolicLink() const;
    base_nodiscard bool isShortcut() const;

    base_nodiscard bool isAlias() const;
    base_nodiscard bool isJunction() const;

    base_nodiscard bool isRoot() const;
    base_nodiscard bool isBundle() const;

    base_nodiscard int64 size() const;
private:
    PlatformFileInfoImplementation* _private = nullptr;
};

__BASE_IO_NAMESPACE_END
