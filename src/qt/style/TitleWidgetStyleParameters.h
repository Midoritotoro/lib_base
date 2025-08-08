#pragma once 

#include <src/qt/style/BaseStyleParameters.h>

__BASE_QT_STYLE_NAMESPACE_BEGIN


// Style for widgets with a title. For example, buttons and labels. 
class TitleWidgetStyleParameters :
	public BaseStyleParameters
{
	virtual base_nodiscard font textFont() const noexcept {
		return _font;
	}

	virtual base_nodiscard int lineHeight() const noexcept {
		return _lineHeight;
	}

	virtual void setTextFont(const font& value) noexcept {
		DebugAssertLog(value, "base::qt::style::TextWidgetStyleParameters::setTextPalette: font is nullptr. ");
		_font = value;
	}

	virtual void setLineHeight(int value) noexcept {
		DebugAssertLog(value, "base::qt::style::TextWidgetStyleParameters::setTextPalette: style is nullptr. ");
		_lineHeight = value;
	}
private:
	font _font;
	int _lineHeight = 14;
};
