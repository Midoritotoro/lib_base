#include <base/qt/ui/animations/AnimationManager.h>

#include <base/qt/ui/animations/OpacityAnimation.h>
#include <base/qt/ui/animations/HorizontalGrowthAnimation.h>

#include <base/qt/ui/animations/VerticalGrowthAnimation.h>
#include <base/qt/ui/animations/CombinedGrowthAnimation.h>

#include <qDebug>


namespace base::qt::ui::animations {
	void AnimationManager::start(not_null<AnimationBase*> animation) {
		_currentAnimationType = animation->_animationType;
		_iterations = animation->_duration
			/ animation->_updateTimeout;

		switch (_currentAnimationType) {
		case AnimationType::Opacity: {
				const auto hide = (_animation._opacity->_opacityFrom
					> _animation._opacity->_opacityTo);

				const auto opacityStep = (_animation._opacity->_opacityTo
					- _animation._opacity->_opacityFrom) / _iterations;

				_step._opacity = hide ? -opacityStep : opacityStep;
				_animation._opacity = static_cast<OpacityAnimation*>(animation.get());
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
				unreachable();
		}

		_timer.setCallback([this] {
			if ((_iterations <= 0) || (_currentAnimationType == AnimationType::Opacity
				&& _animation._opacity->_opacity < 0 
				|| _animation._opacity->_opacity > 1))
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
		qDebug() << _iterations;
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
				int x = _animation._combined->_rect.x();
				int y = _animation._combined->_rect.y();

				int width = _animation._combined->_rect.width();
				int height = _animation._combined->_rect.height();

				if (_animation._combined->_direction == DirectionFlag::LeftToRight)
					width += _step._combined._horizontal;
				else
					width -= _step._combined._horizontal;

				if (_animation._combined->_direction == DirectionFlag::TopToBottom)
					height += _step._combined._vertical;
				else
					height -= _step._combined._vertical;


				if (_animation._combined->_startCorner  == CombinedGrowthAnimation::Corner::LeftTop) {
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

				qDebug() << "x: " << x << "y: " << y;

				_animation._combined->_rect.setRect(x, y, width, height);
			}
				_animation._combined->call();
				break;
			default:
				unreachable();
		}

		--_iterations;
	}

	void AnimationManager::adjustCombined(not_null<CombinedGrowthAnimation*> animation)
	{
		switch (animation->_startCorner) {
		case CombinedGrowthAnimation::Corner::LeftTop:
			_animation._combined->_rect.setSize(QSize(0, 0));
			break;
		case CombinedGrowthAnimation::Corner::LeftBottom:
			_animation._combined->_rect.setWidth(0);
			break;
		case CombinedGrowthAnimation::Corner::RightTop:
			_animation._combined->_rect.setHeight(0);
			break;
		case CombinedGrowthAnimation::Corner::RightBottom:
			break;
		default:
			unreachable();
		}
	}
} // namespace base::qt::ui::animations