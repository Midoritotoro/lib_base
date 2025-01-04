#pragma once

#include <QTimer>

#include <base/Time.h>
#include <base/qt/Types.h>

namespace base::qt::common {

	class Timer final : private QObject {
	public:
		explicit Timer(
			not_null<QThread*> thread,
			Fn<void()> callback = nullptr);
		explicit Timer(Fn<void()> callback = nullptr);

		static Qt::TimerType DefaultType(Time::time timeout);

		void setCallback(Fn<void()> callback);

		void callOnce(Time::time timeout);
		void callOnce(Time::time timeout, Qt::TimerType type);

		void callEach(Time::time timeout);
		void callEach(Time::time timeout, Qt::TimerType type);

		void cancel();
		static void Adjust();

		[[nodiscard]] bool isActive() const noexcept;
		[[nodiscard]] Time::time remainingTime() const noexcept;
	protected:
		void timerEvent(QTimerEvent* e) override;
	private:
		enum class Repeat : unsigned {
			Interval = 0,
			SingleShot = 1,
		};

		void start(Time::time timeout, Qt::TimerType type, Repeat repeat);
		void adjust();

		void setTimeout(Time::time timeout);
		int timeout() const;

		void setRepeat(Repeat repeat);
		Repeat repeat() const;

		Fn<void()> _callback;
		Time::time _next = 0;

		int _timeout = 0;
		int _timerId = 0;

		Qt::TimerType _type : 2;

		bool _adjusted = false;
		unsigned _repeat = 0;

	};


} // namespace base::qt::common