#pragma once 

#include <base/system/AbstractFileEngine.h>


// ��� ������������ ���� ������ ���������� ����� ���������,
// ��������� LIB_BASE_SYSTEM_NO_FAILURE, � ��������� ������ ������, ��������� �����, ������� std::abort

namespace base::system {
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
		//! ���� ���� �� ���� path. � ������ � ������������� ���������� recurse = true,
		//! ������� ��� ��������� �����, ������� � path
		//! \param path - ���� � �������� ��� ������
		//! \param filter - ��������� ���������� ������, �� ��������� �����������
		//! \return ���������� ������ �� ����� � ��������� ������ 
		static [[nodiscard]] std::vector<std::string>
			find(
				const std::string& path,
				const FileFilter& filter,
				bool recurse = true);


		static void find(
			const FileFilter& filter,
			std::vector<base_string>& output,
			bool recurse = true);
		
		[[nodiscard]] bool close();

		[[nodiscard]] bool open(
			const std::string& path,
			FileOpenModes mode);
		[[nodiscard]] bool open(
			const std::string& path,
			const char* mode);

		[[nodiscard]] bool rename(const std::string& newFileName);
		static [[nodiscard]] bool rename(
			const std::string& oldFileName, 
			const std::string& newFileName);

		//!
		//! \brief
		//! ���������� ��������� ����� �� �������� �������
		[[nodiscard]] bool rewind(sizetype position);
		[[nodiscard]] bool rewind(FilePositions position);

		[[nodiscard]] void remove();
		[[nodiscard]] void remove(const std::string& path);
		
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

		//!
		//! \brief
		//! \return ������ ����� � �����
		[[nodiscard]] sizetype fileSize() const noexcept;
	private:
		AbstractFileEngine* _engine = nullptr;
	};
} // namespace base::system