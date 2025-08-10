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

      bool exists() const;
    static   bool exists(const std::string& file);

    void refresh();

      std::string filePath() const;

      std::string absoluteFilePath() const;
      std::string canonicalFilePath() const;

      std::string fileName() const;
      std::string baseName() const;

      std::string completeBaseName() const;
      std::string suffix() const;

      std::string bundleName() const;
      std::string completeSuffix() const;

      std::string path() const;

      std::string absolutePath() const;
      std::string canonicalPath() const;

      Directory dir() const;
      Directory absoluteDir() const;

      bool isReadable() const;
      bool isWritable() const;

      bool isExecutable() const;

      bool isHidden() const;
      bool isNativePath() const;

      bool isRelative() const;
      inline bool isAbsolute() const { return !isRelative(); }

      bool makeAbsolute();

      bool isFile() const;

      bool isDir() const;
      bool isSymLink() const;

      bool isSymbolicLink() const;
      bool isShortcut() const;

      bool isAlias() const;
      bool isJunction() const;

      bool isRoot() const;
      bool isBundle() const;

      int64 size() const;
private:
    PlatformFileInfoImplementation* _private = nullptr;
};

__BASE_IO_NAMESPACE_END
