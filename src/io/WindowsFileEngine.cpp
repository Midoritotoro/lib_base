#include <base/io/WindowsFileEngine.h>

#if defined(OS_WIN)

#include <base/io/WindowsSmartHandle.h>
#include <base/system/SystemTools.h>

#define WIN_EXTENDED_PATH_KEY "\\?"
#define WIN_EXTENDED_PATH_KEY_SIZE 3

namespace base::io {
	WindowsFileEngine::WindowsFileEngine()
	{}

	WindowsFileEngine::WindowsFileEngine(const std::string& path) :
		_path(path)
	{}

	WindowsFileEngine::~WindowsFileEngine() {
		close();
	}

	void WindowsFileEngine::setFileName(const std::string& path) {
		_path = path;
	}

	bool WindowsFileEngine::isOpened() const noexcept {
		return (_handle.handle() != nullptr);
	}

	std::string WindowsFileEngine::path() const noexcept {
		return _path;
	}

	bool WindowsFileEngine::exists(const std::string& path) {
		DWORD dwAttrib = GetFileAttributesA(path.c_str());
		return 
			(dwAttrib != INVALID_FILE_ATTRIBUTES &&
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	void WindowsFileEngine::find(
		std::vector<std::string>& output,
		const FileFilter& filter,
		bool recurse)
	{
		DWORD dwDisksMask = GetLogicalDrives();

		for (DWORD i = 0; i < ENGLISH_ALPHABET_SIZE; ++i) {
			if (dwDisksMask & i)
				find(
					static_cast<char>(toupper(ENGLISH_ALPHABET_ASCII_START + i))
						+ std::string(":"),
					output, filter, recurse);

			dwDisksMask >>= 1;
		}
	}

	void WindowsFileEngine::find(
		const std::string& path,
		std::vector<std::string>& output,
		const FileFilter& filter,
		bool recurse)
	{
		const auto filterNames		 = (filter.nameContains.empty() == false);
		const auto filterSize		 = (filter.minimumSize > 0 || filter.maximumSize > 0);

		const auto useFilter	     = (filterNames || filterSize);

		const auto filterMaximumSize = (filterSize && filter.maximumSize <= 0 
			? MAXLONGLONG 
			: filter.maximumSize);

		WIN32_FIND_DATAA findFileData;
		std::string szPath = path + std::string("\\*");

		WindowsSmartHandle hFind = FindFirstFileA(szPath.c_str(), &findFileData);

		hFind.setAutoDelete(true);
		hFind.setDeleteCallback(FindClose);

		if (hFind == INVALID_HANDLE_VALUE)
			return;

		std::vector<std::string> directories;

		do {
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				directories.push_back(szPath);
				continue;
			}

			if (useFilter) {
				if (filterNames && filterSize) {
					LARGE_INTEGER fileSize = { 0 };

					fileSize.LowPart = findFileData.nFileSizeLow;
					fileSize.HighPart = findFileData.nFileSizeHigh;

					if (strstr(findFileData.cFileName, filter.nameContains.c_str()) != 0
						&& fileSize.QuadPart > filter.minimumSize
						&& fileSize.QuadPart < filterMaximumSize)
						output.push_back(findFileData.cFileName);
				}

				else if (filterNames) {
					if (strstr(findFileData.cFileName, filter.nameContains.c_str()) != 0)
						output.push_back(findFileData.cFileName);
				}
				else {
					LARGE_INTEGER fileSize = { 0 };

					fileSize.LowPart = findFileData.nFileSizeLow;
					fileSize.HighPart = findFileData.nFileSizeHigh;

					if (fileSize.QuadPart > filter.minimumSize
						&& fileSize.QuadPart < filterMaximumSize)
						output.push_back(findFileData.cFileName);
				}
			}
				
			if ((!strcmp(findFileData.cFileName, ".")) || (!strcmp(findFileData.cFileName, "..")))
				continue;

			szPath = path + std::string("\\") + findFileData.cFileName;

		} while (FindNextFileA(hFind.handle(), &findFileData) != 0);

		for (int i = 0; i < directories.size(); ++i)
			find(directories[i], output, filter, recurse);
		
	}

	std::string WindowsFileEngine::absolutePathFromDescriptor(FILE* descriptor) {
		static const char* err = "base::system::AbsolutePathFromDescriptor: Не удается извлечь путь из нулевого дескриптора файла. ";
		SystemAssert(descriptor != nullptr, err, "");

		int fd = _fileno(descriptor);
		SystemAssert(fd != -1, err, "");

		WindowsSmartHandle handle = (HANDLE)_get_osfhandle(fd);
		SystemAssert(handle != INVALID_HANDLE_VALUE, err, "");

		std::vector<char> buffer(MAX_PATH);
		DWORD length = GetFinalPathNameByHandleA(
			handle.handle(), buffer.data(), MAX_PATH,
			FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

		SystemAssert(length != 0, err, "");

		if (length > MAX_PATH) {
			buffer.resize(length);
			length = GetFinalPathNameByHandleA(
				handle.handle(), buffer.data(), length,
				FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

			SystemAssert(length != 0, err, "");
		}

		std::string path(buffer.data());
		SystemAssert(!path.empty(), err, "");

		return path;
	}

	void WindowsFileEngine::close() {
		if (isOpened())
			_handle.forceDelete();
	}

	bool WindowsFileEngine::open(
		const std::string& path,
		FileOpenModes mode)
	{
		int _mode;

		if (mode & FileOpenMode::Read)
			_mode |= GENERIC_READ;

		else if (mode & FileOpenMode::Write)
			_mode |= GENERIC_WRITE;

		else if (mode & FileOpenMode::Append)
			_mode = FILE_APPEND_DATA;

		else
			AssertUnreachable();

		return open(path, _mode);
	}

	bool WindowsFileEngine::open(
		const std::string& path,
		int mode)
	{
		measureExecutionTime("WindowsFileEngine::open")

		if (path.empty())
			return false;

		char* correctExtendedPath = nullptr;

		if (path.substr(0, 3) != WIN_EXTENDED_PATH_KEY) {
			correctExtendedPath = new char[path.size() + WIN_EXTENDED_PATH_KEY_SIZE];

			strcpy(correctExtendedPath, WIN_EXTENDED_PATH_KEY);
			strcpy(correctExtendedPath + WIN_EXTENDED_PATH_KEY_SIZE, path.c_str());
		}

		correctExtendedPath = new char[path.size()];
		strcpy(correctExtendedPath, path.c_str());

		DWORD creationFlags = 0;

		if (mode & GENERIC_READ && mode & ~GENERIC_WRITE && mode &~ FILE_APPEND_DATA)
			creationFlags = OPEN_EXISTING;

		else if (mode & GENERIC_WRITE || mode & FILE_APPEND_DATA)
			creationFlags = CREATE_ALWAYS;

		_handle = CreateFileA(
			correctExtendedPath, mode, 0, nullptr,
			creationFlags, FILE_ATTRIBUTE_NORMAL,
			nullptr);

		IOAssert(_handle.handle() != nullptr, "base::WindowsFileEngine::open: Не удается открыть файл. ", false);

		return (_handle.handle() != nullptr);
	}

	bool WindowsFileEngine::rename(const std::string& newFileName) {
		if (_path.empty())
			return false;

		return rename(_path, newFileName);
	}

	bool WindowsFileEngine::rename(
		const std::string& oldFileName,
		const std::string& newFileName)
	{
		if (MoveFileA(oldFileName.c_str(), newFileName.c_str()) != 0)
			return true;

		IOAssert(false, "base::io::WindowsFileEngine::rename: Ошибка в WinApi при переименовании файла. ", false);

		return false;
	}

	bool WindowsFileEngine::rewind(int64 position) {
		IOAssert(_handle.handle() != nullptr, "base::io::WindowsFileEngine::rewind: Попытка переместить указатель файла, дескриптор которого равен nullptr. ", false);
		return (SetFilePointer(
			_handle.handle(), position, nullptr, FILE_CURRENT)
			!= INVALID_SET_FILE_POINTER);
	}

	bool WindowsFileEngine::rewind(FilePositions position) {
		switch (position) {

			case FilePosition::FileBegin:
				return (SetFilePointer(
					_handle.handle(), 0, nullptr, FILE_BEGIN)
					!= INVALID_SET_FILE_POINTER);

			case FilePosition::FileEnd:
				return (SetFilePointer(
					_handle.handle(), 0, nullptr, FILE_END)
					!= INVALID_SET_FILE_POINTER);

			default:
				AssertUnreachable();
		}

		return false;
	}

	void WindowsFileEngine::remove() {
		remove(_path);
	}

	void WindowsFileEngine::remove(const std::string& path) {
		if (path.empty())
			return;

		if (DeleteFileA(path.c_str()) == false) {
			DWORD dwLastError = GetLastError();

			const bool notFoundError	 = (dwLastError & ERROR_FILE_NOT_FOUND);
			const bool accessDeniedError = (dwLastError & ERROR_ACCESS_DENIED);

			IOAssert(notFoundError, "base::io::WindowsFileEngine::remove: Невозможно удалить несуществующий файл. ", (void)0);
			IOAssert(accessDeniedError, "base::io::WindowsFileEngine::remove: Недостаточно прав для удаления файла. ", (void)0);
		}
	}

	bool WindowsFileEngine::write(
		const std::string& path,
		void* inBuffer,
		sizetype sizeInBytes,
		FileOpenModes mode)
	{
		if (path.empty())
			return false;

		char* correctExtendedPath = nullptr;

		if (path.substr(0, 3) != WIN_EXTENDED_PATH_KEY) {
			correctExtendedPath = new char[path.size() + WIN_EXTENDED_PATH_KEY_SIZE];

			strcpy(correctExtendedPath, WIN_EXTENDED_PATH_KEY);
			strcpy(correctExtendedPath + WIN_EXTENDED_PATH_KEY_SIZE, path.c_str());
		}

		correctExtendedPath = new char[path.size()];
		strcpy(correctExtendedPath, path.c_str());

		DWORD creationFlags = 0;

		if (mode & FileOpenMode::Write || mode & FileOpenMode::Append)
			creationFlags = CREATE_ALWAYS;

		WindowsSmartHandle _handle = CreateFileA(
			correctExtendedPath, mode, 0, nullptr,
			creationFlags, FILE_ATTRIBUTE_NORMAL,
			nullptr);

		IOAssert(_handle.handle() != nullptr, "base::io::WindowsFileEngine::write: Не удается открыыть файл. ", false);

		_handle.setAutoDelete(true);
		_handle.setDeleteCallback(CloseHandle);

		DWORD numberOfWrittenBytes = 0;
		const auto success = WriteFile(
			_handle.handle(), inBuffer, 
			sizeInBytes, &numberOfWrittenBytes, nullptr);

		return ((success != 0) && (numberOfWrittenBytes == sizeInBytes));
	}

	sizetype WindowsFileEngine::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{
		DWORD readedBytes = 0;
		const auto readResult = ReadFile(
			_handle.handle(), outBuffer, 
			sizeInBytes, &readedBytes, nullptr);

		return (readResult != 0);
	}

	ReadResult WindowsFileEngine::readAll()
	{
		measureExecutionTime("WindowsFileEngine::readAll()")
		IOAssert(_handle.handle() != nullptr, "base::WindowsFileEngine::readAll: Попытка чтения из файла с нулевым дескриптором. ", {});

		uchar* result = 0;

		SIMD_ALIGNAS(
#if LIB_BASE_ENABLE(sse4_2)
		BASE_SIMD_SSE4_2_ALIGNMENT
#elif LIB_BASE_ENABLE(sse4_1)
		BASE_SIMD_SSE4_1_ALIGNMENT
#elif LIB_BASE_ENABLE(ssse3)
		BASE_SIMD_SSSE3_ALIGNMENT
#elif LIB_BASE_ENABLE(sse2)
		BASE_SIMD_SSE2_ALIGNMENT
#endif
		) uchar buffer[1024] = { 0 };

		DWORD readed = 0;
		sizetype size = 0;

#if defined(LIB_BASE_ENABLE_sse2)
		__m128i r = _mm_set1_epi8(0);

		while ((ReadFile(
			_handle.handle(), buffer, sizeof(buffer),
			&readed, nullptr)) != 0)
		{
			size += readed;

			for (int i = 0; i < readed; i += 16) {
				__m128i a = _mm_load_si128((const __m128i*)(buffer + i));
				r = _mm_add_epi8(a, r);
			}

			memset(buffer, 0, sizeof(buffer));
		}

		for (int i = 0; i < 16; ++i)
			result += ((unsigned char*)&r)[i];
#else
		while ((ReadFile(
			_handle.handle(), buffer, 
			sizeof(buffer), &readed, nullptr)) != 0) 
		{
			size += readed;

			for (int i = 0; i < readed; ++i)
				result += buffer[i];
		}
#endif
		return {
			.data = result,
			.sizeInBytes = size
		};
	}

	sizetype WindowsFileEngine::fileSize(const std::string& path) {
		if (path.empty())
			return 0;
		
		LARGE_INTEGER fileSize = { 0 };
		WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;

		if (GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &fileAttributeData)) {
			fileSize.LowPart = fileAttributeData.nFileSizeLow;
			fileSize.HighPart = fileAttributeData.nFileSizeHigh;
		}
		
		return std::max(fileSize.QuadPart, LONGLONG(0));
	}

} // namespace base::io

#endif