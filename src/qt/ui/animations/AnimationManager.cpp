#include <base/qt/ui/animations/AnimationManager.h>

#include <base/qt/ui/animations/OpacityAnimation.h>
#include <base/qt/ui/animations/HorizontalGrowthAnimation.h>

#include <base/qt/ui/animations/VerticalGrowthAnimation.h>
#include <base/qt/ui/animations/CombinedGrowthAnimation.h>

#include <qDebug>

__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

void AnimationManager::start(not_null<AnimationBase*> animation) {
	_currentAnimationType = animation->_animationType;
	_iterations = animation->_duration
		/ animation->_updateTimeout;

	switch (_currentAnimationType) {
		case AnimationType::Opacity: {
			_animation._opacity = static_cast<OpacityAnimation*>(animation.get());

			const auto hide = (_animation._opacity->_opacityFrom
				> _animation._opacity->_opacityTo);

			const auto opacityStep = (_animation._opacity->_opacityTo
				- _animation._opacity->_opacityFrom) / _iterations;

			_step._opacity = hide ? -opacityStep : opacityStep;
		}
			break;

		case AnimationType::HorizontalGrowth:
			_animation._horizontal = static_cast<HorizontalGrowthAnimation*>(animation.get());
			_step._rect = _animation._horizontal->_rect.width() / _iterations;

			if (_animation._horizontal->_direction == HorizontalGrowthAnimation::Direction::LeftToRight)
				_animation._horizontal->_rect.setWidth(0);

			break;

		case AnimationType::VerticalGrowth:
			_animation._vertical = static_cast<VerticalGrowthAnimation*>(animation.get());
			_step._rect = _animation._horizontal->_rect.height() / _iterations;

			if (_animation._vertical->_direction == VerticalGrowthAnimation::Direction::TopToBottom)
				_animation._vertical->_rect.setHeight(0);

			break;

		case AnimationType::CombinedGrowth:
			_animation._combined = static_cast<CombinedGrowthAnimation*>(animation.get());

			_step._combined._horizontal = _animation._combined->_rect.width() / _iterations;
			_step._combined._vertical = _animation._combined->_rect.height() / _iterations;

			adjustCombined(_animation._combined);
			break;

		default:
			AssertUnreachable();
	}

	_timer.setCallback([this] {
		if (_iterations <= 1)
			stop();

		update();
	});

	_timer.callEach(animation->_updateTimeout);
}

void AnimationManager::stop() {
	_timer.cancel();
}

bool AnimationManager::animating() const noexcept {
	return _timer.isActive();
}

void AnimationManager::update() {
	--_iterations;

	switch (_currentAnimationType) {
		case AnimationType::Opacity:
			_animation._opacity->_opacity += _step._opacity;

			_animation._opacity->call();
			break;
		case AnimationType::HorizontalGrowth:
			_animation._horizontal->_direction == HorizontalGrowthAnimation::Direction::LeftToRight
				? _animation._horizontal->_rect.setWidth(
					_animation._horizontal->_rect.width() + _step._rect)
				: _animation._horizontal->_rect.setWidth(
					_animation._horizontal->_rect.width() - _step._rect);

			_animation._horizontal->call();
			break;
		case AnimationType::VerticalGrowth:
			_animation._vertical->_direction == VerticalGrowthAnimation::Direction::TopToBottom
				? _animation._vertical->_rect.setHeight(
					_animation._vertical->_rect.height() + _step._rect)
				: _animation._vertical->_rect.setHeight(
					_animation._vertical->_rect.height() - _step._rect);
			
			_animation._vertical->call();
			break;
		case AnimationType::CombinedGrowth: {
			float x = _animation._combined->_rect.x();
			float y = _animation._combined->_rect.y();

			float width = _animation._combined->_rect.width();
			float height = _animation._combined->_rect.height();

			if (_animation._combined->_direction & DirectionFlag::LeftToRight)
				width += _step._combined._horizontal;
			else if (_animation._combined->_direction & DirectionFlag::RightToLeft)
				width -= _step._combined._horizontal;

			if (_animation._combined->_direction & DirectionFlag::TopToBottom)
				height += _step._combined._vertical;
			else if (_animation._combined->_direction & DirectionFlag::BottomToTop)
				height -= _step._combined._vertical;

			if (_animation._combined->_startCorner == CombinedGrowthAnimation::Corner::LeftTop) {
			}
			else if (_animation._combined->_startCorner == CombinedGrowthAnimation::Corner::LeftBottom) {
				y -= height - _animation._combined->_rect.height();
			}
			else if (_animation._combined->_startCorner == CombinedGrowthAnimation::Corner::RightTop) {
				x -= width - _animation._combined->_rect.width();
			}
			else if (_animation._combined->_startCorner == CombinedGrowthAnimation::Corner::RightBottom)
			{
				x -= width - _animation._combined->_rect.width();
				y -= height - _animation._combined->_rect.height();
			}

			_animation._combined->_rect.setRect(x, y, width, height);
		}
			_animation._combined->call();
			break;
		default:
			AssertUnreachable();
	}
}

void AnimationManager::adjustCombined(not_null<CombinedGrowthAnimation*> animation)
{
	switch (animation->_startCorner) {
		case CombinedGrowthAnimation::Corner::LeftTop:
			animation->_rect.setSize(QSize(0, 0));
			break;
		case CombinedGrowthAnimation::Corner::LeftBottom:
			animation->_rect.setWidth(0);
			break;
		case CombinedGrowthAnimation::Corner::RightTop:
			animation->_rect.setHeight(0);
			break;
		case CombinedGrowthAnimation::Corner::RightBottom:
			break;
		default:
			AssertUnreachable();
	}
}

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END
