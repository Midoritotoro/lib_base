#include <base/system/WindowsFileEngine.h>

#if defined(OS_WIN)

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

	std::vector<std::string>
		WindowsFileEngine::find(
			const std::string& path,
			const FileFilter& filter,
			bool recurse)
	{

	}


	std::string WindowsFileEngine::absolutePathFromDescriptor(FILE* descriptor) {

	}


} // namespace base::system

#endif