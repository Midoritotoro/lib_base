#pragma once 

#include <base/io/AbstractFileEngine.h>
#include <base/io/WindowsSmartHandle.h>


#if defined(OS_WIN)

__BASE_IO_NAMESPACE_BEGIN

class WindowsFileEngine final: 
	public AbstractFileEngine 
{
	public:
		WindowsFileEngine();
		WindowsFileEngine(const std::string& path);

		virtual ~WindowsFileEngine();

		void setFileName(const std::string& path) override;
		NODISCARD bool isOpened() const noexcept override;

		NODISCARD std::string path() const noexcept override;

		static NODISCARD bool exists(const std::string& path);

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

		NODISCARD std::string absolutePathFromDescriptor(FILE* descriptor) override;

		NODISCARD bool open(
			const std::string& path,
			FileOpenModes mode) override;
		NODISCARD bool open(
			const std::string& path,
			int mode) override;

		NODISCARD bool rename(const std::string& newFileName) override;
		static bool rename(
			const std::string& oldFileName,
			const std::string& newFileName);

		NODISCARD bool rewind(int64 position) override;
		NODISCARD bool rewind(FilePositions position) override;

		void remove() override;
		static void remove(const std::string& path);

		static NODISCARD bool write(
			const std::string& path,
			const char* inBuffer,
			sizetype sizeInBytes);

		NODISCARD ReadResult read(sizetype sizeInBytes) override;
		NODISCARD ReadResult readAll() override;

		static NODISCARD sizetype fileSize(const std::string& path);
		NODISCARD sizetype fileSize() const noexcept;
	private:
		enum ReadType : uchar {
			Value,
			All
		};

		NODISCARD bool readSSE2(
			__m128i* outVector,
			uchar* tempOutBuffer,
			uchar* outBuffer,
			sizetype sizeInBytesRequiredForReading,
			sizetype* successfullyReadedBytes);

		NODISCARD bool readSSSE3(
			__m256i* outVector,
			uchar* tempOutBuffer,
			uchar* outBuffer,
			sizetype sizeInBytesRequiredForReading,
			sizetype* successfullyReadedBytes);

		NODISCARD bool readSSE4_1(
			__m512i* outVector,
			uchar* tempOutBuffer,
			uchar* outBuffer,
			sizetype sizeInBytesRequiredForReading,
			sizetype* successfullyReadedBytes);

		NODISCARD bool readSSE4_2(
			__m512i* outVector,
			uchar* tempOutBuffer,
			uchar* outBuffer,
			sizetype sizeInBytesRequiredForReading,
			sizetype* successfullyReadedBytes);

		NODISCARD bool readNoSimd(
			ReadResult* outBuffer,
			uchar* tempOutBuffer,
			sizetype sizeInBytesRequiredForReading,
			sizetype* successfullyReadedBytes
		);

		NODISCARD bool read(
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