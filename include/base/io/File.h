#pragma once 

#include <base/io/AbstractFileEngine.h>


// ��� ������������ ���� ������ ���������� ����� ���������,
// ��������� LIB_BASE_IO_NO_FAILURE, � ��������� ������ ������, ��������� �����, ������� std::abort

namespace base::io {
	class File {
	public:
		File();
		File(const std::string& path);

		File(
			not_null<FILE*> file,
			bool tryToExtractPathFromDescriptor = false);

		File(
			not_null<FILE*> file,
			const std::string& path);

		~File();

		static [[nodiscard]] bool exists(const std::string& path);

		//!
		//! \brief
		//! ���� ���� �� ���� path.
		//! \param path - ���� � �������� ��� ������
		//! \param output - �������� ������ �� ����� � ��������� ������ 
		//! \param filter - ��������� ���������� ������, �� ��������� �����������
		//! \param recurse - �������� �� ����������� ����� ���� ��������� ����� �� ���� path.
		static void find(
			const std::string& path,
			std::vector<std::string>& output,
			const FileFilter& filter = {},
			bool recurse = true);

		//!
		//! \brief
		//! ���� ���� �� ���� ������ � ������������ ������� ���������
		//! \param output - �������� ������ �� ����� � ��������� ������ 
		//! \param filter - ��������� ���������� ������, �� ��������� �����������
		//! \param recurse - �������� �� ����������� ����� ���� ��������� ����� �� ���� path.
		static void find(
			std::vector<std::string>& output,
			const FileFilter& filter = {},
			bool recurse = true);
		
		void close();

		[[nodiscard]] bool open(
			const std::string& path,
			FileOpenModes mode);
		[[nodiscard]] bool open(
			const std::string& path,
			const char* mode);

		[[nodiscard]] bool open(FileOpenModes mode);
		[[nodiscard]] bool open(const char* mode);

		[[nodiscard]] bool rename(const std::string& newFileName);
		static [[nodiscard]] bool rename(
			const std::string& oldFileName, 
			const std::string& newFileName);

		//!
		//! \brief
		//! ���������� ��������� ����� �� �������� �������
		[[nodiscard]] bool rewind(int64 position);
		[[nodiscard]] bool rewind(FilePositions position);

		void remove();
		static void remove(const std::string& path);

		//!
		//! \return � ������, ���� ���������� ������� ���������� ������
		//! ����� sizeInBytes, ���������� true, � ��������� ������ false
		static [[nodiscard]] bool write(
			const std::string& path,
			void* inBuffer,
			sizetype sizeInBytes,
			const char* mode);
		
		//!
		//! \brief ������ sizeInBytes ���� � outBuffer.
		//!
		//! ���� sizeInBytes ������ ������� outBuffer � ��������� ������
		//! LIB_BASE_SYSTEM_NO_FAILURE, ���������� 0. ���� LIB_BASE_SYSTEM_NO_FAILURE
		//! �� ���������, �������� std::abort().
		//! 
		//! ���� sizeInBytes ����� 0, ���������� 0.
		//!
		//! \param outBuffer - ��������� �� ������, � ������� ������������ ����������� ������.
		//! \param sizeInBytes - ������ ����������� ������ � ������.
		//!
		//! \return ���������� ����������� ����.
		[[nodiscard]] sizetype read(
			_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
			_SAL2_In_ sizetype sizeInBytes);

		[[nodiscard]] ReadResult readAll();

		//!
		//! \brief
		//! \return ������ ����� � ������
		[[nodiscard]] sizetype fileSize() const noexcept;
		static [[nodiscard]] sizetype fileSize(const std::string& path);
	private:
		AbstractFileEngine* _engine = nullptr;
	};
} // namespace base::io