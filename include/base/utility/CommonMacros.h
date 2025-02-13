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
 *ѕосле выхода из текущей области видимости выводит затраченное на выполнение этого блока кода врем€
 *\param name - "»м€" области видимости
*/
#define measureExecutionTime(name) \
	const auto ms = base::Time::now(); \
	const auto timer = gsl::finally([=] { \
		std::cout << name << " completed for: " \
		<< base::Time::now() - ms << " ms" << '\n'; });
//!
//! \brief
//! ѕосле выхода из текущей области видимости записывает затраченное на выполнение этого блока кода врем€ в value
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
//! ѕровер€ет, €вл€етс€ ли ptr с названием member членом структуры типа type
//! \param ptr - ”казатель на член структуры (типа member).
//! \param type - “ип структуры, котора€ содержит member.
//! \param member - »м€ члена структуры, на который указывает ptr.
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
