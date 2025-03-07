#pragma once 

#include <base/qt/ui/animations/AnimationBase.h>
#include <base/qt/common/Timer.h>


__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

class CombinedGrowthAnimation;
class HorizontalGrowthAnimation;
class VerticalGrowthAnimation;
class OpacityAnimation;

class AnimationManager {
public:
	AnimationManager() = default;
	AnimationManager& operator=(const AnimationManager& other) = default;

	void start(not_null<AnimationBase*> animation);
	void stop();

	[[nodiscard]] bool animating() const noexcept;
private:
	void update();
	void adjustCombined(not_null<CombinedGrowthAnimation*> animation);

	union {
		CombinedGrowthAnimation* _combined;
		HorizontalGrowthAnimation* _horizontal;
		VerticalGrowthAnimation* _vertical;
		OpacityAnimation* _opacity;
	} _animation;

	union {
		float _opacity;
		double _rect;
		struct {
			double _vertical;
			double _horizontal;
		} _combined;
	} _step;

	AnimationType _currentAnimationType;
	common::Timer _timer;

	int _iterations;
};

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END
