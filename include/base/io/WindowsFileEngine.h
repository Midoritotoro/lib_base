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
			void* inBuffer,
			sizetype sizeInBytes,
			FileOpenModes mode);

		NODISCARD sizetype read(
			_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
			_SAL2_In_ sizetype sizeInBytes) override;

		NODISCARD ReadResult readAll() override;

		static NODISCARD sizetype fileSize(const std::string& path);
	private:
		NODISCARD sizetype readSSE2(
			__m128i* outVector,
			uchar* tempOutBuffer,
			void* outBuffer,
			sizetype sizeInBytes);

		NODISCARD sizetype readSSSE3(
			__m256i* outVector,
			uchar* tempOutBuffer,
			void* outBuffer,
			sizetype sizeInBytes);

		NODISCARD sizetype readSSE4_1(
			__m512i* outVector,
			uchar* tempOutBuffer,
			void* outBuffer,
			sizetype sizeInBytes);

		NODISCARD sizetype readSSE4_2(
			__m512i* outVector,
			uchar* tempOutBuffer,
			void* outBuffer,
			sizetype sizeInBytes);




		io::WindowsSmartHandle _handle;
		std::string _path = "";
};

__BASE_IO_NAMESPACE_END

#endif