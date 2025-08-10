#pragma once 

#include <base/core/io/AbstractFileEngine.h>
#include <base/core/io/WindowsSmartHandle.h>


#if defined(base_os_windows)

__BASE_IO_NAMESPACE_BEGIN

class WindowsFileEngine final: 
	public AbstractFileEngine 
{
	public:
		WindowsFileEngine();
		WindowsFileEngine(const std::string& path);

		virtual ~WindowsFileEngine();

		void setFileName(const std::string& path) override;
		  bool isOpened() const noexcept override;

		  std::string path() const noexcept override;

		static   bool exists(const std::string& path);

		static void find(
			const std::string& path,
			std::vector<std::string>& output,
			const FileFilter& filter = {},
			bool recurse = true);

		static void find(
			std::vector<std::string>& output,
			const FileFilter& filter = {},
			bool recurse = true);

		void close() override;

		  std::string absolutePathFromDescriptor(FILE* descriptor) override;

		  bool open(
			const std::string& path,
			FileOpenModes mode) override;
		  bool open(
			const std::string& path,
			int mode) override;

		  bool rename(const std::string& newFileName) override;
		static bool rename(
			const std::string& oldFileName,
			const std::string& newFileName);

		  bool rewind(int64 position) override;
		  bool rewind(FilePositions position) override;

		void remove() override;
		static void remove(const std::string& path);

		static   bool write(
			const std::string& path,
			const char* inBuffer,
			sizetype sizeInBytes);

		  ReadResult read(sizetype sizeInBytes) override;
		  ReadResult readAll() override;

		static   sizetype fileSize(const std::string& path);
		  sizetype fileSize() const noexcept;
	private:
		enum ReadType : uchar {
			Value,
			All
		};

		  bool read(
			ReadType type,
			ReadResult* outBuffer,
			sizetype sizeInBytes = 0);

		// Расширение до 32768 символов в пути
		static void expandPath(std::string& path);

		io::WindowsSmartHandle _handle;
		std::string _path = "";

		SECURITY_ATTRIBUTES _securityAttributes;
};

__BASE_IO_NAMESPACE_END

#endif