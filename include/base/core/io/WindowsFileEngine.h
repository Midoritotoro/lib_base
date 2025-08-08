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
		base_nodiscard bool isOpened() const noexcept override;

		base_nodiscard std::string path() const noexcept override;

		static base_nodiscard bool exists(const std::string& path);

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

		base_nodiscard std::string absolutePathFromDescriptor(FILE* descriptor) override;

		base_nodiscard bool open(
			const std::string& path,
			FileOpenModes mode) override;
		base_nodiscard bool open(
			const std::string& path,
			int mode) override;

		base_nodiscard bool rename(const std::string& newFileName) override;
		static bool rename(
			const std::string& oldFileName,
			const std::string& newFileName);

		base_nodiscard bool rewind(int64 position) override;
		base_nodiscard bool rewind(FilePositions position) override;

		void remove() override;
		static void remove(const std::string& path);

		static base_nodiscard bool write(
			const std::string& path,
			const char* inBuffer,
			sizetype sizeInBytes);

		base_nodiscard ReadResult read(sizetype sizeInBytes) override;
		base_nodiscard ReadResult readAll() override;

		static base_nodiscard sizetype fileSize(const std::string& path);
		base_nodiscard sizetype fileSize() const noexcept;
	private:
		enum ReadType : uchar {
			Value,
			All
		};

		base_nodiscard bool read(
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