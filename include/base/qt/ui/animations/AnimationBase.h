#pragma once 

#include <base/core/system/Time.h>
#include <base/qt/common/Size.h>

#include <base/core/Types.h>


__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

inline constexpr auto kDefaultAnimationDuration = Time::time_t(500);

NODISCARD always_inline Time::time_t MinimumAnimationUpdateTimeout() {
	return Time::time_t(1000) / common::ScreenRefreshRate();
}

NODISCARD always_inline Time::time_t MaximumAnimationUpdateTimeout() {
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

	NODISCARD AnimationType animationType() const noexcept;
	NODISCARD float opacity() const noexcept;
protected:
	Fn<void()> _animationCallback;

	Time::time_t _duration;
	Time::time_t _updateTimeout;

	float _opacity = 1.f;

	AnimationType _animationType;
	friend class AnimationManager;
};

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END
