#pragma once

#include <cstdio>
#include <base/system/CompilerDetection.h>

#include <gsl/gsl>
#include <base/system/Time.h>

#include <iostream>

#define _PP_CAT(a,b) a##b
#define PP_CAT(a,b) _PP_CAT(a,b)

#define ELEVENTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11
#define COUNT_ARGS(...) ELEVENTH_ARGUMENT(_, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define measureExecutionTime_0() \
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([&]() { \
	std::cout << (FUNC_INFO) << " completed for: " \
				<< base::Time::now() - ms << " ms" << '\n'; });

#define measureExecutionTime_1(name)		\
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([&]() { \
	std::cout << (name) << " completed for: " \
		<< base::Time::now() - ms << " ms" << '\n'; });

#define measureExecutionTime(...) \
	PP_CAT(measureExecutionTime_, \
		COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__) 

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
