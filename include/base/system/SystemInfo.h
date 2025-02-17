#pragma once

#include <base/system/Platform.h>

namespace base::system {
	class SystemInfo {
	public:
        enum Sizes {
            WordSize = (sizeof(void*) << 3)
        };

        enum Endian {
            BigEndian,
            LittleEndian
#  if BYTE_ORDER == BIG_ENDIAN
            , ByteOrder = BigEndian
#  elif BYTE_ORDER == LITTLE_ENDIAN
            , ByteOrder = LittleEndian
#  else
#    error "Undefined byte order"
#  endif
        };

        static [[nodiscard]] int GetCpuCount();
	};
} // namespace base::system