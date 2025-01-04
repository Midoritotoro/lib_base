#include "StyleScale.h"

#include <algorithm>


namespace style {
	namespace {
		auto devicePixelRatioValue = 1;
		auto scaleValue = kScaleDefault;
	} // namespace

	int DevicePixelRatio() {
		return devicePixelRatioValue;
	}

	void SetDevicePixelRatio(int ratio) {
		devicePixelRatioValue = std::clamp(ratio, 1, kScaleMax / kScaleMin);
	}

	int Scale() {
		return scaleValue;
	}

	void SetScale(int scale) {
		scaleValue = scale;
	}
}