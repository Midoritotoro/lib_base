#pragma once

#include <base/core/arch/Platform.h>

__BASE_IO_NAMESPACE_BEGIN

#if defined(OS_WIN)
	#include <base/core/io/WindowsFileSystemPath.h>
	using FileSystemPath = base::io::WindowsFileSystemPath;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/io/UnixFileSystemPath.h>
	using FileSystemPath = base::io::UnixFileSystemPath;
#endif

__BASE_IO_NAMESPACE_END
