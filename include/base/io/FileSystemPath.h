#pragma once

#include <base/system/Platform.h>

namespace base::io {
	#if defined(OS_WIN)
		#include <base/io/WindowsFileSystemPath.h>
		using FileSystemPath = base::io::WindowsFileSystemPath;
	#elif defined(OS_MAC) || defined(OS_LINUX)
		#include <base/io/UnixFileSystemPath.h>
		using FileSystemPath = base::io::UnixFileSystemPath;
	#endif
} // namespace base::io
