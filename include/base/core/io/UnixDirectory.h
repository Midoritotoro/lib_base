#pragma once 


#include <base/core/io/AbstractDirectory.h>


#if defined(OS_MAC) || defined(OS_LINUX) 

__BASE_IO_NAMESPACE_BEGIN

class UnixDirectory final :
	public AbstractDirectory
{
public:
	UnixDirectory(const std::string& path);
	~UnixDirectory();

	void setDirectoryName(const std::string& path) override;

	NODISCARD std::string directoryName() const noexcept override;
	NODISCARD std::string absolutePath() const noexcept override;

	static void mkdir(const std::string& path);

	NODISCARD static bool exists(const std::string& path) noexcept;
	NODISCARD bool exists() const noexcept override;

	NODISCARD int64 size() const noexcept override;
private:
	std::string _path;
};

__BASE_IO_NAMESPACE_END

#endif  