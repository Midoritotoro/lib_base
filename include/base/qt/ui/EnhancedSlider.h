#pragma once

#include <QSlider>
#include <QLabel>

#include <QStyle>
#include <QMouseEvent>

#include <QPropertyAnimation>


namespace base::qt::ui {
    class EnhancedSlider : public QSlider
    {
    private:
        Q_OBJECT
        uint16_t m_HandleLen;
        bool bIsMove = false;
    public:
        EnhancedSlider(
            QWidget* parent = nullptr,
            Qt::Orientation orientation = Qt::Horizontal,
            uint16_t handleLen = 0);

        void setSliderValue(
            int val,
            bool bNotify = false);
    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
    private:
        [[nodiscard]] int64_t mousePositionToSliderVal(const QPoint& pos);
    };
} // namespace base::qt::ui