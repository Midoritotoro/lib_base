#pragma once

#include <QPushButton>

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>


__BASE_QT_UI_NAMESPACE_BEGIN

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
    NODISCARD State state() const noexcept;
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QImage _pause, _play, _repeat;

    State _state;
};

__BASE_QT_UI_NAMESPACE_END
