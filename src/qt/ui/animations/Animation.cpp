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

	void Animation::start(const OpacityAnimation& animation) {
		Expects(animation.duration != 0);
		Expects(animation.from != animation.to);

		_animationStart = Time::now();	
		_opacity = animation.from;

		_animation = OpacityAnimation{
			.from = animation.from,
			.to = animation.to,
			.duration = animation.duration,
			.updateTimeout = std::clamp(animation.updateTimeout,
				MinimumAnimationUpdateTimeout(),
				MaximumAnimationUpdateTimeout())
		};

		_animationManager.start(this);
	}

	void Animation::start(const HorizontalGrowthAnimation& animation) {

	}

	void Animation::start(const VerticalGrowthAnimation& animation) {

	}

	void Animation::stop() {
		_animationManager.stop();
	}

	void Animation::restart() {
		/*if (animating() == false)
			return start(std::get<VerticalGrowthAnimation>(_animation));

		stop();
		start(_animation);*/
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