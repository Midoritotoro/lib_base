#pragma once 

#ifdef LIB_BASE_ENABLE_QT
	#include <QString>
#endif

namespace base::string {
	#ifdef LIB_BASE_ENABLE_QT
		using String = QString;
	#else
		class String;
	#endif
}