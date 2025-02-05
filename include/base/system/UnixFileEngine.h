#pragma once

#if defined(OS_MAC) || defined(OS_LINUX)

#include <base/system/AbstractFileEngine.h>

namespace base::system {
	class UnixFileEngine final : public AbstractFileEngine {
	public:
		~UnixFileEngine() override;

		[[nodiscard]] bool exists(const std::string& path) override;
		[[nodiscard]] std::vector<std::string>
			find(
				const std::string& path,
				const FileFilter& filter,
				bool recurse = true) override;


	};
} // namespace base::system
#endif