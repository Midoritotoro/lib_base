#pragma once

#include <vector>
#include <QGuiApplication>

#include <cfenv>
#include <algorithm>

#include <gsl/gsl>

#include "utility/Accumulate.h"

#include "utility/Round.h"
#include "utility/Size.h"

#include <iostream>

/**
 *После выхода из текущей области видимости определяет затраченное на выполнение этого блока кода время
 *\param name - "Имя" области видимости
*/
#define measureExecutionTime(name) \
	 const auto ms = Time::now(); \
	 const auto timer = gsl::finally([] { \
		std::cout << name << " completed for: " \
		<< Time::now() - ms << " ms" << '\n'; }); \

namespace core::utility {
	[[nodiscard]] inline bool IsCtrlPressed() {
		return (QGuiApplication::keyboardModifiers() == Qt::ControlModifier);
	}

	[[nodiscard]] inline bool IsAltPressed() {
		return (QGuiApplication::keyboardModifiers() == Qt::AltModifier);
	}

	[[nodiscard]] inline bool IsShiftPressed() {
		return (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier);
	}
} // namespace core::utility