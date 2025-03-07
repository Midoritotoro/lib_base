#pragma once

#include <atomic>

#include <QApplication>
#include <QTimer>

#include <QWidget>
#include <QWidgetList>

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include <base/core/BaseNamespace.h>


__BASE_QT_UI_NAMESPACE_BEGIN

class WidgetsHider : public QObject
{
    Q_OBJECT
public:
    using DurationT = int;

    explicit inline WidgetsHider(bool showAnimation, bool hideAnimation, const QWidgetList& qWidgetList = QWidgetList{})
        : _inactivityDuration{ DurationT{4000} }
        , _qWidgetList(qWidgetList)
        , _showAnimation(showAnimation)
        , _hideAnimation(hideAnimation)
        , _animationDuration(DurationT{ 2000 })
    {
        QCoreApplication::instance()->installEventFilter(this);
        _timer.setSingleShot(true);
        _timer.callOnTimeout(
            [this]() {
                ControlVisibility(false);
            }
        );
        if (hideAnimation) {
            _animationTimer.setSingleShot(true);
            _animationTimer.callOnTimeout(
                [this]() {
                    ControlAnimationVisibility(false);
                });
        }
    }

    inline void SetInactivityDuration(DurationT msecs) {
        _inactivityDuration = msecs;
    }

    inline void SetAnimationDuration(DurationT msecs) {
        _animationDuration = msecs;
    }

    inline void removeWidget(QWidget* widget) {
        const auto index = _qWidgetList.indexOf(widget);
        if (index != -1)
            _qWidgetList.remove(index);
    }

    inline void addWidget(QWidget* widget) {
        const auto index = _qWidgetList.indexOf(widget);
        if (index == -1)
            _qWidgetList.append(widget);
    }

    inline void resetTimer() {
        ControlVisibility(true);

        if (_inactivityDuration != DurationT{})
            _timer.start(_inactivityDuration);

        if (_showAnimation || _hideAnimation) {
            ControlAnimationVisibility(true);
            if (_inactivityDuration != DurationT{})
                _animationTimer.start(_animationDuration);
        }
    }

Q_SIGNALS:
    void widgetsHidden();
    void widgetsShowed();
private:
    std::atomic<DurationT> _inactivityDuration, _animationDuration;
    QTimer _timer, _animationTimer;
    QWidgetList _qWidgetList;
    bool _showAnimation, _hideAnimation;

    inline bool eventFilter(QObject* pWatched, QEvent* pEvent) override {
        if (pEvent->type() == QEvent::MouseMove
            || pEvent->type() == QEvent::MouseButtonPress) {

            resetTimer();
        }

        return QObject::eventFilter(pWatched, pEvent);
    }

    void showAnimation(QWidget* widget) {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
        widget->setGraphicsEffect(effect);

        QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(effect, "opacity");

        fadeInAnimation->setDuration(_animationDuration);
        fadeInAnimation->setStartValue(0);
        fadeInAnimation->setEndValue(0.9);

        fadeInAnimation->setEasingCurve(QEasingCurve::InBack);
        fadeInAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    }

    void hideAnimation(QWidget* widget) {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
        widget->setGraphicsEffect(effect);

        QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(effect, "opacity");

        fadeOutAnimation->setDuration(_animationDuration);
        fadeOutAnimation->setStartValue(0.9);
        fadeOutAnimation->setEndValue(0);

        fadeOutAnimation->setEasingCurve(QEasingCurve::InBack);
        fadeOutAnimation->start(QPropertyAnimation::DeleteWhenStopped);

    }

    inline void ControlAnimationVisibility(bool Show) {
        foreach(QWidget * widget, _qWidgetList)
            widget->setVisible(Show);
        Show ? emit widgetsShowed() : emit widgetsHidden();
    }

    inline void ControlVisibility(bool Show) {
        foreach(QWidget * widget, _qWidgetList) {
            if (Show)
                if (_hideAnimation)
                    hideAnimation(widget);
                else
                    widget->setVisible(Show);
            else
                if (_showAnimation)
                    showAnimation(widget);
                else
                    widget->setVisible(Show);
        }

        if (!_hideAnimation && !_showAnimation)
            Show ? emit widgetsShowed() : emit widgetsHidden();
    }
};

__BASE_QT_UI_NAMESPACE_END
