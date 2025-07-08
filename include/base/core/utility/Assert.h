#pragma once  

#include <base/core/arch/CompilerDetection.h>

#include <cstdlib>
#include <cassert>

#include <iostream>

WARNING_DISABLE_MSVC(6011)

struct staticlocale {
	staticlocale() {
		setlocale(LC_ALL, "");
	}
};

static staticlocale lc;

static inline void fail(
	const char* message,
	const char* file,
	int line)
{
	printf("Error: %s in File \"%s\", Line: %d\n", message, file, line);

	// Crash due to access violation and create crash report.
	 
	volatile auto nullptr_value = (int*)nullptr;
	*nullptr_value = 0;

	std::abort();
	std::terminate();
}

static inline constexpr NODISCARD
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
	((UNLIKELY(!(condition)))\
		? fail(message, file, line)\
		: void(0))

#define AssertValidationConditionWithRet(condition, message, file, line, retval)\
	if ((UNLIKELY(!(condition)))) \
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
#define AssertUnreachable() Assert(!"unreachable", UNREACHABLE())


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

#define DebugAssertReturn	AssertReturn
#define DebugAssert			Assert

#define DebugAssertLog		AssertLog
#define AssertIsDebug(...)

#else 

#define DebugAssertReturn(condition, message, return_value)
#define DebugAssert(condition)

#define DebugAssertLog(condition, message)

#endif // _DEBUG