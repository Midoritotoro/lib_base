#pragma once

#include <base/core/arch/Platform.h>

namespace base::system {
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

	class SystemInfo {
	public:
        enum Sizes {
            WordSize = (sizeof(void*) << 3)
        };

        constexpr static bool IsLittleEndian() noexcept {
            return (Endian::ByteOrder == Endian::LittleEndian);
        }

        constexpr static bool IsBigEndian() noexcept {
            return (Endian::ByteOrder == Endian::BigEndian);
        }


        static NODISCARD int GetCpuCount();
	};
} // namespace base::system