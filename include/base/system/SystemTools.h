#pragma once 

#include <base/system/Platform.h>
#include <vector>


namespace base::system::utility {
	#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
		[[nodiscard]] int ConvertWCharToUnicode(
			char* buffer,
			size_t bufferlen,
			const wchar_t* input);

		[[nodiscard]] int ConvertUnicodeToWChar(
			wchar_t* buffer,
			size_t bufferlen,
			const char* input);
	#endif

	std::vector<std::string> GetAllProcesses();
} // namespace base::system::utility