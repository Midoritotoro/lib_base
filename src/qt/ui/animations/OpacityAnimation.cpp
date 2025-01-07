#include <base/qt/ui/animations/OpacityAnimation.h>
#include <base/Assert.h>

#include <base/Concurrent.h>


namespace base::qt::ui::animations {
	OpacityAnimation::OpacityAnimation() {
		_animationType = AnimationType::Opacity;
		_animationManager = new AnimationManager();
	}

	void OpacityAnimation::start(
		float from,
		float to,
		Time::time_t duration,
		Time::time_t updateTimeout) 
	{
		Expects(duration != 0);
		Expects(from != to);
		_opacity = from;

		_opacityFrom = from;
		_opacityTo = to;

		_duration = duration;
		_updateTimeout = std::clamp(updateTimeout,
			MinimumAnimationUpdateTimeout(),
			MaximumAnimationUpdateTimeout());

		_animationManager->start(this);
	}

	void OpacityAnimation::stop() {
		_animationManager->stop();
	}

	void OpacityAnimation::restart() {
		_opacity = _opacityFrom;

		if (animating() == false)
			return _animationManager->start(this);

		stop();
		_animationManager->start(this);
	}

	void OpacityAnimation::restartAfterFinished() {
		concurrent::invokeAsync([=] {
			while (animating());
			restart();
		});
	}

	bool OpacityAnimation::animating() const noexcept {
		return _animationManager->animating();
	}
} // namespace base::qt::ui::animations