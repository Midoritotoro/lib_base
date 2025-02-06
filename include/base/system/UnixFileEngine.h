#pragma once

#if defined(OS_MAC) || defined(OS_LINUX)

#include <base/system/AbstractFileEngine.h>

namespace base::system {
	class UnixFileEngine final : public AbstractFileEngine {
	public:
		UnixFileEngine();
		UnixFileEngine(const std::string& path);

		UnixFileEngine(
			not_null<FILE*> file,
			bool tryToExtractPathFromDescriptor = false);

		UnixFileEngine(
			not_null<FILE*> file,
			const std::string& path);

		~UnixFileEngine() override;

		void setFileName(const std::string& path) override;
		void setFileDescriptor(not_null<FILE*> file) override;

		[[nodiscard]] FILE* fileDescriptor() const noexcept override;
		[[nodiscard]] std::string path() const noexcept override;

		[[nodiscard]] bool exists(const std::string& path) override;
		[[nodiscard]] std::vector<std::string>
			find(
				const std::string& path,
				const FileFilter& filter,
				bool recurse = true) override;

		[[nodiscard]] std::string absolutePathFromDescriptor(FILE* descriptor) override;
	private:
		FILE* _desc = nullptr;
		std::string _path = "";
	};
} // namespace base::system
#endif