#include <base/qt/ui/animations/AnimationManager.h>


namespace base::qt::ui::animations {
	void AnimationManager::start(AnimationBase* animation) {
		_currentAnimation = animation;

		_timer.setCallback([=] { 
			_animationDurationProgress -= _currentAnimation->_animationProgress.updateTimeout;

			if (_animationDurationProgress <= 0 || _currentAnimation->_currentOpacity <= 0)
				_timer.cancel();

			update();
		});
		_timer.callEach(
			_currentAnimation->_animationProgress.duration 
			/ _currentAnimation->_animationProgress.updateTimeout);
	}

	void AnimationManager::stop() {
		_timer.cancel();
	}

	bool AnimationManager::animating() const noexcept {
		return _timer.isActive() && _animationDurationProgress > 0;
	}

	void AnimationManager::update() {
		_currentAnimation->_currentOpacity = _currentAnimation->_currentOpacity -
			(_currentAnimation->_currentOpacity /
				(_currentAnimation->_animationProgress.duration
					/ _currentAnimation->_animationProgress.updateTimeout));

		_currentAnimation->call();
	}
} // namespace base::qt::ui::animations