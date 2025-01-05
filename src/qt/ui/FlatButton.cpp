#include <base/qt/ui/FlatButton.h>

#include <QPalette>
#include <QPainter>

#include <QMenu>
#include <QPaintEvent>

#include <QColor>


namespace base::qt::ui {
    FlatButton::FlatButton(QWidget* parent) :
        AbstractFlatButton(parent)
    {
        setBorderRadius(10);
        setColor(palette().color(QPalette::Base));

        setTextColor(Qt::white);
        setPopupMode(AbstractFlatButton::InstantPopup);

        setFitToText(false);
        setButtonStyle(Qt::ToolButtonStyle::ToolButtonTextOnly);
        setIconSize({ 38, 38 });

        setAttribute(Qt::WA_Hover);
        setMouseTracking(true);

        setCheckable(false);
        setChecked(false);

        setAutoRaise(false);
        setCursor(Qt::PointingHandCursor);
    }

    void FlatButton::drawBackground(QPainter& _painter, const QStyleOptionComplex& _option, const QRect& _iconRect)
    {
        const auto checked = _option.state & QStyle::State_On;
        const auto hovered = _option.state & QStyle::State_MouseOver;

        if (size().isEmpty() || (autoRaise() && !checked && !hovered))
            return;

        auto state = _option.state;
        if (!(_option.activeSubControls & QStyle::SC_ToolButton))
            state &= ~(QStyle::State_MouseOver | QStyle::State_Sunken);

        auto buttonMask = QPixmap((QSizeF(size()) * style::DevicePixelRatio()).toSize());
        buttonMask.setDevicePixelRatio(style::DevicePixelRatio());

        buttonMask.fill(Qt::transparent);

        auto maskPainter = QPainter(&buttonMask);
        maskPainter.setRenderHint(QPainter::Antialiasing);

        maskPainter.setPen(Qt::NoPen);
        maskPainter.setBrush(style::flatButton::buttonColor(_option.palette, state));

        if (buttonStyle() == Qt::ToolButtonTextOnly) {
            const auto radius = std::min(_option.rect.width(), _option.rect.height()) / 2;
            maskPainter.drawRoundedRect(_option.rect, radius, radius);
        }
        else
            maskPainter.drawEllipse(_iconRect);

        maskPainter.setCompositionMode(QPainter::CompositionMode_Source);
        maskPainter.setBrush(Qt::transparent);

        if (style::flatButton::hasBadge(badgeValue()))
            maskPainter.drawEllipse(badgeRect(_iconRect).marginsAdded(style::flatButton::borderMargins));
        if (popupMode() != InstantPopup)
            maskPainter.drawEllipse(menuButtonRect(_iconRect).marginsAdded(style::flatButton::borderMargins));

        _painter.drawPixmap(0, 0, buttonMask);
    }

    void FlatButton::drawBadge(QPainter& _painter, const QStyleOptionComplex& _option) {
        if (!badgeValue().isValid())
            return;

        auto text = QString();
        auto brush = QBrush();

        switch (badgeValue().type()) {
        case QMetaType::Type::QColor:
            brush = badgeValue().value<QColor>();
            break;
        case QMetaType::Type::QBrush:
            brush = badgeValue().value<QBrush>();
            break;
        case QMetaType::Type::UInt:
        case QMetaType::Type::ULongLong:
            brush = _option.palette.highlight();
            text = style::flatButton::bageText(badgeValue().toULongLong());
            break;
        case QMetaType::Type::Int:
        case QMetaType::Type::LongLong:
            brush = _option.palette.highlight();
            text = style::flatButton::bageText(badgeValue().toLongLong());
            break;
        case QMetaType::Type::Char:
            brush = _option.palette.highlight();
            text = style::flatButton::bageText(badgeValue().toChar());
            break;
        case QMetaType::Type::QString:
            brush = _option.palette.highlight();
            text = style::flatButton::bageText(badgeValue().toString());
            break;
        default:
            return;
        }

        _painter.setPen(Qt::NoPen);
        _painter.setBrush(brush);
        _painter.drawEllipse(_option.rect);

        if (!text.isEmpty()) {
            _painter.setBrush(Qt::NoBrush);
            _painter.setPen(_option.palette.color(QPalette::HighlightedText));

            _painter.setFont(style::flatButton::badgeFont());
            _painter.drawText(_option.rect, Qt::AlignCenter, text);
        }
    }

    void FlatButton::drawIcon(QPainter& painter, const QIcon& icon, const QStyleOption& option)
    {
        const auto iconRect = option.rect;

        auto pixmap = QPixmap(iconRect.size());
        pixmap.fill(Qt::transparent);

        auto pixmapPainter = QPainter(&pixmap);
        icon.paint(&pixmapPainter, pixmap.rect());

        pixmapPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        pixmapPainter.fillRect(pixmap.rect(), style::flatButton::iconColor(option.state));

        painter.drawPixmap(iconRect, pixmap);
    }

    void FlatButton::updateHoverControl(const QPoint& pos) {
        const auto last = _hoverControl;
        _hoverControl = QStyle::SC_None;

        if (rect().contains(pos)) {
            _hoverControl |= QStyle::SC_ToolButton;

            if (popupMode() != InstantPopup && menuButtonRect(iconRect()).contains(pos))
                _hoverControl |= QStyle::SC_ToolButtonMenu;
        }

        if (_hoverControl != last)
            update();
    }

    void FlatButton::drawText(
        QPainter& painter,
        const QStyleOptionComplex& option,
        const QString& text)
    {
        painter.setFont(font());

       /* if (images::IsRgbNull(textColor().rgb()) == false) {
            painter.setPen(textColor());
            painter.drawText(option.rect, Qt::AlignCenter, this->text());
            return;
        }*/

        const auto inverseColor = buttonStyle() == Qt::ToolButtonTextOnly
            && (option.state & (QStyle::State_Sunken | QStyle::State_On));

        const auto pen = option.palette.color(
            inverseColor
            ? QPalette::ButtonText
            : QPalette::BrightText);

        painter.setPen(pen);
        painter.drawText(option.rect, Qt::AlignCenter, this->text());
    }

    void FlatButton::paintEvent(QPaintEvent* event) {
        const auto fill = rect().intersected(event->rect());
        auto painter = QPainter(this);

        painter.setBrush(color());
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing | QPainter::Antialiasing);

        auto option = QStyleOptionComplex();
        option.initFrom(this);

        option.palette = color();

        if (isDown())
            option.state |= QStyle::State_Sunken;
        if (isChecked())
            option.state |= QStyle::State_On;
        if (isChecked() && isDown())
            option.state |= QStyle::State_Raised;

        option.activeSubControls = _hoverControl;

        const auto state = option.state;

        const auto _iconRect = iconRect();
        const auto _textRect = textRect(_iconRect);

        if (isDown() && (option.activeSubControls & QStyle::SC_ToolButtonMenu))
            option.state &= ~(QStyle::State_Sunken);

        if (buttonStyle() == Qt::ToolButtonTextOnly) {
            fitToText()
                ? option.rect = _textRect.marginsAdded(style::flatButton::iconMargins)
                : option.rect = rect();
            drawBackground(painter, option, QRect());
        }
        else {
            drawBackground(painter, option, _iconRect);
            option.rect = _iconRect.marginsRemoved(style::flatButton::iconMargins);
            drawIcon(painter, icon(), option);
        }

        option.rect = badgeRect(_iconRect);
        drawBadge(painter, option);

        if (popupMode() == AbstractFlatButton::MenuButtonPopup) {
            option.rect = menuButtonRect(_iconRect);
            option.state &= ~QStyle::State_On;

            isDown() && (option.activeSubControls & QStyle::SC_ToolButtonMenu)
                ? option.state |= QStyle::State_Sunken
                : option.state &= ~(QStyle::State_Sunken);

            // Отрисовка меню ...
        }

        if (buttonStyle() != Qt::ToolButtonIconOnly) {
            fitToText()
                ? option.rect = _textRect
                : option.rect = rect().marginsRemoved(style::flatButton::iconMargins);

            option.state = state;

            drawText(painter, option, text());
        }
    }

    void FlatButton::showMenu(not_null<QMenu*> menu) {
        if (menu == nullptr)
            return;

        const auto menuSize = menu->sizeHint();
        const auto point = QPoint(
            width() / 2. - menuSize.width() / 2.,
            -menuSize.height());

        menu->exec(mapToGlobal(point));
    }

    bool FlatButton::event(QEvent* _event)
    {
        switch (_event->type()) {
        case QEvent::HoverLeave:
        case QEvent::HoverEnter:
        case QEvent::HoverMove:
            updateHoverControl(static_cast<const QHoverEvent*>(_event)->pos());
            break;
        case QEvent::PaletteChange:
            update();
            break;
        case QEvent::LocaleChange:
        case QEvent::StyleChange:
        case QEvent::LanguageChange:
        case QEvent::FontChange:
        default:
            break;
        }
        return QAbstractButton::event(_event);
    }

    void FlatButton::mousePressEvent(QMouseEvent* event) {
        if (event->button() != Qt::LeftButton
            && popupMode() != AbstractFlatButton::InstantPopup
            || !menuButtonRect(iconRect()).contains(event->pos())
            )
            return QAbstractButton::mousePressEvent(event);

        setDown(true);
        update();

        if (menu())
            showMenu(menu());
        else if (popupMode() == AbstractFlatButton::MenuButtonPopup)
            emit menuRequested();

        setDown(false);
        update();
    }
} // namespace base::qt::ui