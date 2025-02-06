#pragma once 

#include <base/system/AbstractFileEngine.h>


// Все возвращаемые этим файлом исключения можно отключить,
// определив LIB_BASE_SYSTEM_NO_FAILURE, в противном случае ошибки, возникшие здесь, вызовут std::abort

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
		//! Ищет файл по пути path. В случае с установленным параметром recurse = true,
		//! обходит все вложенные папки, начиная с path
		//! \param path - Путь к каталогу для поиска
		//! \param filter - Параметры фильтрации файлов, по умолчанию отсутствуют
		//! \return Возвращает вектор из путей к найденным файлам 
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
		//! Перемещает указатель файла на заданную позицию
		[[nodiscard]] bool rewind(sizetype position);
		[[nodiscard]] bool rewind(FilePositions position);

		[[nodiscard]] void remove();
		[[nodiscard]] void remove(const std::string& path);
		
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

		//!
		//! \brief
		//! \return Размер файла в битах
		[[nodiscard]] sizetype fileSize() const noexcept;
	private:
		AbstractFileEngine* _engine = nullptr;
	};
} // namespace base::system