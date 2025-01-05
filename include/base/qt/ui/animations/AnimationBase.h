#pragma once 

#include <base/Time.h>
#include <base/qt/common/Types.h>


namespace base::qt::ui::animations {
	inline constexpr auto kMinimumAnimationUpdateTimeout = Time::time_t(10);
	inline constexpr auto kMaximumAnimationUpdateTimeout = Time::time_t(100);

	enum AnimationType : uchar {
		Default
	};


	struct AnimationProgress {
		float from;
		float to;

		Time::time_t duration;
		Time::time_t updateTimeout;
	};

	class AnimationBase {
	public:
		AnimationBase() = default;
		AnimationBase& operator=(const AnimationBase& other) = default;

		void setAnimationCallback(Fn<void()> callback);
		void call();
	protected:
		Fn<void()> _animationCallback;
		AnimationProgress _animationProgress;

		Time::time_t _animationStart;
		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations