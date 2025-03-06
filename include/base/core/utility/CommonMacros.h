#pragma once

#include <cstdio>
#include <base/core/arch/CompilerDetection.h>

#include <base/core/utility/MacrosOverload.h>

#include <gsl/gsl>
#include <base/core/system/Time.h>

#include <iostream>

#define measureExecutionTime_0() \
	const auto ms = base::Time::now();						\
	const auto timer = gsl::finally([&]() {					\
	std::cout << FUNC_INFO << " completed for: "			\
			<< base::Time::now() - ms << " ms" << '\n'; });

#define measureExecutionTime_1(name)						\
	const auto ms = base::Time::now();						\
	const auto timer = gsl::finally([&]() {					\
	std::cout << name << " completed for: "					\
		<< base::Time::now() - ms << " ms" << '\n'; });

#define measureExecutionTime(...)				\
    BASE_MACRO_OVERLOAD_COMPAT_BLOCK(			\
		BASE_MACRO_OVERLOAD_MACRO_CHOOSER(		\
			measureExecutionTime, __VA_ARGS__)  \
				(__VA_ARGS__))

//!
//! \brief
//! ����� ������ �� ������� ������� ��������� ���������� ����������� �� ���������� ����� ����� ���� ����� � value
//! 
#define measureExecutionTimeToValue(value) \
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([&]() mutable { \
		value = base::Time::now() - ms; });

//!
//! \brief
//! ���������, �������� �� ptr � ��������� member ������ ��������� ���� type
//! \param ptr - ��������� �� ���� ��������� (���� member).
//! \param type - ��� ���������, ������� �������� member.
//! \param member - ��� ����� ���������, �� ������� ��������� ptr.
//! 
#define container_of(ptr, type, member) \
    ((type *)(((char *)(ptr)) - offsetof(type, member)))


#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define free_null(a) \
	do { \
		free( a ); \
		a = nullptr; \
	} while(0)

#define delete_null(ptr) \
	do { \
		delete ptr; \
		ptr = nullptr; \
	} while (0)

#define delete_array_null(arr_ptr) \
	do { \
		delete[] arr_ptr; \
		arr_ptr = nullptr; \
	} while (0)

#define empty_str(str) (!str || !*str)
#define stringify(x)	#x
