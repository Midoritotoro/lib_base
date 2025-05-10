#pragma once

#include <base/core/arch/Platform.h>

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

        constexpr static NODISCARD bool IsLittleEndian();
        constexpr static NODISCARD bool IsBigEndian();

        static NODISCARD int GetCpuCount();
	};
} // namespace base::system