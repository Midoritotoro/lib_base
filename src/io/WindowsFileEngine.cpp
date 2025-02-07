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
		return _isOpened;
	}

	FILE* WindowsFileEngine::fileDescriptor() const noexcept {
		return _desc;
	}

	std::string WindowsFileEngine::path() const noexcept {
		return _path;
	}

	bool WindowsFileEngine::exists(const std::string& path) {
		DWORD dwAttrib = GetFileAttributes(ConvertString(path).c_str());
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
						+ ConvertString(":"),
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

		WIN32_FIND_DATA findFileData;
		base_string szPath = path + ConvertString("\\*");

		WindowsSmartHandle hFind = FindFirstFile(szPath.c_str(), &findFileData);
		base_string pathToWindowsDirectory(MAX_PATH, 0);

		GetWindowsDirectory(&pathToWindowsDirectory[0], MAX_PATH);

		if (hFind != INVALID_HANDLE_VALUE) {
			std::vector<base_string> directories;

			do {
				if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false 
					&& filterNames 
					&& base_strstr(findFileData.cFileName,
						filter.nameContains.c_str()) != 0)
					output.push_back(findFileData.cFileName);
				
				if ((!lstrcmpW(findFileData.cFileName, L".")) || (!lstrcmpW(findFileData.cFileName, L"..")))
					continue;

				szPath = path + ConvertString("\\") + findFileData.cFileName;

				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
					&& (!base_strstr(
						szPath.c_str(), pathToWindowsDirectory.c_str())))
					directories.push_back(szPath);

			} while (FindNextFile(hFind, &findFileData) != 0);

			FindClose(hFind);

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
		if (_isOpened == false)
			return;

		_isOpened = false;

		if (_desc != nullptr)
			fclose(_desc);
	}

	bool WindowsFileEngine::open(
		const std::string& path,
		FileOpenModes mode)
	{

	}

	bool WindowsFileEngine::open(
		const std::string& path,
		const char* mode)
	{
		const auto openAlreadyOpened = (path == _path && _isOpened == true);
		AssertReturn(openAlreadyOpened != false, "base::io::WindowsFileEngine::open: Попытка открыть уже открытый файл. ", false);

		#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
			wchar_t wMode[64];
			wchar_t wFilename[1024];
			if (ConvertUnicodeToWChar(wFilename, ARRAY_SIZE(wFilename), path.c_str()) == 0)
				return false;

			if (ConvertUnicodeToWChar(wMode, ARRAY_SIZE(wFilename), mode) == 0)
				return false;

		#if defined(_MSC_VER) && _MSC_VER >= 1400
			if (0 != _wfopen_s(&_desc, wFilename, wMode))
						_desc = 0;
		#else
			_desc = _wfopen(wFilename, wMode);
		#endif

		#elif defined(_MSC_VER) && _MSC_VER >= 1400
			if (0 != fopen_s(&_desc, path.c_str(), mode))
				_desc = 0;
		#else
			_desc = fopen(path.c_str(), mode);
		#endif

		return (_desc != nullptr);
	}

	bool WindowsFileEngine::rename(const std::string& newFileName) {

	}

	bool WindowsFileEngine::rename(
		const std::string& oldFileName,
		const std::string& newFileName)
	{

	}

	bool WindowsFileEngine::rewind(sizetype position) {

	}

	bool WindowsFileEngine::rewind(FilePositions position) {

	}

	void WindowsFileEngine::remove() {

	}

	void WindowsFileEngine::remove(const std::string& path) {
		bool result = DeleteFile(ConvertString(path).c_str());

		IOAssert(_desc != nullptr, "base::io::WindowsFileEngine::read: Невозможно прочитать из файла. Дескриптор равен nullptr. ", 0);
	}

	sizetype WindowsFileEngine::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{
		IOAssert(_desc != nullptr, "base::io::WindowsFileEngine::read: Невозможно прочитать из файла. Дескриптор равен nullptr. ", 0);
		return fread(outBuffer, 1, sizeInBytes, _desc);
	}

	sizetype WindowsFileEngine::fileSize() const noexcept {
		if (_path.empty())
			return 0;
		
		LARGE_INTEGER file_size = { 0 };
		WIN32_FILE_ATTRIBUTE_DATA file_attr_data;

		if (GetFileAttributesEx(ConvertString(_path).c_str(), GetFileExInfoStandard, &file_attr_data)) {
			file_size.LowPart = file_attr_data.nFileSizeLow;
			file_size.HighPart = file_attr_data.nFileSizeHigh;
		}
		
		return std::max(file_size.QuadPart, LONGLONG(0));
	}

} // namespace base::io

#endif