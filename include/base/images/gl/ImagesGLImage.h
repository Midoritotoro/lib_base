#pragma once 

#include <base/images/Images.h>
#include <string>


__BASE_IMAGES_NAMESPACE_BEGIN

class IntegralImage;

class GLImage final {
public:

#if defined(LIB_BASE_ENABLE_OPENGL) || defined(LIB_BASE_ENABLE_QT_OPENGL) || defined(LIB_BASE_FOUND_QT_OPENGL)
	void paint();
#endif
private:
//	GLImageData* _data = nullptr;
};

__BASE_IMAGES_NAMESPACE_END
