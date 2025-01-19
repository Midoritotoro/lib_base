#pragma once 

#include <base/Types.h>
#include <type_traits>

#include <qrect.h>

namespace base::images {
	template <typename Integer = int32>
		requires std::_Is_nonbool_integral<Integer>
	class Point final {
	public:
		constexpr explicit Point(Integer x, Integer y) noexcept {
			_x = x;
			_y = y;
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
		constexpr explicit Size(Integer width, Integer height) noexcept {
			_width = width;
			_height = height;
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

		constexpr inline [[nodiscard]] bool Rect::isNull() const noexcept
		{
			return 
				_pointRightXY.x() == _pointLeftXY.x() - 1 
				&& _pointRightXY.y() == _pointLeftXY.y() - 1;
		}

		constexpr inline [[nodiscard]] bool Rect::isEmpty() const noexcept
		{
			return 
				_pointLeftXY.x() > _pointRightXY.x() 
				| _pointLeftXY.y() > _pointRightXY.y();
		}

		constexpr inline [[nodiscard]] bool Rect::isValid() const noexcept
		{
			return 
				_pointLeftXY.x() <= _pointRightXY.x() 
				&& _pointLeftXY.y() <= _pointRightXY.y();
		}

		constexpr inline [[nodiscard]] int Rect::left() const noexcept
		{
			return _pointLeftXY.x();
		}

		constexpr inline [[nodiscard]] int Rect::top() const noexcept
		{
			return _pointLeftXY.y();
		}

		constexpr inline [[nodiscard]] int Rect::right() const noexcept
		{
			return _pointRightXY.x();
		}

		constexpr inline [[nodiscard]] int Rect::bottom() const noexcept
		{
			return _pointRightXY.y();
		}

		constexpr inline [[nodiscard]] int Rect::x() const noexcept
		{
			return _pointLeftXY.x();
		}

		constexpr inline [[nodiscard]] int Rect::y() const noexcept
		{
			return _pointLeftXY.y();
		}

		constexpr inline void Rect::setLeft(int pos) noexcept
		{
			_pointLeftXY.setX(pos);
		}

		constexpr inline void Rect::setTop(int pos) noexcept
		{
			_pointLeftXY.setY(pos);
		}

		constexpr inline void Rect::setRight(int pos) noexcept
		{
			_pointRightXY.setX(pos);
		}

		constexpr inline void Rect::setBottom(int pos) noexcept
		{
			_pointRightXY.setY(pos);
		}

		constexpr inline void Rect::setTopLeft(const Point<IntegerPoint>& p) noexcept
		{
			_pointLeftXY.setX(p.x());
			_pointLeftXY.setY(p.y());
		}

		constexpr inline void Rect::setBottomRight(const Point<IntegerPoint>& p) noexcept
		{
			_pointRightXY.setX(p.x());
			_pointRightXY.setY(p.y());
		}

		constexpr inline void Rect::setTopRight(const Point<IntegerPoint>& p) noexcept
		{
			_pointRightXY.setX(p.x());
			_pointLeftXY.setY(p.y());
		}

		constexpr inline void Rect::setBottomLeft(const Point<IntegerPoint>& p) noexcept
		{
			_pointLeftXY.setX(p.x());
			_pointRightXY.setY(p.y());
		}

		constexpr inline void Rect::setX(int ax) noexcept
		{
			_pointLeftXY.setX(ax);
		}

		constexpr inline void Rect::setY(int ay) noexcept
		{
			_pointLeftXY.setY(ay);
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> Rect::topLeft() const noexcept
		{
			return _pointLeftXY;
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> Rect::bottomRight() const noexcept
		{
			return _pointRightXY;
		}

		constexpr inline [[nodiscard]] Point<IntegerPoint> Rect::topRight() const noexcept
		{
			return Point<IntegerPoint>(_pointRightXY.x(), _pointLeftXY.y());
		}

		constexpr inline [[nodiscard]] Point Rect::bottomLeft() const noexcept
		{
			return Point<IntegerPoint>(_pointLeftXY.x(), _pointRightXY.y());
		}

		constexpr inline [[nodiscard]] QPoint Rect::center() const noexcept
		{
			return Point<IntegerPoint>(
				IntegerPoint((int64(_pointLeftXY.x()) + _pointRightXY.x()) / 2),
				IntegerPoint((int64(_pointLeftXY.y()) + _pointRightXY.y()) / 2));
		}

		constexpr inline [[nodiscard]] int Rect::width() const noexcept
		{
			return _pointRightXY.x() - _pointLeftXY.x() + 1;
		}

		constexpr inline [[nodiscard]] int Rect::height() const noexcept
		{
			return _pointRightXY.y() - _pointLeftXY.y() + 1;
		}

		constexpr inline [[nodiscard]] Size<IntegerSize> Rect::size() const noexcept
		{
			return _size;
		}

	private:
		Size<IntegerSize> _size;

		Point<IntegerPoint> _pointLeftXY;
		Point<IntegerPoint> _pointRightXY;
	};
} // namespace base::images