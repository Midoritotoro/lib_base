#include <base/qt/ui/animations/AnimationManager.h>
#include <qDebug>


namespace base::qt::ui::animations {
	void AnimationManager::start(AnimationBase* animation) {
		Expects(animation != nullptr);
		_animation = animation;

		_timer.setCallback([=] {
			if (_animation->_opacity < 0 || _animation->opacity() > 1)
				stop();

			update();
		});

		const auto iterations = _animation->_opacityAnimation.duration
			/ _animation->_opacityAnimation.updateTimeout;

		const auto hide = (_animation->_opacityAnimation.from
			> _animation->_opacityAnimation.to);

		_opacityStep = (_animation->_opacityAnimation.to
			- _animation->_opacityAnimation.from) / iterations;

		_timer.callEach(_animation->_opacityAnimation.updateTimeout);
	}

	void AnimationManager::stop() {
		_timer.cancel();
	}

	bool AnimationManager::animating() const noexcept {
		return _timer.isActive() 
			&& _animation->_opacity > 0
			&& _animation->opacity() < 1;
	}

	void AnimationManager::update() {
		_animation->_opacity += _opacityStep;
		_animation->call();
	}
} // namespace base::qt::ui::animations