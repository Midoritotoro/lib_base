#pragma once 

#include <base/system/Platform.h>
#include <base/io/IOConfig.h>

#include <base/core/BaseNamespace.h>
#include <vector>


__BASE_IO_NAMESPACE_BEGIN

class AbstractFileEngine {
public:
	virtual ~AbstractFileEngine() {};

	virtual void setFileName(const std::string& path) = 0;

	virtual void close() = 0;

	virtual NODISCARD std::string path() const noexcept = 0;
	virtual NODISCARD bool isOpened() const noexcept = 0;

	virtual NODISCARD std::string absolutePathFromDescriptor(FILE* descriptor) = 0;

	virtual NODISCARD bool open(
		const std::string& path,
		FileOpenModes mode) = 0;
	virtual NODISCARD bool open(
		const std::string& path,
		int mode) = 0;

	virtual NODISCARD bool rename(const std::string& newFileName) = 0;

	virtual NODISCARD bool rewind(int64 position) = 0;
	virtual NODISCARD bool rewind(FilePositions position) = 0;

	virtual void remove() = 0;

	virtual NODISCARD ReadResult read(sizetype sizeInBytes) = 0;
	virtual NODISCARD ReadResult readAll() = 0;
};

__BASE_IO_NAMESPACE_END
