#pragma once 

#include <base/Platform.h>

#include <base/images/StbImage.h>
#include <base/images/StbImageWrite.h>

#include <base/Types.h>

#ifdef LIB_BASE_ENABLE_QT
#include <QImage>
#endif

namespace base::gl {
	class Image;
}

namespace base::images {
#ifdef LIB_BASE_ENABLE_QT
	using Image = QImage;
	using Rect = QRect;
	using Size = QSize;
#else
	struct Size {
		int _width = 0;
		int _height = 0;

		void setWidth(int width) {
			_width = width;
		}

		void setHeight(int height) {
			_height = height;
		}

		[[nodiscard]] int width() const {
			return _width;
		}

		[[nodiscard]] int height() const {
			return _height;
		}
	};

	struct Rect {
		Size _size;

		int _x = 0;
		int _y = 0;

		[[nodiscard]] Size size() const {
			return _size;
		}

		[[nodiscard]] int width() const {
			return size.width();
		}

		[[nodiscard]] int height() const {
			return size.height();
		}

		[[nodiscard]] int x() const {
			return _x;
		}

		[[nodiscard]] int y() const {
			return _y;
		}

		void setX(int xCoord) {
			_x = xCoord;
		}

		void setY(int yCoord) {
			_y = yCoord;
		}

		void setWidth(int width) {
			_size.setWidth(width);
		}

		void setHeight(int height) {
			_size.setHeight(height);
		}
	};

	using Image = gl::Image;
#endif
} // namespace base::images