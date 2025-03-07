#include <base/qt/ui/EnhancedSlider.h>

#include <QPainter>

#include <QStyleOptionSlider>
#include <QStylePainter>


__BASE_QT_UI_NAMESPACE_BEGIN

EnhancedSlider::EnhancedSlider(
    QWidget* parent,
    Qt::Orientation orientation,
    uint16_t handleLen
) :
    QSlider(parent)
    , m_HandleLen(handleLen)
{
    setContentsMargins(0, 0, 0, 0);
    setOrientation(orientation);

    setTickPosition(QSlider::TicksBelow);
    setCursor(Qt::PointingHandCursor);

    setAttribute(Qt::WA_NoSystemBackground);
}

void EnhancedSlider::setSliderValue(int val, bool bNotify) {
    setSliderPosition(val);
    if (bNotify)
        emit sliderMoved(val);
}

void EnhancedSlider::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton)
        return;
    setSliderDown(true);
    int64_t pos = mousePositionToSliderVal(event->pos());
    if (pos != sliderPosition()) {
        setSliderPosition(pos);
        emit sliderMoved(pos);
    }
}

void EnhancedSlider::mouseMoveEvent(QMouseEvent* event) {
    QSlider::mouseMoveEvent(event);
    bIsMove = true;
}

void EnhancedSlider::mouseReleaseEvent(QMouseEvent* event) {
    setSliderDown(false);
    if (!isEnabled() || event->button() != Qt::LeftButton) {
        bIsMove = false;
        return;
    }

    if (bIsMove) {
        QSlider::mouseReleaseEvent(event);
        bIsMove = false;
    }
    else {
        int64_t pos = mousePositionToSliderVal(event->pos());
        if (pos != sliderPosition()) {
            setSliderPosition(pos);
            emit sliderMoved(pos);
        }
    }
}

int64_t EnhancedSlider::mousePositionToSliderVal(const QPoint& pos) {
    int64_t duration = maximum() - minimum();
    int64_t sliderPos = 0;
    qreal mousePos = 0;
    uint16_t handleLen = 0;
    if (Qt::Vertical == orientation()) {
        handleLen = m_HandleLen ? m_HandleLen : width() / 2.0;
        mousePos = (qreal)(pos.y() - handleLen / 2);
        if (0 > mousePos)
            mousePos = 0;
        else if (mousePos > height() - handleLen)
            mousePos = (qreal)(height() - handleLen);
        sliderPos = maximum() - duration * (mousePos / (height() - handleLen));
    }
    else {
        handleLen = m_HandleLen ? m_HandleLen : height() / 2.0;
        mousePos = (qreal)(pos.x() - handleLen / 2);
        if (0 > mousePos)
            mousePos = 0;
        else if (pos.x() > width() - handleLen)
            mousePos = (qreal)(width() - handleLen);
        sliderPos = minimum() + duration * (mousePos / (width() - handleLen));
    }
    return sliderPos;
}

__BASE_QT_UI_NAMESPACE_END
