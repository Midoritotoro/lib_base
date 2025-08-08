#pragma once 

#include <base/qt/ui/animations/AnimationManager.h>


__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

class OpacityAnimation : public AnimationBase {
public: 
	OpacityAnimation();
	OpacityAnimation& operator=(const OpacityAnimation& other) = default;

	void start(
		float from,
		float to,
		Time::time_t duration = kDefaultAnimationDuration,
		Time::time_t updateTimeout = MinimumAnimationUpdateTimeout());

	void stop();

	void restart();
	void restartAfterFinished();

	base_nodiscard bool animating() const noexcept;
private: 
	float _opacityFrom = 1.f;
	float _opacityTo = 1.f;

	AnimationManager* _animationManager = nullptr;
	friend class AnimationManager;
};

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END