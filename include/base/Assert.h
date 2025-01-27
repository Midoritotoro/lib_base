#pragma once 

#include <base/Platform.h>

#include <cstdio>
#include <cstdlib>

#include <cassert>


#if defined (__GNUC__) || defined (__clang__)
#define AssertUnlikelyHelper(x) __builtin_expect(!!(x), 0)
#else
#define AssertUnlikelyHelper(x) (!!(x))
#endif

static inline void fail(
	const char* message,
	const char* file,
	int line)
{
	printf("Error: %s in File \"%s\", Line: %d\n", message, file, line);

	// Crash with access violation and generate crash report.
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


#define AssertValidationCondition(condition, message, file, line)\
	((AssertUnlikelyHelper(!(condition)))\
		? fail(message, file, line)\
		: void(0))

#define SOURCE_FILE_BASENAME (extract_basename(\
	__FILE__,\
	sizeof(__FILE__)))

#define AssertLog(condition, message) (AssertValidationCondition(\
	condition,\
	message,\
	SOURCE_FILE_BASENAME,\
	__LINE__))
#define Assert(condition) AssertLog(condition, "\"" #condition "\"")


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