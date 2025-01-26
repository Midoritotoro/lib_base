#include <base/images/gl/ImagesGLImage.h>
#include <base/OverflowCheck.h>

#include <base/Assert.h>
#ifdef LIB_BASE_ENABLE_QT
#include <base/images/ImagesQtUtility.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/StbImage.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/StbImageWrite.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <base/images/StbImageResize.h>


#include <base/Utility.h>


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