#pragma once

#include <base/qt/ui/animations/AnimationBase.h>


namespace base::qt::ui::animations {
	void AnimationBase::setAnimationCallback(Fn<void()> callback) {
		_animationCallback = std::move(callback);
	}

	void AnimationBase::call() {
		return _animationCallback();
	}
} // namespace base::qt::ui::animations