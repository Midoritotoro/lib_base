#pragma once 

#include <string>
#include <vector>

#include <base/Flags.h>

// ��� ������������ ���� ������ ���������� ����� ���������,
// ��������� LIB_BASE_SYSTEM_NO_FAILURE, � ��������� ������ ������, ��������� �����, ������� std::abort

namespace base::system {
	class File {
	public:
		enum Format : uchar {
			Read = 0x01, // "r"
			Write = 0x02, // "w"
			Append = 0x04, // "a"
			ReadEx = 0x10, // "r+"
			WriteEx = 0x20, // "w+"
			AppendEx = 0x40 // "a+"
#if defined(OS_WIN)
			,
			// ��� ��� � cstdlib fopen() ���� "b" ������������ � �������� POSIX,
			// �� � ������������ ��� ��� ������ ���. � Windows ���� "b" ��������� ��������� '\n' � '\x1A'.
			Binary = 0x100
#endif
		};

		DECLARE_FLAGS(Formats, Format);

		struct FileFilter {
			std::string nameContains = "";
			sizetype minimumSize = 0;
		};

		File();

		File(const char* path);
		File(const std::string& path);

		File(not_null<FILE*> file);

		~File();

		static [[nodiscard]] bool exists(const char* path);
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
				const char* path,
				const FileFilter& filter);

		static [[nodiscard]] std::vector<std::string>
			find(
				const std::string& path,
				const FileFilter& filter);
		
		[[nodiscard]] bool close();

		[[nodiscard]] bool open(
			const std::string& path,
			Formats format = Format::Read);
		[[nodiscard]] bool open(
			const char* path,
			Formats format = Format::Read);

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
		FILE* _desc = nullptr;
		std::string _path;
	};
} // namespace base::system