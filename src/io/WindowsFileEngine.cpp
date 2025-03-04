#include <base/io/WindowsFileEngine.h>

#if defined(OS_WIN)

#include <base/io/WindowsSmartHandle.h>
#include <base/system/SystemTools.h>

#define WIN_EXTENDED_PATH_KEY			"\\?"
#define WIN_EXTENDED_PATH_KEY_SIZE		3

#define WIN_READ_BUFFER_SIZE			1024
#define WIN_MAX_READ_BUFFER_SIZE		(32 * 1024 * 1024)

__BASE_IO_NAMESPACE_BEGIN

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
	IOAssert(descriptor != nullptr, err, "");

	int fd = _fileno(descriptor);
	IOAssert(fd != -1, err, "");

	WindowsSmartHandle handle = (HANDLE)_get_osfhandle(fd);
	IOAssert(handle != INVALID_HANDLE_VALUE, err, "");

	std::vector<char> buffer(MAX_PATH);
	DWORD length = GetFinalPathNameByHandleA(
		handle.handle(), buffer.data(), MAX_PATH,
		FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

	IOAssert(length != 0, err, "");

	if (length > MAX_PATH) {
		buffer.resize(length);
		length = GetFinalPathNameByHandleA(
			handle.handle(), buffer.data(), length,
			FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

		IOAssert(length != 0, err, "");
	}

	std::string path(buffer.data());
	IOAssert(!path.empty(), err, "");

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
	int _mode = 0;

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
	if (path.empty())
		return false;

	_path = path;
	if (path.size() > MAX_PATH && path.substr(0, 3) != WIN_EXTENDED_PATH_KEY)
		expandPath(_path);

	DWORD creationFlags = 0;

	if (mode & GENERIC_READ && mode & ~GENERIC_WRITE && mode & ~FILE_APPEND_DATA)
		creationFlags = OPEN_EXISTING;
	
	else if (mode & GENERIC_WRITE || mode & FILE_APPEND_DATA)
		creationFlags = CREATE_ALWAYS;
	

	_handle = CreateFileA(
		_path.c_str(), mode, 0, nullptr,
		creationFlags, FILE_ATTRIBUTE_NORMAL,
		nullptr);

	IOAssert(_handle.isValid(), "base::WindowsFileEngine::open: Не удается открыть файл. ", false);
	return _handle.isValid();
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

	IOAssert(false, "base::io::WindowsFileEngine::rename: Ошибка при переименовании файла. ", false);
	return false;
}

bool WindowsFileEngine::rewind(int64 position) {
	IOAssert(_handle.isValid(), "base::io::WindowsFileEngine::rewind: Попытка переместить указатель файла, дескриптор которого равен nullptr. ", false);
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

		const auto notFoundError	 = (dwLastError & ERROR_FILE_NOT_FOUND);
		const auto accessDeniedError = (dwLastError & ERROR_ACCESS_DENIED);

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

	std::string correctPath = path;
	if (path.size() > MAX_PATH && path.substr(0, 3) != WIN_EXTENDED_PATH_KEY)
		expandPath(correctPath);

	DWORD creationFlags = CREATE_ALWAYS;

	WindowsSmartHandle _handle = CreateFileA(
		correctPath.c_str(), mode, 0, nullptr,
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

ReadResult WindowsFileEngine::read(sizetype sizeInBytes)
{
	IOAssert(_handle.isValid(), "base::WindowsFileEngine::readAll: Попытка чтения из файла с нулевым дескриптором. ", {});

	ReadResult result = {};

	read(ReadType::Value, &result, 
		std::min(fileSize(), sizeInBytes));

	return result;
}

ReadResult WindowsFileEngine::readAll()
{
	measureExecutionTime("WindowsFileEngine::readAll()")
	IOAssert(_handle.isValid(), "base::WindowsFileEngine::readAll: Попытка чтения из файла с нулевым дескриптором. ", {});

	ReadResult result = {};
	read(ReadType::All, &result);

	std::cout << result.sizeInBytes;
	
	return result;
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

sizetype WindowsFileEngine::fileSize() const noexcept {
	return fileSize(_path);
}

bool WindowsFileEngine::readSSE2(
	__m128i* outVector,
	uchar* tempOutBuffer,
	uchar* outBuffer,
	sizetype sizeInBytesRequiredForReading,
	sizetype* successfullyReadedBytes)
{
	const auto readResult = ReadFile(
		_handle.handle(), tempOutBuffer, 
		sizeInBytesRequiredForReading,
		reinterpret_cast<LPDWORD>(successfullyReadedBytes),
		nullptr);

	for (int i = 0; i < *successfullyReadedBytes; i += 16) {
		__m128i a = _mm_load_si128((const __m128i*)(tempOutBuffer + i));
		*outVector = _mm_add_epi8(a, *outVector);
	}

	memset(outBuffer, 0, sizeInBytesRequiredForReading);

	for (int i = 0; i < 16; ++i)
		tempOutBuffer += ((uchar*)outVector)[i];

	return (readResult != 0);
}

bool WindowsFileEngine::readSSSE3(
	__m256i* outVector,
	uchar* tempOutBuffer,
	uchar* outBuffer,
	sizetype sizeInBytesRequiredForReading,
	sizetype* successfullyReadedBytes)
{
	const auto readResult = ReadFile(
		_handle.handle(), tempOutBuffer,
		sizeInBytesRequiredForReading,
		reinterpret_cast<LPDWORD>(successfullyReadedBytes),
		nullptr);

	for (int i = 0; i < *successfullyReadedBytes; i += 32) {
		__m256i a = _mm256_load_si256((const __m256i*)(tempOutBuffer + i));
		*outVector = _mm256_add_epi8(a, *outVector);
	}

	memset(outBuffer, 0, sizeInBytesRequiredForReading);

	for (int i = 0; i < 32; ++i)
		tempOutBuffer += ((uchar*)outVector)[i];

	return (readResult != 0);
}

bool WindowsFileEngine::readSSE4_1(
	__m512i* outVector,
	uchar* tempOutBuffer,
	uchar* outBuffer,
	sizetype sizeInBytesRequiredForReading,
	sizetype* successfullyReadedBytes)
{
	const auto readResult = ReadFile(
		_handle.handle(), tempOutBuffer,
		sizeInBytesRequiredForReading,
		reinterpret_cast<LPDWORD>(successfullyReadedBytes),
		nullptr);

	for (int i = 0; i < *successfullyReadedBytes; i += 64) {
		__m512i a = _mm512_load_si512((const __m512i*)(tempOutBuffer + i));
		*outVector = _mm512_add_epi8(a, *outVector);
	}

	memset(tempOutBuffer, 0, sizeInBytesRequiredForReading);

	for (int i = 0; i < 64; ++i)
		outBuffer += ((uchar*)outVector)[i];

	return (readResult != 0);
}

bool WindowsFileEngine::readSSE4_2(
	__m512i* outVector,
	uchar* tempOutBuffer,
	uchar* outBuffer,
	sizetype sizeInBytesRequiredForReading,
	sizetype* successfullyReadedBytes)
{
	// В данном случае используются инструкции, идентичные для SSE4_1 и SSE4_2
	return readSSE4_1(
		outVector, tempOutBuffer,
		outBuffer, sizeInBytesRequiredForReading,
		successfullyReadedBytes);
}

bool WindowsFileEngine::readNoSimd(
	uchar* outBuffer,
	uchar* tempOutBuffer,
	sizetype sizeInBytesRequiredForReading,
	sizetype* successfullyReadedBytes) 
{
	const auto readResult = ReadFile(
		_handle.handle(), tempOutBuffer,
		sizeInBytesRequiredForReading, 
		reinterpret_cast<LPDWORD>(successfullyReadedBytes),
		nullptr);

	for (sizetype i = 0; i < *successfullyReadedBytes; ++i)
		outBuffer += tempOutBuffer[i];

	memset(tempOutBuffer, 0, sizeInBytesRequiredForReading);
	
	std::cout << "WindowsFileEngine::readNoSimd: " << readResult << " readed bytes: " 
		<< *successfullyReadedBytes << "handle value: " << (_handle.handle() != nullptr) 
		<< " sizeInBytesRequiredForReading: " << sizeInBytesRequiredForReading << " LastError: " << GetLastError() << '\n';

	return (readResult != 0);
}



bool WindowsFileEngine::read(
	ReadType type,
	ReadResult* outBuffer,
	sizetype sizeInBytes)
{
	auto fileSizeForRead = fileSize();

	sizetype readedBytes = 0;
	Fn<bool(sizetype, sizetype)> readContinueCondition = nullptr;

	switch (type) {
		case ReadType::Value:
			readContinueCondition = std::move([=](
				sizetype _readed, 
				sizetype _sizeInBytesForReading) -> bool 
			{
				return (_readed < fileSizeForRead);
			});
			break;

		case ReadType::All:
			readContinueCondition = std::move([=](
				sizetype _readed, 
				sizetype _sizeInBytesForReading) -> bool 
			{
				UNUSED(_readed);
				UNUSED(_sizeInBytesForReading);

				return true;
			});

			break;

		default:
			AssertUnreachable();
	}

	const sizetype sizeInBytesForReading = (sizeInBytes == 0) 
		? WIN_MAX_READ_BUFFER_SIZE 
		: sizeInBytes;

//#if LIB_BASE_ENABLE(sse4_2)
//	__m512i outVectorizedResult = _mm512_set1_epi8(0);
//	SIMD_ALIGNAS(BASE_SIMD_SSE4_2_ALIGNMENT)
//#elif LIB_BASE_ENABLE(sse4_1)
//	__m512i outVectorizedResult = _mm512_set1_epi8(0);
//	SIMD_ALIGNAS(BASE_SIMD_SSE4_1_ALIGNMENT)
//#elif LIB_BASE_ENABLE(ssse3)
//	__m256i outVectorizedResult = _mm256_set1_epi8(0);
//	SIMD_ALIGNAS(BASE_SIMD_SSSE3_ALIGNMENT)
//#elif LIB_BASE_ENABLE(sse2)
//	__m128i outVectorizedResult = _mm_set1_epi8(0);
//	SIMD_ALIGNAS(BASE_SIMD_SSE2_ALIGNMENT)
//#endif

	uchar* buffer = new uchar[sizeInBytesForReading];
	sizetype size = 0;

//#if LIB_BASE_ENABLE(sse4_2)
//	while (readSSE4_2(&outVectorizedResult, buffer,
//		outBuffer->data, sizeInBytesForReading, &readedBytes)
//		&& readContinueCondition())
// 	   availableBytesForRead -= readedBytes;
//#elif LIB_BASE_ENABLE(sse4_1)
//	while (readSSE4_1(&outVectorizedResult, buffer,
//		outBuffer->data, sizeInBytesForReading, &readedBytes)
//		&& readContinueCondition())
// 	   availableBytesForRead -= readedBytes;
//#elif LIB_BASE_ENABLE(ssse3)
//	while (readSSE3(&outVectorizedResult, buffer,
//		outBuffer->data, sizeInBytesForReading, &readedBytes)
//		&& readContinueCondition())
// 	   availableBytesForRead -= readedBytes;
//#elif LIB_BASE_ENABLE(sse2)
//	while (readSSE2(&outVectorizedResult, buffer,
//		outBuffer->data, sizeInBytesForReading, &readedBytes)
//		&& readContinueCondition())
// 	   availableBytesForRead -= readedBytes;
//#else

	sizetype av = fileSizeForRead;
	while (readNoSimd(
		outBuffer->data, buffer, sizeInBytesForReading,
		&readedBytes) && readContinueCondition(readedBytes, sizeInBytesForReading)) {
		av -= sizeInBytesForReading;
		std::cout << "fs:  " << fileSizeForRead << " readed: " << readedBytes << " av: " << av << '\n';
	}
//#endif


	return true;
}

void WindowsFileEngine::expandPath(std::string& path) {
	path = WIN_EXTENDED_PATH_KEY + path;
}

__BASE_IO_NAMESPACE_END

#endif