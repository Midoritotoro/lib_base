#pragma once 

#include <base/io/AbstractFileEngine.h>


// Все возвращаемые этим файлом исключения можно отключить,
// определив LIB_BASE_IO_NO_FAILURE, в противном случае ошибки, возникшие здесь, вызовут std::abort

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
		//! Ищет файл по пути path.
		//! \param path - Путь к каталогу для поиска
		//! \param output - Выходной вектор из путей к найденным файлам 
		//! \param filter - Параметры фильтрации файлов, по умолчанию отсутствуют
		//! \param recurse - Отвечает за рекурсивный обход всех вложенных папок по пути path.
		static void find(
			const std::string& path,
			std::vector<std::string>& output,
			const FileFilter& filter = {},
			bool recurse = true);

		//!
		//! \brief
		//! Ищет файл на всех дисках и подключённых внешних носителях
		//! \param output - Выходной вектор из путей к найденным файлам 
		//! \param filter - Параметры фильтрации файлов, по умолчанию отсутствуют
		//! \param recurse - Отвечает за рекурсивный обход всех вложенных папок по пути path.
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
		//! Перемещает указатель файла на заданную позицию
		[[nodiscard]] bool rewind(int64 position);
		[[nodiscard]] bool rewind(FilePositions position);

		void remove();
		static void remove(const std::string& path);

		//!
		//! \return В случае, если количество успешно записанных байтов
		//! равно sizeInBytes, возвращает true, в противном случае false
		static [[nodiscard]] bool write(
			const std::string& path,
			void* inBuffer,
			sizetype sizeInBytes,
			const char* mode);
		
		//!
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

		[[nodiscard]] ReadResult readAll();

		//!
		//! \brief
		//! \return Размер файла в байтах
		[[nodiscard]] sizetype fileSize() const noexcept;
		static [[nodiscard]] sizetype fileSize(const std::string& path);
	private:
		AbstractFileEngine* _engine = nullptr;
	};
} // namespace base::io