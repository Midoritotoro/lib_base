#pragma once 


#include <base/core/io/AbstractDirectory.h>

#if defined(base_os_windows)

__BASE_IO_NAMESPACE_BEGIN

class WindowsDirectory final :
	public AbstractDirectory 
{
public:
	WindowsDirectory(const std::string& path);
	~WindowsDirectory();

	void setDirectoryName(const std::string& path) override;

	  std::string directoryName() const noexcept override;
	  std::string absolutePath() const noexcept override;

	static void mkdir(const std::string& path);

	  static bool exists(const std::string& path) noexcept;
	  bool exists() const noexcept override;

	  int64 size() const noexcept override;
private: 
	std::string _path;
};

__BASE_IO_NAMESPACE_END

#endif 