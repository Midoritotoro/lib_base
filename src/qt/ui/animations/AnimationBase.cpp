#pragma once

#include <base/qt/ui/animations/AnimationBase.h>

__BASE_QT_UI_ANIMATIONS_NAMESPACE_BEGIN

void AnimationBase::setAnimationCallback(Fn<void()> callback) {
	_animationCallback = std::move(callback);
}

void AnimationBase::call() {
	(_animationCallback != nullptr) ?
		_animationCallback()
		: unused(_animationCallback);
}

AnimationType AnimationBase::animationType() const noexcept {
	return _animationType;
}

float AnimationBase::opacity() const noexcept {
	return _opacity;
}

__BASE_QT_UI_ANIMATIONS_NAMESPACE_END
