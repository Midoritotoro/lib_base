#include <base/core/io/FileInfo.h>

__BASE_IO_NAMESPACE_BEGIN


FileInfo::FileInfo(const std::string& path):
	_private(new PlatformFileInfoImplementation(path))
{}

std::string FileInfo::absoluteFilePath() const
{
	return _private->absoluteFilePath();
}

std::string FileInfo::canonicalFilePath() const
{
	return _private->canonicalFilePath();
}

std::string FileInfo::absolutePath() const
{
	return _private->absolutePath();
}

std::string FileInfo::canonicalPath() const
{
	return _private->canonicalPath();
}

std::string FileInfo::path() const
{
	return _private->path();
}

bool FileInfo::isRelative() const
{
	return _private->isRelative();
}

bool FileInfo::makeAbsolute()
{
	return _private->makeAbsolute();
}

bool FileInfo::exists() const
{
	return _private->exists();
}

bool FileInfo::exists(const std::string& path)
{
	return PlatformFileInfoImplementation::exists(path);
}

void FileInfo::refresh()
{
	_private->refresh();
}

std::string FileInfo::filePath() const
{
	return _private->filePath();
}

std::string FileInfo::fileName() const
{
	return _private->fileName();
}

std::string FileInfo::bundleName() const
{
	return _private->bundleName();
}

std::string FileInfo::baseName() const
{
	return _private->baseName();
}

std::string FileInfo::completeBaseName() const
{
	return _private->completeBaseName();
}

std::string FileInfo::completeSuffix() const
{
	return _private->completeSuffix();
}

std::string FileInfo::suffix() const
{
	return _private->suffix();
}

Directory FileInfo::dir() const
{
	return _private->dir();
}

Directory FileInfo::absoluteDir() const
{
	return _private->absoluteDir();
}

bool FileInfo::isReadable() const
{
	return _private->isReadable();
}

bool FileInfo::isWritable() const
{
	return _private->isWritable();
}

bool FileInfo::isExecutable() const
{
	return _private->isExecutable();
}

bool FileInfo::isHidden() const
{
	return _private->isHidden();
}

bool FileInfo::isNativePath() const
{
	return _private->isNativePath();
}

bool FileInfo::isFile() const
{
	return _private->isFile();
}

bool FileInfo::isDir() const
{
	return _private->isDir();
}

bool FileInfo::isBundle() const
{
	return _private->isBundle();
}

bool FileInfo::isSymLink() const
{
	return _private->isSymLink();
}

bool FileInfo::isSymbolicLink() const
{
	return _private->isSymbolicLink();
}

bool FileInfo::isShortcut() const
{
	return _private->isShortcut();
}

bool FileInfo::isAlias() const
{
	return _private->isAlias();
}

bool FileInfo::isJunction() const
{
	return _private->isJunction();
}

bool FileInfo::isRoot() const
{
	return _private->isRoot();
}

int64 FileInfo::size() const
{
	return _private->size();
}

__BASE_IO_NAMESPACE_END
