#include <base/qt/ui/animations/AnimationManager.h>
#include <qDebug>


namespace base::qt::ui::animations {
	void AnimationManager::start(AnimationBase* animation) {
		_currentAnimation = animation;
		_animationDurationProgress = _currentAnimation->_animationProgress.duration;

		_timer.setCallback([=] { 
			_animationDurationProgress -= _currentAnimation->_animationProgress.updateTimeout;

			if (_animationDurationProgress <= 0)
				_timer.cancel();

			update();
		});

		_iterations = _currentAnimation->_animationProgress.duration
			/ _currentAnimation->_animationProgress.updateTimeout;

		const auto hide = (_currentAnimation->_animationProgress.from 
			> _currentAnimation->_animationProgress.to);

		_opacityStep = (_currentAnimation->_animationProgress.to
			- _currentAnimation->_animationProgress.from) / _iterations;

		_timer.callEach(_currentAnimation->_animationProgress.updateTimeout);
	}

	void AnimationManager::stop() {
		_timer.cancel();
	}

	bool AnimationManager::animating() const noexcept {
		return _timer.isActive() && _animationDurationProgress > 0;
	}

	void AnimationManager::update() {
		_currentAnimation->_currentOpacity += _opacityStep;

		qDebug() << "_opacityStep: " << _opacityStep;

		_currentAnimation->call();
	}
} // namespace base::qt::ui::animations