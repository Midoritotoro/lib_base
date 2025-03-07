#include <base/qt/style/StyleScale.h>
#include <algorithm>


__BASE_QT_STYLE_NAMESPACE_BEGIN

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

__BASE_QT_STYLE_NAMESPACE_END
