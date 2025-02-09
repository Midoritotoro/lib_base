#pragma once

#include <QPushButton>

namespace base::qt::ui {
    class VideoStateWidget : public QPushButton {
        Q_OBJECT
    public:
        enum class State {
            Play,
            Pause,
            Repeat
        };

        VideoStateWidget(QWidget* parent = nullptr);

        void setState(State state);
        [[nodiscard]] State state() const noexcept;
    protected:
        void paintEvent(QPaintEvent* event) override;
    private:
        QImage _pause, _play, _repeat;

        State _state;
    };
} // namespace base::qt::ui