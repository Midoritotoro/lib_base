#include <base/system/WindowsFileEngine.h>

#if defined(OS_WIN)

#include <base/system/WindowsSmartHandle.h>
#include <base/system/SystemTools.h>


namespace base::system {
	WindowsFileEngine::WindowsFileEngine()
	{}

	WindowsFileEngine::WindowsFileEngine(const std::string& path) :
		_path(path)
	{}

	WindowsFileEngine::WindowsFileEngine(
		not_null<FILE*> file,
		bool tryToExtractPathFromDescriptor)
	{
		
	}

	WindowsFileEngine::WindowsFileEngine(
		not_null<FILE*> file,
		const std::string& path)
	{

	}

	WindowsFileEngine::~WindowsFileEngine() {
		if (_desc != nullptr)
			fclose(_desc);
	}

	void WindowsFileEngine::setFileName(const std::string& path) {

	}

	void WindowsFileEngine::setFileDescriptor(not_null<FILE*> file) {

	}

	FILE* WindowsFileEngine::fileDescriptor() const noexcept {
		return _desc;
	}

	std::string WindowsFileEngine::path() const noexcept {
		return _path;
	}

	bool WindowsFileEngine::exists(const std::string& path) {

	}

	void WindowsFileEngine::find(
		const base_string& path,
		const FileFilter& filter,
		std::vector<base_string>& output,
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
				find(directories[i], filter, output, recurse);
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
} // namespace base::system

#endif