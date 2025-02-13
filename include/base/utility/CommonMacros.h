#pragma once

#include <cstdio>
#include <base/system/CompilerDetection.h>

#define dFUNC_CHOOSER_7(_f0, _f1, N, ... ) N

#define dFUNC_RECOMPOSER(argsWithParentheses)\
        dFUNC_CHOOSER_7 argsWithParentheses

#define dMACRO_CHOOSER(target_, ...)\
        dCHOOSE_FROM_ARG_COUNT(target_, target_##_NO_ARG_EXPANDER __VA_ARGS__ ())

#define dCHOOSE_FROM_ARG_COUNT(arg_, ...) \
        dFUNC_RECOMPOSER((__VA_ARGS__, arg_##_7, arg_##_6, arg_##_5, arg_##_4, arg_##_3, arg_##_2, arg_##_1, ))

#define ACTION(...)\
        dMACRO_CHOOSER( dACTION, __VA_ARGS__)(__VA_ARGS__)

#if __has_include(<gsl/gsl>)
	#include <gsl/gsl>
/**
 *����� ������ �� ������� ������� ��������� ������� ����������� �� ���������� ����� ����� ���� �����
 *\param name - "���" ������� ���������
*/
#define measureExecutionTime(name) \
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([=] { \
		std::cout << name << " completed for: " \
		<< base::Time::now() - ms << " ms" << '\n'; });
//!
//! \brief
//! ����� ������ �� ������� ������� ��������� ���������� ����������� �� ���������� ����� ����� ���� ����� � value
#define measureExecutionTimeToValue(value) \
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([&]() mutable { \
		value = base::Time::now() - ms; });

#define measureExecutionTime() \
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([=] { \
		std::cout << FUNC_INFO << " completed for: " \
		<< base::Time::now() - ms << " ms" << '\n'; });

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
