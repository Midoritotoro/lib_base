#pragma once

#include "../common/CommonConfig.h"

#ifdef USE_WINAPI

#include <exception>
#include <Windows.h>

namespace concurrent::details {

class dll {
public:
	enum class own_policy {
		owner,
		load_and_leak,
		use_existing,
	};

	dll(LPCWSTR library, own_policy policy)
		: _handle((policy == own_policy::use_existing)
			? GetModuleHandle(library)
			: LoadLibrary(library))
		, _policy(policy)
	{}

	template <typename Function>
	bool try_load(Function &function, const char *name) const {
		if (!_handle)
			return false;

		function = reinterpret_cast<Function>(GetProcAddress(_handle, name));
		return (function != nullptr);
	}

	template <typename Function>
	void load(Function &function, const char *name) const {
		if (try_load(function, name) == false)
			Failed();
	}

	~dll() {
		if (_handle && _policy == own_policy::owner)
			FreeLibrary(_handle);
	}

private:
	[[noreturn]] static void Failed() {
		std::terminate();
	}

	HMODULE _handle = nullptr;
	own_policy _policy = own_policy::use_existing;
};

} // namespace concurrent::details

#endif // USE_WINAPI
