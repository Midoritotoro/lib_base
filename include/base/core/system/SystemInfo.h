#pragma once

#include <base/core/arch/Platform.h>

namespace base::system {
    enum Endian {
        BigEndian,
        LittleEndian
#  if base_byte_order == base_big_endian
        , ByteOrder = BigEndian
#  elif base_byte_order == base_little_endian
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


        static base_nodiscard int GetCpuCount();
	};
} // namespace base::system