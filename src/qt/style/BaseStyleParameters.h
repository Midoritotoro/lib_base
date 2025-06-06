#pragma once 

#include <base/core/utility/TypeTraits.h>

#include <base/qt/style/StyleTypes.h>
#include <base/qt/style/StyleFont.h>

#include <QPoint>

__BASE_QT_STYLE_NAMESPACE_BEGIN

class BaseStyleParameters {
public:
	virtual NODISCARD float opacity() const noexcept {
		return _opacity;
	}

	virtual NODISCARD bool hasAnimation() const noexcept {
		return _hasAnimation;
	}

	virtual NODISCARD int minimumWidth() const noexcept {
		return _minimumWidth;
	}

	virtual NODISCARD int minimumHeight() const noexcept {
		return _minimumHeight;
	}

	virtual NODISCARD int maximumWidth() const noexcept {
		return _maximumWidth;
	}

	virtual NODISCARD int maximumHeight() const noexcept {
		return _maximumHeight;
	}

	virtual NODISCARD color backgroundColor() const noexcept {
		return _backgroundColor;
	}

	virtual NODISCARD margins widgetMargins() const noexcept {
		return _margins;
	}

	virtual NODISCARD int borderRadius() const noexcept {
		return _borderRadius;
	}

	virtual NODISCARD int borderWidth() const noexcept {
		return _borderWidth;
	}

	virtual NODISCARD color borderColor() const noexcept {
		return _borderColor;
	}

	virtual void setOpacity(float value) noexcept {
		_opacity = value;
	}

	virtual void setHasAnimation(bool value) noexcept {
		_hasAnimation = value;
	}

	virtual void setMinimumWidth(int value) noexcept {
		_minimumWidth = value;
	}

	virtual void setMinimumHeight(int value) noexcept {
		_minimumHeight = value;
	}

	virtual void setMaximumWidth(int value) noexcept {
		_maximumWidth = value;
	}

	virtual void setMaximumHeight(int value) noexcept {
		_maximumHeight = value;
	}

	virtual void setBackgroundColor(const color& value) noexcept {
		_backgroundColor = value;
	}

	virtual void setWidgetMargins(const margins& value) noexcept {
		_margins = value;
	}

	virtual void setLeftMargin(int value) noexcept {
		_margins.setLeft(value);
	}

	virtual void setRightMargin(int value) noexcept {
		_margins.setRight(value);
	}

	virtual void setTopMargin(int value) noexcept {
		_margins.setTop(value);
	}

	virtual void setBottomMargin(int value) noexcept {
		_margins.setBottom(value);
	}

	virtual void setBorderWidth(int value) noexcept {
		_borderWidth = value;
	}

	virtual void setBorderRadius(int value) noexcept {
		_borderRadius = value;
	}

	virtual void setBorderColor(const color& value) noexcept {
		_borderColor = value;
	}
private:
	float _opacity = 0;
	bool _hasAnimation = false;

	int _borderWidth : 2 = 0;
	int _borderRadius : 2 = 0;

	int _minimumWidth : 2 = 0;
	int _minimumHeight : 2 = 0;

	int _maximumWidth : 2 = 0;
	int _maximumHeight : 2 = 0;

	margins _margins = { 0, 0, 0, 0 };

	color _backgroundColor = {};
	color _borderColor = {};
};

__BASE_QT_STYLE_NAMESPACE_END


