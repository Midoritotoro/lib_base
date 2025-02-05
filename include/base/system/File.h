#pragma once 

#include <string>
#include <vector>

#include <base/Flags.h>

// Все возвращаемые этим файлом исключения можно отключить,
// определив LIB_BASE_SYSTEM_NO_FAILURE, в противном случае ошибки, возникшие здесь, вызовут std::abort

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
			// Так как в cstdlib fopen() флаг "b" игнорируется в системах POSIX,
			// то и использовать его там смысла нет. В Windows флаг "b" отключает обработку '\n' и '\x1A'.
			Binary = 0x80 // "b"
#endif
		};

		enum Position : uchar {
			FileBegin = 0x01,
			FileEnd = 0x02
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
		//! Ищет файл по пути path. В случае с установленным параметром recurse = true,
		//! обходит все вложенные папки, начиная с path
		//! \param path - Путь к каталогу для поиска
		//! \param filter - Параметры фильтрации файлов, по умолчанию отсутствуют
		//! \return Возвращает вектор из путей к найденным файлам 
		static [[nodiscard]] std::vector<std::string>
			find(
				const char* path,
				const FileFilter& filter,
				bool recurse = true);

		static [[nodiscard]] std::vector<std::string>
			find(
				const std::string& path,
				const FileFilter& filter,
				bool recurse = true);
		
		[[nodiscard]] bool close();

		[[nodiscard]] bool open(
			const std::string& path,
			Formats format);
		[[nodiscard]] bool open(
			const char* path,
			Formats format);

		
		[[nodiscard]] bool open(
			const std::string& path,
			const char* format);
		[[nodiscard]] bool open(
			const char* path,
			const char* format);

		[[nodiscard]] bool rename(const std::string newFileName);
		static [[nodiscard]] bool rename(
			const std::string& oldFileName, 
			const std::string newFileName);

		//!
		//! \brief
		//! Перемещает указатель файла на заданную позицию
		[[nodiscard]] bool rewind(sizetype position);
		[[nodiscard]] bool rewind(Position position);

		[[nodiscard]] void remove();
		[[nodiscard]] void remove(const std::string& path);
			
		//! \brief Читает sizeInBytes байт в outBuffer.
		//!
		//! Если sizeInBytes больше размера outBuffer и определен макрос
		//! LIB_BASE_SYSTEM_NO_FAILURE, возвращает 0. Если LIB_BASE_SYSTEM_NO_FAILURE
		//! не определен, вызывает std::abort().
		//! 
		//! Если sizeInBytes равно 0, возвращает 0.
		//!
		//! \param outBuffer - Указатель на массив, в который записываются прочитанные данные.
		//! \param sizeInBytes - Размер прочитанных данных в байтах.
		//!
		//! \return Количество прочитанных байт.
		[[nodiscard]] sizetype read(
			_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
			_SAL2_In_ sizetype sizeInBytes);

		//!
		//! \brief
		//! \return Размер файла в битах
		[[nodiscard]] sizetype fileSize() const noexcept;
	private:
		FILE* _desc = nullptr;
		std::string _path;
	};
} // namespace base::system