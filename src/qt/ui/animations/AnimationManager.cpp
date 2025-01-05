#include <base/qt/ui/animations/AnimationManager.h>


namespace base::qt::ui::animations {
	void AnimationManager::start(AnimationBase* animation) {
		_currentAnimation = animation;

		_timer.setCallback([=] { 
			_animationDurationProgress -= _currentAnimation->_animationProgress.updateTimeout;

			if (_animationDurationProgress <= 0)
				_timer.cancel();

			update();
		});
		_timer.callEach(_currentAnimation->_animationProgress.duration 
			/ _currentAnimation->_animationProgress.updateTimeout);
	}

	void AnimationManager::stop() {

	}

	void AnimationManager::update() {
		_currentAnimation->call();
	}
} // namespace base::qt::ui::animations