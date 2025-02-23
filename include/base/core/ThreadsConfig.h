#pragma once 

#include <base/utility/Assert.h>

// Отключает все внутренние исключения
#ifndef BASE_THREADS_NO_FAILURE
#  define BASE_THREADS_NO_FAILURE
#endif

#ifdef BASE_THREADS_NO_FAILURE
#  define ThreadsAssert						AssertReturn
#else
#  define ThreadsAssert(cond, mes, retval)	(AssertValidationCondition(\
	  cond,\
	  mes,\
	  SOURCE_FILE_BASENAME,\
	  __LINE__))
#endif
