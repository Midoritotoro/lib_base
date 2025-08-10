#pragma once 


#include <base/core/io/AbstractDirectory.h>

__BASE_IO_NAMESPACE_BEGIN

class Directory {
public:
	Directory(const std::string& path);
	~Directory();

	void setDirectoryName(const std::string& path);

	  std::string directoryName() const noexcept;
	  std::string absolutePath() const noexcept;

	static void mkdir(const std::string& path);

	  static bool exists(const std::string& path) noexcept;
	  bool exists() const noexcept;

	  int64 size() const noexcept;
private:
	AbstractDirectory* _private = nullptr;
};

__BASE_IO_NAMESPACE_END