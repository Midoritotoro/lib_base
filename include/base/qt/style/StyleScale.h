#pragma once 

#include <xtr1common>
#include <cmath>

#include <base/core/Types.h>


__BASE_QT_STYLE_NAMESPACE_BEGIN

inline constexpr auto kScaleMin = 50;
inline constexpr auto kScaleMax = 300;
inline constexpr auto kScaleDefault = 100;

base_nodiscard int DevicePixelRatio();
void SetDevicePixelRatio(int ratio);

base_nodiscard int Scale();
void SetScale(int scale);

template <typename T>
base_nodiscard inline T ConvertScale(T value, int scale) {
	if (value < 0.)
		return -ConvertScale(-value, scale);

	const auto result = T(std::round(
		(double(value) * scale / 100.) - 0.01));

	return (!std::is_integral_v<T> || !value || result) ? result : 1;
}

template <typename T>
base_nodiscard inline T ConvertScale(T value) {
	return ConvertScale(value, Scale());
}

__BASE_QT_STYLE_NAMESPACE_END