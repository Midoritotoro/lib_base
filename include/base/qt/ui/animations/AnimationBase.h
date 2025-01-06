#pragma once 

#include <base/Time.h>
#include <base/qt/common/Size.h>

#include <base/qt/common/Types.h>


namespace base::qt::ui::animations {
	inline constexpr auto kDefaultAnimationDuration = Time::time_t(1000);

	[[nodiscard]] always_inline Time::time_t MinimumAnimationUpdateTimeout() {
		return Time::time_t(1000) / common::ScreenRefreshRate();
	}

	[[nodiscard]] always_inline Time::time_t MaximumAnimationUpdateTimeout() {
		return Time::time_t(100);
	}

	enum AnimationType : uchar {
		Default
	};


	struct OpacityAnimation {
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

		[[nodiscard]] float opacity() const noexcept;
	protected:
		Fn<void()> _animationCallback;
		OpacityAnimation _animationProgress;

		Time::time_t _animationStart;
		float _currentOpacity;

		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations