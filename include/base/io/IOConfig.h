#pragma once 

#include <base/system/Platform.h>
#include <base/utility/Flags.h>


namespace base::io {
    enum class FileOpenMode : uchar {
        Read = 0x01,    // "r"
        Write = 0x02,   // "w"
        Append = 0x04,  // "a"
        ReadEx = 0x10,  // "r+"
        WriteEx = 0x20, // "w+"
        AppendEx = 0x40 // "a+"
#if defined(OS_WIN)
        ,
        // Так как в cstdlib fopen() флаг "b" игнорируется в системах POSIX,
        // то и использовать его там смысла нет. В Windows флаг "b" отключает обработку '\n' и '\x1A'.
        Binary = 0x80 // "b"
#endif
    };

    enum FilePosition : uchar {
        FileBegin = 0x01, // SEEK_SET
        FileEnd = 0x02    // SEEK_END
    };

    DECLARE_FLAGS(FileOpenModes, FileOpenMode);
    DECLARE_FLAGS_ENUM(FilePositions, FilePosition);

    struct FileFilter {
        std::string nameContains;
        sizetype minimumSize = 0;
        sizetype maximumSize = 0;
    };

    struct ReadResult {
        uchar* data = nullptr;
        sizetype sizeInBytes = 0;
    };
} // namespace base::io


#if defined(OS_MAC) || defined(OS_LINUX)
    #include <unistd.h>
    #include <fcntl.h> // Для fcntl
#elif defined(OS_WIN)
    #include <io.h> // Для _get_osfhandle
#endif

#include <limits>  // Для PATH_MAX

#if defined(LIB_BASE_IO_NO_FAILURE)
	#define IOAssert						AssertReturn
#else
	#define IOAssert(cond, mes, retval)		AssertLog(cond, mes)
#endif

