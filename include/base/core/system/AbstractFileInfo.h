#pragma once 

#include <base/system/Platform.h>
#include <string>

__BASE_IO_NAMESPACE_BEGIN

class Directory;
class AbstractFileInfo {
public:
    virtual bool exists() const = 0;
    virtual void refresh() = 0;

    virtual std::string filePath() const = 0;
    virtual std::string path() const = 0;

    virtual std::string absoluteFilePath() const = 0;
    virtual std::string canonicalFilePath() const = 0;

    virtual std::string fileName() const = 0;
    virtual std::string baseName() const = 0;

    virtual std::string completeBaseName() const = 0;
    virtual std::string suffix() const = 0;

    virtual std::string bundleName() const = 0;
    virtual std::string completeSuffix() const = 0;

    virtual std::string absolutePath() const = 0;
    virtual std::string canonicalPath() const = 0;

    virtual Directory dir() const = 0;
    virtual Directory absoluteDir() const = 0;

    virtual bool isReadable() const = 0;
    virtual bool isWritable() const = 0;

    virtual bool isExecutable() const = 0;

    virtual bool isHidden() const = 0;
    virtual bool isNativePath() const = 0;

    virtual bool isRelative() const = 0;
    virtual bool isAbsolute() const = 0;

    virtual bool makeAbsolute() = 0;
    virtual bool isFile() const = 0;

    virtual bool isDir() const = 0;
    virtual bool isSymLink() const = 0;

    virtual bool isSymbolicLink() const = 0;
    virtual bool isShortcut() const = 0;

    virtual bool isAlias() const = 0;
    virtual bool isJunction() const = 0;

    virtual bool isRoot() const = 0;
    virtual bool isBundle() const = 0;

    virtual int64 size() const = 0;
};

__BASE_IO_NAMESPACE_END
