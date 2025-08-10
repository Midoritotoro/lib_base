#include <base/qt/common/Timer.h>


__BASE_QT_COMMON_NAMESPACE_BEGIN

namespace {
	  QObject* TimersAdjuster() {
		static QObject adjuster;
		return &adjuster;
	}
} // namespace

Timer::Timer(
	not_null<QThread*> thread,
	Fn<void()> callback)
	: Timer(std::move(callback)) {
	moveToThread(thread);
}

Timer::Timer(Fn<void()> callback)
	: QObject(nullptr)
	, _callback(std::move(callback))
	, _type(Qt::PreciseTimer) {
	setRepeat(Repeat::Interval);
	connect(
		TimersAdjuster(),
		&QObject::destroyed,
		this,
		[this] { adjust(); },
		Qt::QueuedConnection);
}

Qt::TimerType Timer::DefaultType(Time::time_t timeout) {
	constexpr auto kThreshold = Time::time_t(240);
	return (timeout > kThreshold) ? Qt::CoarseTimer : Qt::PreciseTimer;
}

void Timer::setCallback(Fn<void()> callback) {
	_callback = std::move(callback);
}

void Timer::callOnce(Time::time_t timeout) {
	callOnce(timeout, DefaultType(timeout));
}

void Timer::callOnce(Time::time_t timeout, Qt::TimerType type) {
	start(timeout, type, Repeat::SingleShot);
}

void Timer::callEach(Time::time_t timeout) {
	callEach(timeout, DefaultType(timeout));
}

void Timer::callEach(Time::time_t timeout, Qt::TimerType type) {
	start(timeout, type, Repeat::Interval);
}

bool Timer::isActive() const noexcept {
	return (_timerId != 0);
}

Time::time_t Timer::remainingTime() const noexcept {
	if (!isActive()) {
		return -1;
	}
	const auto now = Time::now();
	return (_next > now) ? (_next - now) : Time::time_t(0);
}

void Timer::setRepeat(Timer::Repeat repeat) {
	_repeat = static_cast<unsigned>(repeat);
}

Timer::Repeat Timer::repeat() const {
	return static_cast<Timer::Repeat>(_repeat);
}

void Timer::start(Time::time_t timeout, Qt::TimerType type, Repeat repeat) {
	cancel();

	_type = type;
	setRepeat(repeat);
	_adjusted = false;
	setTimeout(timeout);
	_timerId = startTimer(_timeout, _type);
	if (_timerId) {
		_next = Time::now() + _timeout;
	}
	else {
		_next = 0;
	}
}

void Timer::cancel() {
	if (isActive()) {
		killTimer(std::exchange(_timerId, {} ));
	}
}


void Timer::Adjust() {
	QObject emitter;
	connect(
		&emitter,
		&QObject::destroyed,
		TimersAdjuster(),
		&QObject::destroyed);
}

void Timer::adjust() {
	auto remaining = remainingTime();
	if (remaining >= 0) {
		cancel();
		_timerId = startTimer(remaining, _type);
		_adjusted = true;
	}
}

void Timer::setTimeout(Time::time_t timeout) {
	if (timeout <= 0 || timeout >= std::numeric_limits<int>::max())
		return;

	_timeout = static_cast<unsigned int>(timeout);
}

int Timer::timeout() const {
	return _timeout;
}

void Timer::timerEvent(QTimerEvent* e) {
	if (repeat() == Repeat::Interval) {
		if (_adjusted) {
			start(_timeout, _type, repeat());
		}
		else {
			_next = Time::now() + _timeout;
		}
	}
	else {
		cancel();
	}

	if (const auto onstack = _callback) {
		onstack();
	}
}

__BASE_QT_COMMON_NAMESPACE_END
