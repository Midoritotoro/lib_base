#pragma once

#include <base/qt/style/StyleCore.h>
#include <base/core/Types.h>

#include <base/qt/ui/BaseWidget.h>

#include <QAbstractButton>
#include <QStyleOptionComplex>

#include <QGuiApplication>


__BASE_QT_STYLE_NAMESPACE_BEGIN

namespace flatButton {
    inline constexpr auto borderWidth = 2;
    inline constexpr auto _spacing = 8;

    inline constexpr auto menuButtonAngle = qDegreesToRadians(qreal(42));
    inline constexpr auto badgeAngle = menuButtonAngle;

    inline constexpr QMargins borderMargins = { 
		borderWidth, borderWidth, borderWidth, borderWidth
    };

    inline constexpr QMargins buttonMargins = { 4, 8, 4, 4 };
    inline constexpr QMargins iconMargins = { 8, 8, 8, 8 };

    inline constexpr QSize badgeSize = { 12, 12 };
    inline constexpr QSize menuButtonSize = { 14, 14 };

      QFont badgeFont();

      QString bageText(qint64 value);
      QString bageText(quint64 value);

      QString bageText(const QString& value);
      QString bageText(const QChar value);

      bool hasBadge(const QVariant& value);

	  constexpr int spacing() noexcept;
	  const int spacing(const QSize& textSize) noexcept;

	  QSize buttonMinSize(Qt::ToolButtonStyle buttonStyle) noexcept;
       
	  QColor buttonColor(const QPalette& palette, QStyle::State state) noexcept;
	  QColor iconColor(QStyle::State state);
} // namespace flatButton

__BASE_QT_STYLE_NAMESPACE_END

__BASE_QT_UI_NAMESPACE_BEGIN

class AbstractFlatButton : 
	public BaseWidget<
		QAbstractButton,
		style::FlatButton> 
{
public:
	AbstractFlatButton(QWidget* parent = nullptr);

	void setFitToText(bool fit);
	  bool fitToText();

	void setText(const QString& text);
	  QString text() const noexcept;

	void setStyle(
		const SelfStyle* style,
		bool repaint = true) override;

	void setOpacity(float opacity);
	  float opacity() const noexcept;

	void setIconSize(const QSize& size);
	void setIcon(const QIcon& _icon);

	void setBadgeValue(const QVariant& value);
	  QVariant badgeValue() const;

	void setAutoRaise(bool on);
	  bool autoRaise() const;

	  QSize sizeHint() const override;
	  QSize minimumSizeHint() const override;

	void setToolButtonStyle(Qt::ToolButtonStyle style);
	  Qt::ToolButtonStyle buttonStyle() const noexcept;

	  bool hasIcon() const noexcept;
	  bool hasText() const noexcept;
protected:
	bool event(QEvent* _event) override = 0;
	void mousePressEvent(QMouseEvent* _event) override = 0;
	void paintEvent(QPaintEvent*) override = 0;

	virtual void drawBackground(
		QPainter& painter,
		const QStyleOptionComplex& option,
		const QRect& iconRect) = 0;
	virtual void drawBadge(
		QPainter& painter,
		const QStyleOptionComplex& option) = 0;

	virtual void drawIcon(
		QPainter& painter,
		const QIcon& icon,
		const QStyleOption& option) = 0;

	virtual void updateHoverControl(const QPoint& pos) = 0;
	virtual void drawText(
		QPainter& painter,
		const QStyleOptionComplex& option,
		const QString& text) = 0;

	  QRect iconRect() const noexcept;
	  QRect textRect(const QRect& iconRect) const noexcept;
	  QRect badgeRect(const QRect& iconRect) const noexcept;
private:
	QString _text;

	mutable QSize _sizeHint;

	QVariant _badgeValue;

	QMenu* _menu = nullptr;
	Qt::ToolButtonStyle _buttonStyle;

	bool _autoRaise;
	bool _fitToText;

	float _opacity = 1.f;
};

__BASE_QT_UI_NAMESPACE_END
