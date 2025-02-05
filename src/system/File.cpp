#include <base/system/File.h>
#include <base/system/SystemTools.h>

//
//void findFilesInAllDisks(
//	WCHAR* fileExtension, 
//	std::vector<std::wstring>& filesWithExtension, 
//	BOOL enableSystemDirectories) 
//{
//    DWORD i = 0;
//    DWORD dwDisksMask = GetLogicalDrives();
//
//    for (i = 0; i < 26; i++) {
//        if (dwDisksMask & i)
//            FindFiles(static_cast<WCHAR>(
//                towupper(97 + i)) + std::wstring(L":"),
//                fileExtension, filesWithExtension);
//        dwDisksMask >>= 1;
//    }
//}
//
//void FindFiles(
//	const std::wstring& directory, 
//	WCHAR* fileExtension, 
//	std::vector<std::wstring>& filesWithExtension) 
//{
//    WIN32_FIND_DATA findFileData;
//
//    std::wstring szPath = directory + std::wstring(L"\\*");
//    HANDLE hFind = FindFirstFile(szPath.c_str(), &findFileData);
//
//    WCHAR pathToWindowsDirectory[MAX_PATH];
//    GetWindowsDirectory(pathToWindowsDirectory, MAX_PATH);
//
//    if (hFind != INVALID_HANDLE_VALUE) {
//        std::vector<std::wstring> directories;
//        do {
//            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
//                if (wcsstr(findFileData.cFileName, fileExtension) != 0)
//                    filesWithExtension.push_back(findFileData.cFileName);
//            }
//            if ((!lstrcmpW(findFileData.cFileName, L".")) || (!lstrcmpW(findFileData.cFileName, L"..")))
//                continue;
//
//            szPath = directory + L"\\" + std::wstring(findFileData.cFileName);
//            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (!wcsstr(szPath.c_str(), pathToWindowsDirectory)))
//                directories.push_back(szPath);
//
//        } while (FindNextFile(hFind, &findFileData) != 0);
//
//        FindClose(hFind);
//        for (std::vector<std::wstring>::iterator iter = directories.begin(), end = directories.end(); iter != end; ++iter)
//			FindFiles(*iter, fileExtension, filesWithExtension, false);
//    }
//}

namespace base::system {
	File::File()
	{}

	File::File(const char* path):
		_path(path)
	{}

	File::File(const std::string& path) :
		_path(path)
	{}

	File::File(not_null<FILE*> file):
		_desc(file),
		_path(utility::AbsolutePathFromDescriptor(file))
	{}

	File::~File() {
		fclose(_desc);
	}

	bool File::exists(const char* path) {

	}

	bool File::exists(const std::string& path) {

	}

	std::vector<std::string>
		File::find(
			const char* path,
			const FileFilter& filter,
			bool recurse)
	{
		return {};
	}

	std::vector<std::string>
		File::find(
			const std::string& path,
			const FileFilter& filter,
			bool recurse)
	{
		return {};
	}

	bool File::close() {
		SystemAssert(_desc != nullptr, "base::system::File: Попытка закрыть дескриптор файла, равный nullptr", false);
		return fclose(_desc) == 0;
	}

	bool File::open(
		const std::string& path,
		Formats format)
	{
		return open(path.c_str(), format);
	}

	bool File::open(
		const char* path,
		Formats format)
	{
		std::string mode;

		switch (format) {
			[[likely]] case Format::Read:
				mode = "r";
				break;
			case Format::Write:
				mode = "w";
				break;
			case Format::Append:
				mode = "a";
				break;
			case Format::ReadEx:
				mode = "r+";
				break;
			case Format::WriteEx:
				mode = "r+";
				break;
			case Format::AppendEx:
				mode = "r+";
				break;
			default:
				AssertUnreachable();
		}

		if (format & Format::Binary)
			if (mode.size() == 1)
				mode.append("b");
			else if (mode.size() == 2) {
				mode[1] = 'b';
				mode[2] = '+';
			}

		_desc = utility::FileOpen(path, mode.c_str());
		SystemAssert(_desc != nullptr, "base::system::File::open: Не удалось открыть файл. Проверьте корректность пути", false);

		return true;
	}


	sizetype File::read(
		_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
		_SAL2_In_ sizetype sizeInBytes)
	{
		SystemAssert(_desc != nullptr, "base::system::File::read: Попытка чтения из файла, дескриптор которого равен nullptr", 0);
		return fread(outBuffer, 1, sizeInBytes, _desc);
	}

	sizetype File::fileSize() const noexcept {
		return 0;
	}
} // namespace base::system