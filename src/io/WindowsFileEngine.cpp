#include <base/io/WindowsFileEngine.h>

#if defined(OS_WIN)

#include <base/io/WindowsSmartHandle.h>
#include <base/system/SystemTools.h>

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
		/*static const char* err = "base::system::AbsolutePathFromDescriptor: Не удается извлечь путь из нулевого дескриптора файла. ";
		SystemAssert(descriptor != nullptr, err, "");

		int fd = _fileno(descriptor);
		SystemAssert(fd != -1, err, "");

		WindowsSmartHandle handle = (HANDLE)_get_osfhandle(fd);
		SystemAssert(handle != INVALID_HANDLE_VALUE, err, "");

		std::vector<char> buffer(MAX_PATH);
		DWORD length = GetFinalPathNameByHandleA(
			handle, buffer.data(), MAX_PATH,
			FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

		SystemAssert(length != 0, err, "");

		if (length > MAX_PATH) {
			buffer.resize(length);
			length = GetFinalPathNameByHandleA(
				handle, buffer.data(), length, 
				FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

			SystemAssert(length != 0, err, "");
		}

		std::string wpath(buffer.data());
		SystemAssert(!wpath.empty(), err, "");

		int size_needed = WideCharToMultiByte(
			CP_UTF8, 0, &wpath[0], (int)wpath.size(),
			NULL, 0, NULL, NULL);

		std::string path(size_needed, 0);

		WideCharToMultiByte(
			CP_UTF8, 0, &wpath[0], (int)wpath.size(),
			&path[0], size_needed, NULL, NULL);

		return path;*/
		return "";
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

		IOAssert(false, "base::io::WindowsFileEngine::rename: Ошибка в WinApi при переименовании файла. ", false);

		return false;
	}

	bool WindowsFileEngine::rewind(int64 position) {
		IOAssert(_desc != nullptr, "base::io::WindowsFileEngine::rewind: Попытка переместить указатель файла, дескриптор которого равен nullptr. ", false);
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
		const char* mode)
	{
		FILE* file = fopen(path.c_str(), mode);
		IOAssert(file != nullptr, "base::io::WindowsFileEngine::write: Не удается открыыть файл. ", false);

		return (fwrite(inBuffer, 1, sizeInBytes, file) == sizeInBytes);
	}

	sizetype WindowsFileEngine::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{
		return fread(outBuffer, 1, sizeInBytes, _desc);
	}

	ReadResult WindowsFileEngine::readAll()
	{
		uchar* result = 0;
		SSE2_ALIGNAS(16) uchar buffer[1024] = { 0 };

		sizetype readed = 0;
		sizetype size = 0;

#if defined(LIB_BASE_ENABLE_sse2)
		__m128i r = _mm_set1_epi8(0);

		while ((readed = fread(buffer, 1, sizeof(buffer), _desc)) > 0) {
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
	while ((readed = fread(buffer, 1, sizeof(buffer), _desc)) > 0) {
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