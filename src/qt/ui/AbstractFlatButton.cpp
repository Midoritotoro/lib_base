#include <base/qt/ui/AbstractFlatButton.h>

#include <base/core/utility/Algorithm.h>
#include <base/qt/common/Size.h>

#include <cmath>

__BASE_QT_STYLE_NAMESPACE_BEGIN

namespace flatButton {
    QFont badgeFont() {
        auto font = qApp->font();
        font.setPixelSize(8);

        return font;
    }

    QString bageText(qint64 value) {
        if (value < 1)
            return QString();
        if (value > 999)
            return QString("1k");

        return QString::number(value);
    }

    QString bageText(quint64 value) {
        if (value <= 1)
            return QString{};
        if (value > 999)
            return QString("1k");
        return QString::number(value);
    }

    QString bageText(const QString& value) {
        return value.isEmpty()
            ? QString()
            : QString(value[0]);
    }

    QString bageText(const QChar value) {
        return QString{ value };
    }

    bool hasBadge(const QVariant& value)
    {
        switch (value.type()) {
        case QVariant::Brush:
        case QVariant::Color:
            return true;
        case QVariant::Int:
        case QVariant::LongLong:
            return value.toLongLong() > 0;
        case QVariant::UInt:
        case QVariant::ULongLong:
            return value.toULongLong() > 0;
        case QVariant::String:
            return !value.toString().isEmpty();
        }
        return false;
    }

    constexpr int spacing() noexcept {
        return _spacing;
    }
    const int spacing(const QSize& textSize) noexcept {
        return textSize.height() > 0
            ? spacing()
            : 0;
    }

    QSize buttonMinSize(Qt::ToolButtonStyle buttonStyle) noexcept {
        switch (buttonStyle) {
        case Qt::ToolButtonTextUnderIcon:
            return QSize(22, 44);
        case Qt::ToolButtonIconOnly:
            return QSize(22, 22);
        }

        return QSize(44, 22);
    }

    QColor buttonColor(
        const QPalette& palette,
        QStyle::State state) noexcept
    {
        const auto enabled = state & QStyle::State_Enabled;
        const auto pressed = state & QStyle::State_Sunken;

        const auto hovered = state & QStyle::State_MouseOver;
        const auto checked = state & QStyle::State_On;

        const auto colorGroup = enabled && checked
            ? QPalette::Normal
            : QPalette::Disabled;

        auto color = QColor();

        if ((enabled && pressed) || checked)
            color = palette.color(colorGroup, QPalette::Light);
        else if (enabled && hovered)
            color = palette.color(colorGroup, QPalette::Midlight).darker(120);
        else
            color = palette.color(colorGroup, QPalette::Dark).darker(150);

        color.setAlpha(enabled ? 200 : 128);
        return color;
    }

    QColor iconColor(QStyle::State state) {
        const auto enabled = state & QStyle::State_Enabled;
        const auto pressed = state & QStyle::State_Sunken;

        const auto checked = state & QStyle::State_On;

        return enabled &&
            (pressed || checked)
            ? Qt::black
            : Qt::white;
    }
} // namespace flatButton

__BASE_QT_STYLE_NAMESPACE_END

__BASE_QT_UI_NAMESPACE_BEGIN

AbstractFlatButton::AbstractFlatButton(QWidget* parent) :
    BaseWidget(parent)
{}

void AbstractFlatButton::setFitToText(bool fit) {
    _fitToText = fit;
    update();
}

bool AbstractFlatButton::fitToText() {
    return _fitToText;
}

void AbstractFlatButton::setText(const QString& text) {
    if (_text == text)
        return;

    _sizeHint = QSize();

    _text = text;
    update();
}

QString AbstractFlatButton::text() const noexcept {
    return _text;
}

void AbstractFlatButton::setStyle(
    const SelfStyle* style,
    bool repaint)
{
    if (_style == style)
        return;

    _style = style;

    if (repaint == false)
        return;

    updateGeometry();
    update();
}

void AbstractFlatButton::setOpacity(float opacity) {
    _opacity = opacity;
    update();
}

float AbstractFlatButton::opacity() const noexcept {
    return _opacity;
}

void AbstractFlatButton::setIconSize(const QSize& size) {
    if (iconSize() == size)
        return;

    _sizeHint = QSize();
    QAbstractButton::setIconSize(size);
}

void AbstractFlatButton::setIcon(const QIcon& icon) {
    QAbstractButton::setIcon(icon);

    //const auto size = iconSize();

    //setIconSize(QSize(size.width()
    //    - style::flatButton::badgeSize.width(),
    //    size.height()
    //    - style::flatButton::badgeSize.height()));
}

void AbstractFlatButton::setBadgeValue(const QVariant& value) {
    if (_badgeValue == value)
        return;

    _badgeValue = value;
    update();
}

QVariant AbstractFlatButton::badgeValue() const {
    return _badgeValue;
}

void AbstractFlatButton::setAutoRaise(bool on) {
    _autoRaise = on;
}

bool AbstractFlatButton::autoRaise() const {
    return _autoRaise;
}

QSize AbstractFlatButton::sizeHint() const {
    if (_sizeHint.isValid())
        return _sizeHint;

    const auto iconRect = QRect(QPoint(), iconSize());
    const auto textSize = base::qt::common::TextSize(text(), font());

    auto width = 0;
    auto height = 0;

    auto tempRect = iconRect;

    switch (_buttonStyle) {
    case Qt::ToolButtonIconOnly:
        tempRect |= badgeRect(iconRect);

        width = tempRect.width();
        height = tempRect.height();

        break;

    case Qt::ToolButtonTextOnly:
        width = textSize.width() + style::flatButton::spacing(textSize) + (
            style::flatButton::iconMargins.left()
            + style::flatButton::iconMargins.right()) / 2.;

        height = textSize.height() + style::flatButton::badgeSize.height() / 2.;
        break;

    case Qt::ToolButtonTextUnderIcon:
        tempRect |= badgeRect(iconRect);
        width = std::max(textSize.width(), tempRect.width());

        height = tempRect.height() + style::flatButton::spacing(textSize) + textSize.height();
        break;

    case Qt::ToolButtonTextBesideIcon:
        width = iconRect.width() + 
            style::flatButton::spacing(textSize) + textSize.width();

        height = std::max(iconRect.height(), textSize.height())
            + style::flatButton::badgeSize.height() / 2;
        break;
    }

    const auto margins = contentsMargins();

    width += margins.left() + margins.right();
    height += margins.top() + margins.bottom();

    _sizeHint = QSize(width, height);
    _sizeHint = _sizeHint.expandedTo(style::flatButton::buttonMinSize(_buttonStyle));

    return _sizeHint;
}

QSize AbstractFlatButton::minimumSizeHint() const {
    return sizeHint();
}

void AbstractFlatButton::setToolButtonStyle(Qt::ToolButtonStyle style) {
    _buttonStyle = style;
}

Qt::ToolButtonStyle AbstractFlatButton::buttonStyle() const noexcept {
    return _buttonStyle;
}

bool AbstractFlatButton::hasIcon() const noexcept {
    return icon().isNull() == false 
        && iconSize().isNull() == false;
}

bool AbstractFlatButton::hasText() const noexcept {
    return _text.isEmpty() == false;
}

QRect AbstractFlatButton::iconRect() const noexcept
{
    const auto margins = contentsMargins();

    switch (_buttonStyle) {
    case Qt::ToolButtonIconOnly:
    case Qt::ToolButtonTextUnderIcon:
        return QRect(QPoint{}, iconSize()).translated(
            width() / 2 - iconSize().width() / 2, margins.top());
    case Qt::ToolButtonTextBesideIcon:
    default:
        return QRect(QPoint{}, iconSize()).
            translated(margins.left(), margins.top());
    }

    return QRect();
}

QRect AbstractFlatButton::textRect(const QRect& iconRect) const noexcept {
    if (_buttonStyle == Qt::ToolButtonIconOnly)
        return QRect{};

    const auto textSize = base::qt::common::TextSize(text(), font());
    auto textRect = QRect(QPoint(), textSize);

    switch (_buttonStyle) {
    case Qt::ToolButtonTextOnly:
        textRect.moveCenter(rect().center());
        break;
    case Qt::ToolButtonTextBesideIcon:
        textRect.moveLeft(iconRect.right() + style::flatButton::spacing());
        textRect.moveTop(height() / 2 - textSize.height() / 2);
        break;
    case Qt::ToolButtonTextUnderIcon:
        textRect.moveLeft(iconRect.center().x() - textRect.width() / 2);
        textRect.moveTop(iconRect.bottom() + style::flatButton::spacing() + 1);
        break;
    default:
        return QRect();
    }

    return textRect;
}

QRect AbstractFlatButton::badgeRect(const QRect& iconRect) const noexcept {
    auto badgeRect = QRect(QPoint(), style::flatButton::badgeSize);

    if (_buttonStyle == Qt::ToolButtonTextOnly) {
        badgeRect.moveTopRight(rect().topRight() + QPoint(-1, 0));
        return badgeRect;
    }

    const int _rect = iconRect.width() / 2 + style::flatButton::borderWidth;
    const auto center = QPoint(
        _rect * qFastCos(style::flatButton::badgeAngle),
        -_rect * qFastSin(style::flatButton::badgeAngle));

    badgeRect.moveCenter(iconRect.center() + center);
    return badgeRect;
}

__BASE_QT_UI_NAMESPACE_END
