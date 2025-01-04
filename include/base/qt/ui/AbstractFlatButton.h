#pragma once

#include <base/qt/StyleCore.h>
#include <base/qt/Types.h>

#include <QAbstractButton>
#include <QStyleOptionComplex>

#include <QGuiApplication>


namespace base::qt::style {
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

        [[nodiscard]] QFont badgeFont();

        [[nodiscard]] QString bageText(qint64 value);
        [[nodiscard]] QString bageText(quint64 value);

        [[nodiscard]] QString bageText(const QString& value);
        [[nodiscard]] QString bageText(const QChar value);

        [[nodiscard]] bool hasBadge(const QVariant& value);

		[[nodiscard]] constexpr int spacing() noexcept;
		[[nodiscard]] const int spacing(const QSize& textSize) noexcept;

		[[nodiscard]] QSize buttonMinSize(Qt::ToolButtonStyle buttonStyle) noexcept;
       
		[[nodiscard]] QColor buttonColor(const QPalette& palette, QStyle::State state) noexcept;
		[[nodiscard]] QColor iconColor(QStyle::State state);
    } // namespace flatButton
} // namespace style


namespace base::qt::ui {
	class AbstractFlatButton : public QAbstractButton {
		Q_OBJECT
	public:
		enum PopupMode {
			InstantPopup,
			MenuButtonPopup
		};

		AbstractFlatButton(QWidget* parent = nullptr);

		void setFitToText(bool fit);
		[[nodiscard]] bool fitToText();

		void setText(const QString& text);
		[[nodiscard]] QString text() const noexcept;

		void setColor(const QColor& color);
		[[nodiscard]] QColor color() const noexcept;

		void setTextColor(const QColor& color = QColor(0, 0, 0));
		[[nodiscard]] QColor textColor() const noexcept;

		void setBorderRadius(int borderRadius);
		[[nodiscard]] int borderRadius() const noexcept;

		void setIconSize(const QSize& size);
		void setIcon(const QIcon& _icon);

		void setBadgeValue(const QVariant& value);
		[[nodiscard]] QVariant badgeValue() const;

		void setButtonStyle(Qt::ToolButtonStyle style);
		[[nodiscard]] Qt::ToolButtonStyle buttonStyle() const;

		void setPopupMode(PopupMode mode);
		[[nodiscard]] PopupMode popupMode() const;

		void setAutoRaise(bool on);
		[[nodiscard]] bool autoRaise() const;

		void setMenu(not_null<QMenu*> menu);
		[[nodiscard]] QMenu* menu() const;

		[[nodiscard]] QSize sizeHint() const override;
		[[nodiscard]] QSize minimumSizeHint() const override;
	protected:
		bool event(QEvent* _event) override = 0;
		void mousePressEvent(QMouseEvent* _event) override = 0;
		void paintEvent(QPaintEvent*) override = 0;

		virtual void showMenu(not_null<QMenu*> _menu) = 0;

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

		[[nodiscard]] QRect iconRect() const noexcept;
		[[nodiscard]] QRect textRect(const QRect& iconRect) const noexcept;

		[[nodiscard]] QRect menuButtonRect(const QRect& iconRect) const noexcept;
		[[nodiscard]] QRect badgeRect(const QRect& iconRect) const noexcept;
	private:
		QColor _color;
		QColor _textColor;

		QString _text = "";

		mutable QSize _sizeHint;
		QVariant _badgeValue;

		QMenu* _menu = nullptr;
		Qt::ToolButtonStyle _buttonStyle;

		PopupMode _popupMode;

		bool _autoRaise;
		bool _fitToText;

		int _borderRadius = 0;
	};
} // namespace base::qt::ui