#pragma once 

#include <base/system/Platform.h>
#include <base/utility/Flags.h>


namespace base::io {
    enum class FileOpenMode : uchar {
        Read = 0x01,
        Write = 0x02,
        Append = 0x04,
    };

    enum FilePosition : uchar {
        FileBegin = 0x01, // SEEK_SET
        FileEnd = 0x02    // SEEK_END
    };

    DECLARE_FLAGS(FileOpenModes, FileOpenMode);
    DECLARE_FLAGS_ENUM(FilePositions, FilePosition);

    struct FileFilter {
        std::string nameContains;
        sizetype minimumSize = 0; // In bytes
        sizetype maximumSize = 0; // In bytes
    };

    struct ReadResult {
        uchar* data = nullptr;
        sizetype sizeInBytes = 0;
    };
} // namespace base::io


#if defined(OS_MAC) || defined(OS_LINUX)
    #include <unistd.h>
    #include <fcntl.h> // Äëÿ fcntl
#elif defined(OS_WIN)
    #include <io.h> // Äëÿ _get_osfhandle
#endif

#include <limits>  // Äëÿ PATH_MAX

#if defined(LIB_BASE_IO_NO_FAILURE)
	#define IOAssert						AssertReturn
#else
	#define IOAssert(cond, mes, retval)		AssertLog(cond, mes)
#endif

