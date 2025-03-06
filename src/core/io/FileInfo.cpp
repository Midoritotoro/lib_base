#include <base/core/io/FileInfo.h>

__BASE_IO_NAMESPACE_BEGIN


FileInfo::FileInfo(const std::string& path):
	_private(new PlatformFileInfoImplementation(path))
{}

std::string FileInfo::absoluteFilePath() const
{
	return _private->av;
}

std::string FileInfo::canonicalFilePath() const
{
	return "";
}

std::string FileInfo::absolutePath() const
{
	return "";
}

std::string FileInfo::canonicalPath() const
{

}

std::string FileInfo::path() const
{

}

bool FileInfo::isRelative() const
{

}

bool FileInfo::makeAbsolute()
{

}

bool FileInfo::exists() const
{

}

bool FileInfo::exists(const std::string& path)
{

}

void FileInfo::refresh()
{

}

std::string FileInfo::filePath() const
{

}

std::string FileInfo::fileName() const
{
}

std::string FileInfo::bundleName() const
{

}

std::string FileInfo::baseName() const
{

}

std::string FileInfo::completeBaseName() const
{

}

std::string FileInfo::completeSuffix() const
{

}

std::string FileInfo::suffix() const
{

}

Directory FileInfo::dir() const
{

}

Directory FileInfo::absoluteDir() const
{

}

bool FileInfo::isReadable() const
{

}


bool FileInfo::isWritable() const
{

}

bool FileInfo::isExecutable() const
{

}

bool FileInfo::isHidden() const
{

}

bool FileInfo::isNativePath() const
{

}

bool FileInfo::isFile() const
{

}


bool FileInfo::isDir() const
{

}

bool FileInfo::isBundle() const
{

}


bool FileInfo::isSymLink() const
{

}


bool FileInfo::isSymbolicLink() const
{

}

bool FileInfo::isShortcut() const
{

}

bool FileInfo::isAlias() const
{

}

bool FileInfo::isJunction() const
{

}

bool FileInfo::isRoot() const
{

}

int64 FileInfo::size() const
{

}

__BASE_IO_NAMESPACE_END
