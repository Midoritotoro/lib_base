#include <base/qt/ui/animations/AnimationManager.h>

#include <base/qt/ui/animations/OpacityAnimation.h>
#include <base/qt/ui/animations/HorizontalGrowthAnimation.h>

#include <base/qt/ui/animations/VerticalGrowthAnimation.h>

#include <qDebug>


namespace base::qt::ui::animations {
	namespace {
	}
	void AnimationManager::start(not_null<AnimationBase*> animation) {
		_currentAnimationType = animation->_animationType;
		_iterations = 0;

		switch (_currentAnimationType) {
			case AnimationType::Opacity:
				_animation._opacity = static_cast<OpacityAnimation*>(animation.get());
				_iterations = _animation._opacity->_duration
					/ _animation._opacity->_updateTimeout;
				break;
			case AnimationType::HorizontalGrowth:
				_animation._horizontal = static_cast<HorizontalGrowthAnimation*>(animation.get());
				_iterations = _animation._horizontal->_duration
					/ _animation._horizontal->_updateTimeout;
				break;
			case AnimationType::VerticalGrowth:
				_animation._vertical = static_cast<VerticalGrowthAnimation*>(animation.get());
				_iterations = _animation._horizontal->_duration
					/ _animation._horizontal->_updateTimeout;
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

		//const auto hide = (_animation._opacity->_opacityFrom
		//		> _animation._opacity->_opacityTo);

		switch (_currentAnimationType) {
			case AnimationType::Opacity:
				_step._opacity = (_animation._opacity->_opacityTo
					- _animation._opacity->_opacityFrom) / _iterations;
				break;
			case AnimationType::HorizontalGrowth:
				_step._rect = _animation._horizontal->_rect.width() / _iterations;
				if (_animation._horizontal->_direction == HorizontalGrowthAnimation::Direction::LeftToRight)
					_animation._horizontal->_rect.setWidth(0);
				break;
			case AnimationType::VerticalGrowth:
				_step._rect = _animation._horizontal->_rect.height() / _iterations;
				if (_animation._vertical->_direction == VerticalGrowthAnimation::Direction::TopToBottom)
					_animation._vertical->_rect.setHeight(0);
				break;
			default:
				unreachable();
		}

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
			default:
				unreachable();
		}

		--_iterations;
	}
} // namespace base::qt::ui::animations