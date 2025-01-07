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

		auto iterations = 0;

		switch (_currentAnimationType) {
			case AnimationType::Opacity:
				_animation._opacity = static_cast<OpacityAnimation*>(animation.get());
				iterations = _animation._opacity->_duration
					/ _animation._opacity->_updateTimeout;
				break;
			case AnimationType::HorizontalGrowth:
				_animation._horizontal = static_cast<HorizontalGrowthAnimation*>(animation.get());
				iterations = _animation._horizontal->_duration
					/ _animation._horizontal->_updateTimeout;
				break;
			case AnimationType::VerticalGrowth:
				_animation._vertical = static_cast<VerticalGrowthAnimation*>(animation.get());
				iterations = _animation._horizontal->_duration
					/ _animation._horizontal->_updateTimeout;
				break;
			default:
				unreachable();
		}

		_timer.setCallback([=] {
			if (_currentAnimationType == AnimationType::Opacity
				&& _animation._opacity->_opacity < 0 
				|| _animation._opacity->opacity() > 1)
				stop();

			update();
		});

		//const auto hide = (_animation._opacity->_opacityFrom
		//		> _animation._opacity->_opacityTo);

		switch (_currentAnimationType) {
			case AnimationType::Opacity:
				_step._opacity = (_animation._opacity->_opacityTo
					- _animation._opacity->_opacityFrom) / iterations;
				break;
			case AnimationType::HorizontalGrowth:
				_step._rect = _animation._horizontal->_rect.width() / iterations;
				break;
			case AnimationType::VerticalGrowth:
				_step._rect = _animation._horizontal->_rect.height() / iterations;
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
		switch (_currentAnimationType) {
		case AnimationType::Opacity:
			_animation._opacity->_opacity += _step._opacity;
			_animation._opacity->call();
			break;
		case AnimationType::HorizontalGrowth:
			_animation._horizontal->_rect += _step._rect;
			_animation._horizontal->call();
			break;
		case AnimationType::VerticalGrowth:
			_animation._vertical->_rect += _step._rect;
			_animation._vertical->call();
			break;
		default:
			unreachable();
		}
	}
} // namespace base::qt::ui::animations