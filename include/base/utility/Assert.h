#pragma once 

#include <base/system/Platform.h>

#include <cstdlib>
#include <cassert>


static inline void fail(
	const char* message,
	const char* file,
	int line)
{
	printf("Error: %s in File \"%s\", Line: %d\n", message, file, line);

	// Сбой из-за нарушения прав доступа и создание отчета о сбое.

	volatile auto nullptr_value = (int*)nullptr;
	*nullptr_value = 0;

	::std::abort();
}

static inline constexpr [[nodiscard]]
	const char* extract_basename(
		const char* path,
		size_t size)
	{
		while (size != 0 && path[size - 1] != '/' && path[size - 1] != '\\')
			--size;

		return path + size;
	}

#define ReturnOnFailure(message, file, line, retval) \
	do { \
		printf("Error: %s in File \"%s\", Line: %d\n", message, file, line); \
		return retval; \
	} \
		while (0)
	

#define AssertValidationCondition(condition, message, file, line)\
	((unlikely(!(condition)))\
		? fail(message, file, line)\
		: void(0))

#define AssertValidationConditionWithRet(condition, message, file, line, retval)\
	if ((unlikely(!(condition)))) \
		ReturnOnFailure(message, file, line, retval)

#define SOURCE_FILE_BASENAME (extract_basename(\
	__FILE__,\
	sizeof(__FILE__)))

#define AssertLog(condition, message) (AssertValidationCondition(\
	condition,\
	message,\
	SOURCE_FILE_BASENAME,\
	__LINE__))

// Возвращает return_value в случае ошибки вместо вызова std::abort
#define AssertReturn(condition, message, return_value) AssertValidationConditionWithRet(\
	condition,\
	message,\
	SOURCE_FILE_BASENAME,\
	__LINE__, \
	return_value)


#define Assert(condition) AssertLog(condition, "\"" #condition "\"")
#define AssertUnreachable() assert(!"unreachable", unreachable())


#define StaticAssert(cond) static_assert(bool(cond), stringify(cond))
#define StaticAssertLog(cond, mes) static_assert(bool(cond), mes)

#ifdef Expects
#undef Expects
#endif // Expects
#define Expects(condition) (AssertValidationCondition(\
	condition,\
	"\"" #condition "\"",\
	SOURCE_FILE_BASENAME,\
	__LINE__))

#ifdef Ensures
#undef Ensures
#endif // Ensures
#define Ensures(condition) (AssertValidationCondition(\
	condition,\
	"\"" #condition "\"",\
	SOURCE_FILE_BASENAME,\
	__LINE__))

#ifdef Unexpected
#undef Unexpected
#endif // Unexpected
#define Unexpected(message) (fail(\
	"Unexpected: " message,\
	SOURCE_FILE_BASENAME,\
	__LINE__))

#ifdef _DEBUG
#define AssertIsDebug(...)
#endif // _DEBUG