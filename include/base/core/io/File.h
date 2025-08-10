#pragma once 

#include <base/core/io/AbstractFileEngine.h>


// Все возвращаемые этим файлом исключения можно отключить,
// определив LIB_BASE_IO_NO_FAILURE, в противном случае ошибки, возникшие здесь, вызовут std::abort
__BASE_IO_NAMESPACE_BEGIN

class File {
public:
	File();
	File(const std::string& path);

	~File();

	static bool exists(const std::string& path);

	static void find(
		const std::string& path,
		std::vector<std::string>& output,
		const FileFilter& filter = {},
		bool recurse = true);

	static void find(
		std::vector<std::string>& output,
		const FileFilter& filter = {},
		bool recurse = true);
		
	void close();

	bool open(
		const std::string& path,
		FileOpenModes mode);

	bool open(FileOpenModes mode);

	bool rename(const std::string& newFileName);
	static  bool rename(
		const std::string& oldFileName, 
		const std::string& newFileName);

	  bool rewind(int64 position);
	  bool rewind(FilePositions position);

	void remove();
	static void remove(const std::string& path);

	static   bool write(
		const std::string& path,
		const char* inBuffer,
		sizetype sizeInBytes);
		
	  ReadResult read(sizetype sizeInBytes);
	  ReadResult readAll();

	  sizetype fileSize() const noexcept;
	static   sizetype fileSize(const std::string& path);
private:
	AbstractFileEngine* _engine = nullptr;
};

__BASE_IO_NAMESPACE_END