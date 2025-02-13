#pragma once

#include <cstdio>
#include <base/system/CompilerDetection.h>


#if __has_include(<gsl/gsl>)
	#include <gsl/gsl>

/**
 *����� ������ �� ������� ������� ��������� ������� ����������� �� ���������� ����� ����� ���� �����
*/
#define measureExecutionTime(...) \
  const auto ms = base::Time::now(); \
  const auto timer = gsl::finally([&]() { \
    std::cout << (__VA_ARGS__[0] ? __VA_ARGS__[0] : FUNC_INFO) << " completed for: " \
              << base::Time::now() - ms << " ms" << '\n'; });

//!
//! \brief
//! ����� ������ �� ������� ������� ��������� ���������� ����������� �� ���������� ����� ����� ���� ����� � value
#define measureExecutionTimeToValue(value) \
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([&]() mutable { \
		value = base::Time::now() - ms; });


#define 

//!
//! \brief
//! ���������, �������� �� ptr � ��������� member ������ ��������� ���� type
//! \param ptr - ��������� �� ���� ��������� (���� member).
//! \param type - ��� ���������, ������� �������� member.
//! \param member - ��� ����� ���������, �� ������� ��������� ptr.
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
