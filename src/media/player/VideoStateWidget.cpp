#include <base/media/player/VideoStateWidget.h>

#include <base/images/ImagesPrepare.h>
#include <base/qt/style/StyleScale.h>

#include <QPainter>


namespace base::qt::ui {

    VideoStateWidget::VideoStateWidget(QWidget* parent) :
        QPushButton(parent)
    {
        _pause = QImage("D:/lib_base/lib_base/assets/images/stop.png");
        _play = QImage("D:/lib_base/lib_base/assets/images/play.png");
        _repeat = QImage("D:/lib_base/lib_base/assets/images/repeat.png");

        setAttribute(Qt::WA_NoSystemBackground);
        setCursor(Qt::PointingHandCursor);

        setState(State::Pause);
    }

    void VideoStateWidget::setState(State state) {
        if (_state == state)
            return;

        _state = state;
        update();
    }

    VideoStateWidget::State VideoStateWidget::state() const noexcept {
        return _state;
    }

    void VideoStateWidget::paintEvent(QPaintEvent* event) {
        auto painter = QPainter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);

        auto image = QImage();

        switch (_state) {
            case State::Play:
                image = _play;
                break;

            case State::Pause:
                image = _pause;
                break;

            case State::Repeat:
                image = _repeat;
                break;
        }

        image = images::Prepare(image, size());
        image = std::move(image).scaled(
            image.width() * style::DevicePixelRatio(),
            image.height() * style::DevicePixelRatio(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation);

        image.setDevicePixelRatio(style::DevicePixelRatio());
        painter.drawImage(0, 0, image);
    }
} // namespace base::qt::ui