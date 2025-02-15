#pragma once

#include <base/io/ByteArray.h>

namespace base::io {
	constexpr sizetype MaxByteArraySize = MaxAllocSize - sizeof(std::remove_pointer<ByteArray::DataPointer>::type) - 1;
	constexpr sizetype MaxStringSize = (MaxAllocSize - sizeof(std::remove_pointer<ByteArray::DataPointer>::type)) / 2 - 1;
} // namespace base::io
