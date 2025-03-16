#pragma once 

#include <base/core/arch/Platform.h>

__BASE_IO_NAMESPACE_BEGIN

class AbstractDirectory {
public:
	virtual void setDirectoryName(const std::string& path) = 0;

	virtual std::string directoryName() const noexcept = 0;
	virtual std::string absolutePath() const noexcept = 0;

	virtual bool exists() const noexcept = 0;

	virtual int64 size() const noexcept = 0;
};

__BASE_IO_NAMESPACE_END