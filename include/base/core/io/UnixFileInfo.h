#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h>

__BASE_IO_NAMESPACE_BEGIN

#if defined(base_os_mac) || defined(base_os_linux)

class UnixFileInfo {
public:
    UnixFileInfo(const std::string& file);

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

      QDir dir() const;
      QDir absoluteDir() const;

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
};

#endif 

__BASE_IO_NAMESPACE_END
