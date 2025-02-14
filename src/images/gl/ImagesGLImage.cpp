#include <base/images/gl/ImagesGLImage.h>
#include <base/utility/OverflowCheck.h>

#include <base/utility/Assert.h>

#ifdef LIB_BASE_ENABLE_QT
	#include <base/images/ImagesUtility.h>
#endif

#include <base/utility/Algorithm.h>


namespace base::images {
#if defined(LIB_BASE_ENABLE_OPENGL) || defined(LIB_BASE_ENABLE_QT_OPENGL) || defined(LIB_BASE_FOUND_QT_OPENGL)
	void GLImage::paint() {
	#ifdef LIB_BASE_ENABLE_OPENGL

	#endif

	#if defined(LIB_BASE_ENABLE_QT_OPENGL) || defined(LIB_BASE_FOUND_QT_OPENGL)

	#endif
	}
#endif
} // namespace base::images