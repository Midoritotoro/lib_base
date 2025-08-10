#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h>

#include <base/core/io/Directory.h>
#include <string>
#include <tuple>


__BASE_IO_NAMESPACE_BEGIN

#if defined(base_os_windows)

class WindowsFileInfo {
public:
    WindowsFileInfo(const std::string& file);

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
      bool isAbsolute() const;

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
    using PathHeadTail = std::pair<std::string, std::string>;
    using PathParts    = std::tuple<std::string, std::string, std::string>;

      PathParts
        splitRoot(const std::string& path) const;

      PathHeadTail
        splitPath(const std::string& path) const;

    std::string _path = "";
    WIN32_FILE_ATTRIBUTE_DATA _data;
};

#endif 


__BASE_IO_NAMESPACE_END
