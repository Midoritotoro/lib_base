#pragma once 

#include <base/core/Types.h>
#include <type_traits>


namespace base::images {
	template <typename Integer = int32>
		requires std::_Is_nonbool_integral<Integer>
	class Point final {
	public:
		constexpr Point() noexcept {}
		constexpr Point(Integer x, Integer y) noexcept {
			_x = x;
			_y = y;
		}

		constexpr explicit Point(const Point<Integer>& point) noexcept {
			_x = point._x;
			_y = point._y;
		}

		constexpr inline void setX(Integer x) {
			_x = x;
		}

		constexpr inline void setY(Integer y) {
			_y = y;
		}

		constexpr inline [[nodiscard]] Integer x() const noexcept {
			return _x;
		}

		constexpr inline [[nodiscard]] Integer y() const noexcept {
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
		constexpr Size() noexcept {}
		constexpr Size(Integer width, Integer height) noexcept {
			_width = width;
			_height = height;
		}

		constexpr explicit Size(const Size<Integer>& size) noexcept {
			_width = size._width;
			_height = size._height;
		}

		constexpr inline void setWidth(Integer width) {
			_width = width;
		}

		constexpr inline void setHeight(Integer height) {
			_height = height;
		}

		constexpr inline [[nodiscard]] Integer width() const noexcept {
			return _width;
		}

		constexpr inline [[nodiscard]] Integer height() const noexcept {
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
		constexpr Rect() noexcept {}
		constexpr explicit Rect(
			const Point<IntegerPoint>& point,
			const Size<IntegerSize>& size)
		{
			_size = size;
			_pointLeftXY = point;
		}

		constexpr explicit Rect(
			IntegerPoint x, IntegerPoint y,
			const Size<IntegerSize>& size)
		{
			_size = size;
			_pointLeftXY = Point<IntegerPoint>(x, y);
		}

		constexpr explicit Rect(
			const Point<IntegerPoint>& point,
			IntegerSize width, IntegerSize height)
		{
			_size = Size<IntegerSize>(width, height);
			_pointLeftXY = point;
		}

		constexpr explicit Rect(
			IntegerPoint x, IntegerPoint y,
			IntegerSize width, IntegerSize height)
		{
			_size = Size<IntegerSize>(width, height);
			_pointLeftXY = Point<IntegerPoint>(x, y);
		}

		constexpr inline [[nodiscard]] Size<IntegerSize> size() const noexcept {
			return _size;
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> point() const noexcept {
			return _pointLeftXY;
		}

		constexpr inline void setWidth(IntegerSize width) {
			_size.setWidth(width);
		}

		constexpr inline void setHeight(IntegerSize height) {
			_size.setHeight(height);
		}

		constexpr inline [[nodiscard]] bool isNull() const noexcept
		{
			return 
				_pointRightXY.x() == _pointLeftXY.x() - 1 
				&& _pointRightXY.y() == _pointLeftXY.y() - 1;
		}

		constexpr inline [[nodiscard]] bool isEmpty() const noexcept
		{
			return 
				_pointLeftXY.x() > _pointRightXY.x() 
				| _pointLeftXY.y() > _pointRightXY.y();
		}

		constexpr inline [[nodiscard]] bool isValid() const noexcept
		{
			return 
				_pointLeftXY.x() <= _pointRightXY.x() 
				&& _pointLeftXY.y() <= _pointRightXY.y();
		}

		constexpr inline [[nodiscard]] int left() const noexcept
		{
			return _pointLeftXY.x();
		}

		constexpr inline [[nodiscard]] int top() const noexcept
		{
			return _pointLeftXY.y();
		}

		constexpr inline [[nodiscard]] int right() const noexcept
		{
			return _pointRightXY.x();
		}

		constexpr inline [[nodiscard]] int bottom() const noexcept
		{
			return _pointRightXY.y();
		}

		constexpr inline [[nodiscard]] int x() const noexcept
		{
			return _pointLeftXY.x();
		}

		constexpr inline [[nodiscard]] int y() const noexcept
		{
			return _pointLeftXY.y();
		}

		constexpr inline void setLeft(int pos) noexcept
		{
			_pointLeftXY.setX(pos);
		}

		constexpr inline void setTop(int pos) noexcept
		{
			_pointLeftXY.setY(pos);
		}

		constexpr inline void setRight(int pos) noexcept
		{
			_pointRightXY.setX(pos);
		}

		constexpr inline void setBottom(int pos) noexcept
		{
			_pointRightXY.setY(pos);
		}

		constexpr inline void setTopLeft(const Point<IntegerPoint>& p) noexcept
		{
			_pointLeftXY.setX(p.x());
			_pointLeftXY.setY(p.y());
		}

		constexpr inline void setBottomRight(const Point<IntegerPoint>& p) noexcept
		{
			_pointRightXY.setX(p.x());
			_pointRightXY.setY(p.y());
		}

		constexpr inline void setTopRight(const Point<IntegerPoint>& p) noexcept
		{
			_pointRightXY.setX(p.x());
			_pointLeftXY.setY(p.y());
		}

		constexpr inline void setBottomLeft(const Point<IntegerPoint>& p) noexcept
		{
			_pointLeftXY.setX(p.x());
			_pointRightXY.setY(p.y());
		}

		constexpr inline void setX(int ax) noexcept
		{
			_pointLeftXY.setX(ax);
		}

		constexpr inline void setY(int ay) noexcept
		{
			_pointLeftXY.setY(ay);
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> topLeft() const noexcept
		{
			return _pointLeftXY;
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> bottomRight() const noexcept
		{
			return _pointRightXY;
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> topRight() const noexcept
		{
			return Point<IntegerPoint>(_pointRightXY.x(), _pointLeftXY.y());
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> bottomLeft() const noexcept
		{
			return Point<IntegerPoint>(_pointLeftXY.x(), _pointRightXY.y());
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> center() const noexcept
		{
			return Point<IntegerPoint>(
				IntegerPoint((int64(_pointLeftXY.x()) + _pointRightXY.x()) / 2),
				IntegerPoint((int64(_pointLeftXY.y()) + _pointRightXY.y()) / 2));
		}

		constexpr inline [[nodiscard]] int width() const noexcept
		{
			return _pointRightXY.x() - _pointLeftXY.x() + 1;
		}

		constexpr inline [[nodiscard]] int height() const noexcept
		{
			return _pointRightXY.y() - _pointLeftXY.y() + 1;
		}
	private:
		Size<IntegerSize> _size;

		Point<IntegerPoint> _pointLeftXY;
		Point<IntegerPoint> _pointRightXY;
	};
} // namespace base::images