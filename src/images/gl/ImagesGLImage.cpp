#include <base/images/gl/ImagesGLImage.h>
#include <base/core/utility/OverflowCheck.h>

#include <base/core/utility/Assert.h>

#ifdef LIB_BASE_ENABLE_QT
	#include <base//images/ImagesUtility.h>
#endif

#include <base/core/utility/Algorithm.h>


__BASE_IMAGES_NAMESPACE_BEGIN

#if defined(LIB_BASE_ENABLE_OPENGL) || defined(LIB_BASE_ENABLE_QT_OPENGL) || defined(LIB_BASE_FOUND_QT_OPENGL)
	void GLImage::paint() {
	#ifdef LIB_BASE_ENABLE_OPENGL

	#endif

	#if defined(LIB_BASE_ENABLE_QT_OPENGL) || defined(LIB_BASE_FOUND_QT_OPENGL)

	#endif
	}
#endif

__BASE_IMAGES_NAMESPACE_END