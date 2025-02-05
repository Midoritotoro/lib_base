#pragma once 

#include <base/Types.h>
#include <string>


namespace base::system {
	class AbstractFileEngine {
	public:
		AbstractFileEngine();

		AbstractFileEngine(const char* path);
		AbstractFileEngine(const std::string& path);

		AbstractFileEngine(not_null<FILE*> file);

		~AbstractFileEngine();

		static [[nodiscard]] bool exists(const char* path);
		static [[nodiscard]] bool exists(const std::string& path);

		static [[nodiscard]] std::vector<std::string>
			find(
				const char* path,
				const FileFilter& filter);

		static [[nodiscard]] std::vector<std::string>
			find(
				const std::string& path,
				const FileFilter& filter);
	private:
		FILE* _desc = nullptr;
		
	}
} // namespace base::system