#pragma once 

#include <base/Types.h>
#include <type_traits>

namespace base::images {
	template <typename Integer = int32>
		requires std::_Is_nonbool_integral<Integer>
	class Point {
	public:
		explicit Point(Integer x, Integer y) {
			_x = x;
			_y = y;
		}

		Point(const Point& point) = default;
		Point& operator=(const Point& point) = default;

		inline void setX(Integer x) {
			_x = x;
		}

		inline void setY(Integer y) {
			_y = y;
		}

		inline [[nodiscard]] Integer x() const noexcept {
			return _x;
		}

		inline [[nodiscard]] Integer y() const noexcept {
			return _y;
		}
	private:
		Integer _x = 0;
		Integer _y = 0;
	};
	
	template <typename Integer = int32>
		requires std::_Is_nonbool_integral<Integer>
	class Size final {
	public:
		explicit Size(Integer width, Integer height) {
			_width = width;
			_height = height;
		}

		inline void setWidth(Integer width) {
			_width = width;
		}

		inline void setHeight(Integer height) {
			_height = height;
		}

		inline [[nodiscard]] Integer width() const noexcept {
			return _width;
		}

		inline [[nodiscard]] Integer height() const noexcept {
			return _height;
		}
	private:
		Integer _width = 0;
		Integer _height = 0;
	};

	template <
		typename IntegerPoint = int32,
		typename IntegerSize = int32>
		requires 
			std::_Is_nonbool_integral<IntegerPoint>
			&& std::_Is_nonbool_integral<IntegerSize>
	class Rect final {
	public:
		explicit Rect(
			const Size<IntegerSize>& size,
			const Point<IntegerPoint>& point) 
		{
			_size = size;
			_point = point;
		}

		explicit Rect(
			const Size<IntegerSize>& size,
			IntegerPoint x, IntegerPoint y)
		{
			_size = size;
			_point = Point<IntegerPoint>(x, y);
		}

		explicit Rect(
			IntegerSize width, IntegerSize height,
			const Point<IntegerPoint>& point)
		{
			_size = Size<IntegerSize>(width, height);
			_point = point;
		}

		explicit Rect(
			IntegerSize width, IntegerSize height,
			IntegerPoint x, IntegerPoint y)
		{
			_size = Size<IntegerSize>(width, height);
			_point = Point<IntegerPoint>(x, y);
		}

		inline [[nodiscard]] const Size<IntegerSize> & size() const noexcept {
			return _size;
		}

		inline [[nodiscard]] const Point<IntegerPoint> & point() const noexcept {
			return _point;
		}

		inline [[nodiscard]] IntegerSize width() const noexcept {
			return _size.width();
		}

		inline [[nodiscard]] IntegerSize height() const noexcept {
			return _size.height();
		}

		inline [[nodiscard]] IntegerPoint x() const noexcept {
			return _point.x();
		}

		inline [[nodiscard]] IntegerPoint y() const noexcept {
			return _point.y();
		}

		inline void setX(IntegerPoint x) {
			_point.setX(x);
		}

		inline void setY(IntegerPoint y) {
			_point.setY(y);
		}

		inline void setWidth(IntegerSize width) {
			_size.setWidth(width);
		}

		inline void setHeight(IntegerSize height) {
			_size.setHeight(height);
		}
	private:
		Size<IntegerSize> _size;
		Point<IntegerPoint> _point;
	};
} // namespace base::images