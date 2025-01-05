#include <base/qt/ui/animations/Animation.h>
#include <base/Assert.h>


namespace base::qt::ui::animations {
	AnimationType Animation::animationType() const noexcept {
		return _animationType;
	}

	void Animation::setAnimationType(AnimationType type) {
		_animationType = type;
	}

	void Animation::start(
		float from,
		float to,
		Time::time_t duration,
		Time::time_t updateTimeout)
	{
		Expects(duration != 0);
		Expects(from != to);

		_animationStart = Time::now();

		_animationProgress = AnimationProgress{
			.from = from,
			.to = to,
			.duration = duration,
			.updateTimeout = std::clamp(updateTimeout, 
				kMinimumAnimationUpdateTimeout,
				kMaximumAnimationUpdateTimeout)
		};

		_animationManager.start(this);
	}

	void Animation::stop() {
		_animationManager.stop();
	}
} // namespace base::qt::ui::animations