#include <base/qt/ui/animations/Animation.h>
#include <base/Assert.h>

#include <base/Concurrent.h>


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
		_opacity = from;

		_opacityAnimation = OpacityAnimation{
			.from = from,
			.to = to,
			.duration = duration,
			.updateTimeout = std::clamp(updateTimeout, 
				MinimumAnimationUpdateTimeout(),
				MaximumAnimationUpdateTimeout())
		};

		_animationManager.start(this);
	}

	void Animation::start(const OpacityAnimation& animation) {
		Expects(animation.duration != 0);
		Expects(animation.from != animation.to);

		_animationStart = Time::now();	
		_opacity = animation.from;

		_opacityAnimation = OpacityAnimation{
			.from = animation.from,
			.to = animation.to,
			.duration = animation.duration,
			.updateTimeout = std::clamp(animation.updateTimeout,
				MinimumAnimationUpdateTimeout(),
				MaximumAnimationUpdateTimeout())
		};

		_animationManager.start(this);
	}


	void Animation::stop() {
		_animationManager.stop();
	}

	void Animation::restart() {
		if (animating() == false)
			return start(_opacityAnimation);

		stop();
		start(_opacityAnimation);
	}

	void Animation::restartAfterFinished() {
		concurrent::invokeAsync([=] {
			while (_animationManager.animating());
			restart();
		});
	}

	bool Animation::animating() const noexcept {
		return _animationManager.animating();
	}
} // namespace base::qt::ui::animations