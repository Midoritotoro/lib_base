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

	struct OpacityAnimation {
		float from;
		float to;

		Time::time_t duration;
		Time::time_t updateTimeout;
	};

	struct HorizontalGrowthAnimation {
		QRect rect;

		Time::time_t duration;
		Time::time_t updateTimeout;
	};

	struct VerticalGrowthAnimation {
		QRect rect;

		Time::time_t duration;
		Time::time_t updateTimeout;
	};

	using AnimationsVariant = std::variant<OpacityAnimation,
		HorizontalGrowthAnimation,
		VerticalGrowthAnimation>;

	class AnimationBase {
	public:
		AnimationBase() = default;
		AnimationBase& operator=(const AnimationBase& other) = default;

		void setAnimationCallback(Fn<void()> callback);
		void call();

		[[nodiscard]] float opacity() const noexcept;
	protected:
		Fn<void()> _animationCallback;

		AnimationsVariant _animation;

		Time::time_t _animationStart;
		float _opacity;

		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations