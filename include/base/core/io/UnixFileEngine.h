#pragma once

#include <base/core/arch/Platform.h>

#if defined(base_os_mac) || defined(base_os_linux)

#include <base/core/io/AbstractFileEngine.h>

__BASE_IO_NAMESPACE_BEGIN

class UnixFileEngine final : public AbstractFileEngine {
public:
	UnixFileEngine();
	UnixFileEngine(const std::string& path);

	UnixFileEngine(
		not_null<FILE*> file,
		bool tryToExtractPathFromDescriptor = false);

	UnixFileEngine(
		not_null<FILE*> file,
		const std::string& path);

	~UnixFileEngine() override;

	void setFileName(const std::string& path) override;
	void setFileDescriptor(not_null<FILE*> file) override;

	  bool isOpened() const noexcept override;

	  FILE* fileDescriptor() const noexcept override;
	  std::string path() const noexcept override;

	static   bool exists(const std::string& path);

	static void find(
		const base_string& path,
		std::vector<base_string>& output,
		const FileFilter& filter = {},
		bool recurse = true);

	static void find(
		std::vector<base_string>& output,
		const FileFilter& filter = {},
		bool recurse = true);

	void close() override;

	  std::string absolutePathFromDescriptor(FILE* descriptor) override;

	  bool open(
		const std::string& path,
		FileOpenModes mode) override;
	  bool open(
		const std::string& path,
		const char* mode) override;

	  bool rename(const std::string& newFileName) override;
	  bool rename(
		const std::string& oldFileName,
		const std::string& newFileName) override;

	  bool rewind(sizetype position) override;
	  bool rewind(FilePositions position) override;

	  void remove() override;
	  void remove(const std::string& path) override;

	  sizetype read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes) override;

	  sizetype fileSize() const noexcept override;
private:
	FILE* _desc = nullptr;
	std::string _path = "";
};

__BASE_IO_NAMESPACE_END

#endif