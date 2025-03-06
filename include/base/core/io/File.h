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

	static NODISCARD bool exists(const std::string& path);

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

	NODISCARD bool open(
		const std::string& path,
		FileOpenModes mode);

	NODISCARD bool open(FileOpenModes mode);

	NODISCARD bool rename(const std::string& newFileName);
	static NODISCARD bool rename(
		const std::string& oldFileName, 
		const std::string& newFileName);

	NODISCARD bool rewind(int64 position);
	NODISCARD bool rewind(FilePositions position);

	void remove();
	static void remove(const std::string& path);

	static NODISCARD bool write(
		const std::string& path,
		const char* inBuffer,
		sizetype sizeInBytes);
		
	NODISCARD ReadResult read(sizetype sizeInBytes);
	NODISCARD ReadResult readAll();

	NODISCARD sizetype fileSize() const noexcept;
	static NODISCARD sizetype fileSize(const std::string& path);
private:
	AbstractFileEngine* _engine = nullptr;
};

__BASE_IO_NAMESPACE_END