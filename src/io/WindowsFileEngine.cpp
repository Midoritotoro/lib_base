#include <base/io/WindowsFileEngine.h>

#if defined(OS_WIN)

#include <base/io/WindowsSmartHandle.h>
#include <base/system/SystemTools.h>

#ifdef UNICODE
#endif 
#if defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	std::string GetErrorMessage(DWORD errorCode) {
		LPWSTR messageBuffer = nullptr;
		size_t size = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&messageBuffer,
			0,
			NULL);

		std::wstring messageW;
		if (size > 0) {
			messageW = std::wstring(messageBuffer, size);
			LocalFree(messageBuffer);
		}
		else {
			messageW = L"Error code No system message available";
		}

		if (messageW.empty()) 
			return "";

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &messageW[0], (int)messageW.size(), NULL, 0, NULL, NULL);
		std::string message(size_needed, 0);

		WideCharToMultiByte(CP_UTF8, 0, &messageW[0], (int)messageW.size(), &message[0], size_needed, NULL, NULL);

		return message;
	}
#else
	std::string GetErrorMessage(DWORD errorCode) {
		char* messageBuffer = nullptr;
		size_t size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&messageBuffer,
			0,
			NULL);

		std::string message;
		if (size > 0) {
			message = std::string(messageBuffer, size);
			delete[] messageBuffer;
		}
		else {
			message = "Error code No system message available";
		}

		if (message.empty())
			return "";

		return message;
	}
#endif

namespace base::io {
	WindowsFileEngine::WindowsFileEngine()
	{}

	WindowsFileEngine::WindowsFileEngine(const std::string& path) :
		_path(path)
	{}

	WindowsFileEngine::WindowsFileEngine(
		not_null<FILE*> file,
		bool tryToExtractPathFromDescriptor
	):
		_desc(file)
	{
		if (tryToExtractPathFromDescriptor)
			_path = absolutePathFromDescriptor(_desc);
	}

	WindowsFileEngine::WindowsFileEngine(
		not_null<FILE*> file,
		const std::string& path
	):
		_desc(file),
		_path(path)
	{}

	WindowsFileEngine::~WindowsFileEngine() {
		close();
	}

	void WindowsFileEngine::setFileName(const std::string& path) {
		_path = path;
	}

	void WindowsFileEngine::setFileDescriptor(not_null<FILE*> file) {
		_desc = file;
	}

	bool WindowsFileEngine::isOpened() const noexcept {
		// Так как fclose() удаляет объект FILE*, 
		// то он открыт только в случае ненулевого значения указателя
		return (_desc != nullptr);
	}

	FILE* WindowsFileEngine::fileDescriptor() const noexcept {
		return _desc;
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
		std::vector<base_string>& output,
		const FileFilter& filter,
		bool recurse)
	{
		DWORD dwDisksMask = GetLogicalDrives();

		for (DWORD i = 0; i < ENGLISH_ALPHABET_SIZE; ++i) {
			if (dwDisksMask & i)
				find(
					static_cast<base_char>(base_toupper(ENGLISH_ALPHABET_ASCII_START + i))
						+ base_string(":"),
					output, filter, recurse);

			dwDisksMask >>= 1;
		}
	}

	void WindowsFileEngine::find(
		const base_string& path,
		std::vector<base_string>& output,
		const FileFilter& filter,
		bool recurse)
	{
		const auto filterNames	= (filter.nameContains.empty() == false);
		const auto filterSize	= (filter.minimumSize > 0);

		WIN32_FIND_DATAA findFileData;
		base_string szPath = path + base_string("\\*");

		WindowsSmartHandle hFind = FindFirstFileA(szPath.c_str(), &findFileData);
		hFind.setDeleteCallback(FindClose);

		base_string pathToWindowsDirectory(MAX_PATH, 0);

		GetWindowsDirectoryA(&pathToWindowsDirectory[0], MAX_PATH);

		if (hFind != INVALID_HANDLE_VALUE) {
			std::vector<base_string> directories;

			do {
				if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false 
					&& filterNames 
					&& base_strstr(findFileData.cFileName,
						filter.nameContains.c_str()) != 0)
					output.push_back(findFileData.cFileName);
				
				if ((!base_strcmp(findFileData.cFileName, ".")) || (!base_strcmp(findFileData.cFileName, "..")))
					continue;

				szPath = path + base_string("\\") + findFileData.cFileName;

				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
					&& (!base_strstr(
						szPath.c_str(), pathToWindowsDirectory.c_str())))
					directories.push_back(szPath);

			} while (FindNextFileA(hFind, &findFileData) != 0);

			for (int i = 0; i < directories.size(); ++i)
				find(directories[i], output, filter, recurse);
		}
	}

	std::string WindowsFileEngine::absolutePathFromDescriptor(FILE* descriptor) {
		static const char* err = "base::system::AbsolutePathFromDescriptor: Не удается извлечь путь из нулевого дескриптора файла. ";
		SystemAssert(descriptor != nullptr, err, "");

		int fd = _fileno(descriptor);
		SystemAssert(fd != -1, err, "");

		WindowsSmartHandle handle = (HANDLE)_get_osfhandle(fd);
		SystemAssert(handle != INVALID_HANDLE_VALUE, err, "");

		std::vector<wchar_t> buffer(MAX_PATH);
		DWORD length = GetFinalPathNameByHandle(
			handle, buffer.data(), MAX_PATH,
			FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

		SystemAssert(length != 0, err, "");

		if (length > MAX_PATH) {
			buffer.resize(length);
			length = GetFinalPathNameByHandle(
				handle, buffer.data(), length, 
				FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

			SystemAssert(length != 0, err, "");
		}

		std::wstring wpath(buffer.data());
		SystemAssert(!wpath.empty(), err, "");

		int size_needed = WideCharToMultiByte(
			CP_UTF8, 0, &wpath[0], (int)wpath.size(),
			NULL, 0, NULL, NULL);

		std::string path(size_needed, 0);

		WideCharToMultiByte(
			CP_UTF8, 0, &wpath[0], (int)wpath.size(),
			&path[0], size_needed, NULL, NULL);

		return path;
	}

	void WindowsFileEngine::close() {
		if (isOpened())
			fclose(_desc);
	}

	bool WindowsFileEngine::open(
		const std::string& path,
		FileOpenModes mode)
	{
		std::string modeStr;

		if (mode & FileOpenMode::Read)
			modeStr = "r";

		else if (mode & FileOpenMode::Write)
			modeStr = "w";

		else if (mode & FileOpenMode::Append)
			modeStr = "a";

		else if (mode & FileOpenMode::ReadEx)
			modeStr = "r+";

		else if (mode & FileOpenMode::WriteEx)
			modeStr = "r+";

		else if (mode & FileOpenMode::AppendEx)
			modeStr = "r+";

		if (mode & FileOpenMode::Binary)
			if (modeStr.size() == 1)
				modeStr.append("b");
			else if (modeStr.size() == 2) {
				modeStr[1] = 'b';
				modeStr[2] = '+';
			}

		return open(path, modeStr);
	}

	bool WindowsFileEngine::open(
		const std::string& path,
		const std::string& mode)
	{
		if (path.empty() || mode.empty())
			return false;

		/*const auto openAlreadyOpened = (path == _path && isOpened());
		AssertReturn(openAlreadyOpened != false, "base::io::WindowsFileEngine::open: Попытка открыть уже открытый файл. ", false);*/

		#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
			wchar_t wMode[64];
			wchar_t wFilename[1024];

			if (ConvertUnicodeToWChar(wFilename, ARRAY_SIZE(wFilename), path.c_str()) == 0)
				return false;

			if (ConvertUnicodeToWChar(wMode, ARRAY_SIZE(wFilename), mode.c_str()) == 0)
				return false;

		#if defined(_MSC_VER) && _MSC_VER >= 1400
			if (0 != _wfopen_s(&_desc, wFilename, wMode))
						_desc = 0;
		#else
			_desc = _wfopen(wFilename, wMode);
		#endif

		#elif defined(_MSC_VER) && _MSC_VER >= 1400
			if (0 != fopen_s(&_desc, path.c_str(), mode.c_str()))
				_desc = 0;
		#else
			_desc = fopen(path.c_str(), mode.c_str());
		#endif

		return (_desc != nullptr);
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

		IOAssert(false, std::string("base::io::WindowsFileEngine::rename: Ошибка в WinApi при переименовании файла: " 
			+ GetErrorMessage(GetLastError())).c_str(), false);

		return false; // Чтобы не было предупреждений
	}

	bool WindowsFileEngine::rewind(int64 position) {
		IOAssert(_desc != nullptr, "base::io::WindowsFileEngine::rewind: Попытка переместить указатель файла, дескриптор которого равен nullptr", false);
		return (fseek(_desc, position, SEEK_CUR) == 0);
	}

	bool WindowsFileEngine::rewind(FilePositions position) {
		switch (position) {
			case FilePosition::FileBegin:
				return (fseek(_desc, SEEK_SET, SEEK_CUR) == 0);
			case FilePosition::FileEnd:
				return (fseek(_desc, SEEK_END, SEEK_CUR) == 0);
			default:
				AssertUnreachable();
		}

		return false; // Чтобы не было предупреждений
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

	sizetype WindowsFileEngine::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{
		// IOAssert(_desc != nullptr, "base::io::WindowsFileEngine::read: Невозможно прочитать из файла. Дескриптор равен nullptr. ", 0);
		return fread(outBuffer, 1, sizeInBytes, _desc);
	}

	ReadResult WindowsFileEngine::readAll()
	{
		measureExecutionTime("WindowsFileEngine::readAll()");
		uchar* result = nullptr;
		uchar buffer[1024];

		sizetype size = 0;

		// IOAssert(_desc != nullptr, "base::io::WindowsFileEngine::read: Невозможно прочитать из файла. Дескриптор равен nullptr. ", 0);

		while (sizetype readed = fread(buffer, 1, sizeof(buffer), _desc)) {
			size += readed;

			for (int i = 0; i < readed; ++i)
				result += buffer[i];
		}

		return {
			.data = buffer,
			.sizeInBytes = size / 1024
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