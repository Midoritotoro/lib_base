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
		const std::string& path,
		const FileFilter& filter,
		std::vector<std::string>& output,
		bool recurse)
	{
		WIN32_FIND_DATA findFileData;

		std::string szPath = path + std::string("\\*");

		HANDLE hFind = FindFirstFile(szPath.c_str(), &findFileData);
		WCHAR pathToWindowsDirectory[MAX_PATH];
		GetWindowsDirectory(pathToWindowsDirectory, MAX_PATH);

		if (hFind != INVALID_HANDLE_VALUE) {
			std::vector<std::string> directories;
			do {
				if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					if (wcsstr(findFileData.cFileName, fileExtension) != 0)
						output.push_back(findFileData.cFileName);
				}
				if ((!lstrcmpW(findFileData.cFileName, L".")) || (!lstrcmpW(findFileData.cFileName, L"..")))
					continue;
				
				const size_t strSize = ARRAY_SIZE(findFileData.cFileName);
				std::string cFileName(strSize, 0);

				utility::ConvertWCharToUnicode(&cFileName[0], strSize, findFileData.cFileName);
				szPath = path + L"\\" + std::string(cFileName);
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (!wcsstr(szPath.c_str(), pathToWindowsDirectory)))
					directories.push_back(szPath);

			} while (FindNextFile(hFind, &findFileData) != 0);

			FindClose(hFind);
			for (std::vector<std::wstring>::iterator iter = directories.begin(), end = directories.end(); iter != end; ++iter)
				find(*iter, filter, output, recurse);
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
		DWORD length = GetFinalPathNameByHandleW(
			handle, buffer.data(), MAX_PATH,
			FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

		SystemAssert(length != 0, err, "");

		if (length > MAX_PATH) {
			buffer.resize(length);
			length = GetFinalPathNameByHandleW(
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