#pragma once

#include <base/qt/ui/animations/AnimationBase.h>


namespace base::qt::ui::animations {
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
} // namespace base::qt::ui::animations