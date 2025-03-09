#pragma once 


#include <base/core/io/AbstractDirectory.h>

__BASE_IO_NAMESPACE_BEGIN

class Directory {
public:
	Directory(const std::string& path);
	~Directory();

	void setDirectoryName(const std::string& path);

	NODISCARD std::string directoryName() const noexcept;
	NODISCARD std::string absolutePath() const noexcept;

	static void mkdir(const std::string& path);

	NODISCARD static bool exists(const std::string& path) noexcept;
	NODISCARD bool exists() const noexcept;

	NODISCARD int64 size() const noexcept;
private:
	AbstractDirectory* _private = nullptr;
};

__BASE_IO_NAMESPACE_END