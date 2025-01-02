#pragma once

#include "Types.h"
#include <QEvent>


namespace core {
	class InvokeQueuedEvent: public QEvent {
		public:
			static auto Type() {
				static const auto Result = QEvent::Type(QEvent::registerEventType());
				return Result;
			}

			explicit InvokeQueuedEvent(Fn<void()> &&method)
				: QEvent(Type())
				, _method(std::move(method)) {
			}

			void invoke() {
				_method();
			}

		private:
			Fn<void()> _method;
	};

	template <typename Lambda>
	inline void InvokeQueued(
		const QObject *context,
		Lambda &&lambda) 
	{
		QCoreApplication::postEvent(
			const_cast<QObject*>(context),
			new core::InvokeQueuedEvent(std::forward<Lambda>(lambda)));
	}
} // namespace core