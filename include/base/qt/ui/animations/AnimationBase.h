#pragma once 

#include <base/Time.h>
#include <base/qt/common/Size.h>

#include <base/qt/common/Types.h>


namespace base::qt::ui::animations {
	inline constexpr auto kDefaultAnimationDuration = Time::time_t(500);

	[[nodiscard]] always_inline Time::time_t MinimumAnimationUpdateTimeout() {
		return Time::time_t(1000) / common::ScreenRefreshRate();
	}

	[[nodiscard]] always_inline Time::time_t MaximumAnimationUpdateTimeout() {
		return Time::time_t(100);
	}

	enum class AnimationType : uchar {
		Opacity = 0x01,
		HorizontalGrowth = 0x02,
		VerticalGrowth = 0x04
	};

	class AnimationBase {
	public:
		void setAnimationCallback(Fn<void()> callback);
		void call();

		[[nodiscard]] AnimationType animationType() const noexcept;
		[[nodiscard]] float opacity() const noexcept;
	protected:
		Fn<void()> _animationCallback;

		Time::time_t _duration;
		Time::time_t _updateTimeout;

		float _opacity = 1.f;

		AnimationType _animationType;
		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations