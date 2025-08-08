#pragma once 


#include <base/core/io/AbstractDirectory.h>

__BASE_IO_NAMESPACE_BEGIN

class Directory {
public:
	Directory(const std::string& path);
	~Directory();

	void setDirectoryName(const std::string& path);

	base_nodiscard std::string directoryName() const noexcept;
	base_nodiscard std::string absolutePath() const noexcept;

	static void mkdir(const std::string& path);

	base_nodiscard static bool exists(const std::string& path) noexcept;
	base_nodiscard bool exists() const noexcept;

	base_nodiscard int64 size() const noexcept;
private:
	AbstractDirectory* _private = nullptr;
};

__BASE_IO_NAMESPACE_END