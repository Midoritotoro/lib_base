#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/io/IOConfig.h>

#include <base/core/BaseNamespace.h>
#include <vector>


__BASE_IO_NAMESPACE_BEGIN

class AbstractFileEngine {
public:
	virtual ~AbstractFileEngine() {};

	virtual void setFileName(const std::string& path) = 0;

	virtual void close() = 0;

	virtual base_nodiscard std::string path() const noexcept = 0;
	virtual base_nodiscard bool isOpened() const noexcept = 0;

	virtual base_nodiscard std::string absolutePathFromDescriptor(FILE* descriptor) = 0;

	virtual base_nodiscard bool open(
		const std::string& path,
		FileOpenModes mode) = 0;
	virtual base_nodiscard bool open(
		const std::string& path,
		int mode) = 0;

	virtual base_nodiscard bool rename(const std::string& newFileName) = 0;

	virtual base_nodiscard bool rewind(int64 position) = 0;
	virtual base_nodiscard bool rewind(FilePositions position) = 0;

	virtual void remove() = 0;

	virtual base_nodiscard ReadResult read(sizetype sizeInBytes) = 0;
	virtual base_nodiscard ReadResult readAll() = 0;
};

__BASE_IO_NAMESPACE_END
