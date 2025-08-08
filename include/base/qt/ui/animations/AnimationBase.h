#pragma once 

#include <base/core/system/Time.h>
#include <base/qt/common/Size.h>

#include <base/core/Types.h>


__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

inline constexpr auto kDefaultAnimationDuration = Time::time_t(500);

base_nodiscard base_always_inline Time::time_t MinimumAnimationUpdateTimeout() {
	return Time::time_t(1000) / common::ScreenRefreshRate();
}

base_nodiscard base_always_inline Time::time_t MaximumAnimationUpdateTimeout() {
	return Time::time_t(100);
}

enum class AnimationType : uchar {
	Opacity = 0x01,
	HorizontalGrowth = 0x02,
	VerticalGrowth = 0x04,
	CombinedGrowth = 0x08
};

class AnimationBase {
public:
	void setAnimationCallback(Fn<void()> callback);
	void call();

	base_nodiscard AnimationType animationType() const noexcept;
	base_nodiscard float opacity() const noexcept;
protected:
	Fn<void()> _animationCallback;

	Time::time_t _duration;
	Time::time_t _updateTimeout;

	float _opacity = 1.f;

	AnimationType _animationType;
	friend class AnimationManager;
};

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END
