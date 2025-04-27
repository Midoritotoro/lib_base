#pragma once 

#include <base/core/arch/Platform.h>

__BASE_NAMESPACE_BEGIN

struct MinMaxInt8Bit {
    int8 minimum = 0;
    int8 maximum = 0;
};

struct MinMaxUInt8Bit {
    uint8 minimum = 0;
    uint8 maximum = 0;
};

struct MinMaxInt16Bit {
    int16 minimum = 0;
    int16 maximum = 0;
};

struct MinMaxUInt16Bit {
    uint16 minimum = 0;
    uint16 maximum = 0;
};

struct MinMaxInt32Bit {
    int32 minimum = 0;
    int32 maximum = 0;
};

struct MinMaxUInt32Bit {
    uint32 minimum = 0;
    uint32 maximum = 0;
};

struct MinMaxInt64Bit {
    int64 minimum = 0;
    int64 maximum = 0;
};

struct MinMaxUInt64Bit {
    uint64 minimum = 0;
    uint64 maximum = 0;
};

struct MinMaxFloat {
    float minimum = 0;
    float maximum = 0;
};

struct MinMaxDouble {
    double minimum = 0;
    double maximum = 0;
};

__BASE_NAMESPACE_END
