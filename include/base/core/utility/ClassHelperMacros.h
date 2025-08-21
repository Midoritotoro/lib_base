#pragma once 

#include <xutility>


#define DISABLE_COPY(Class) \
	Class(const Class&) = delete;\
	Class& operator=(const Class&) = delete;

#define DISABLE_COPY_MOVE(Class) \
	DISABLE_COPY(Class) \
	Class(Class&&) = delete; \
	Class &operator=(Class &&) = delete;

#define MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(Class) \
    Class &operator=(Class &&other) noexcept { \
        Class moved(std::move(other)); \
        swap(moved); \
        return *this; \
    }

#define MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(Class) \
    Class &operator=(Class &&other) noexcept { \
        swap(other); \
        return *this; \
    }
