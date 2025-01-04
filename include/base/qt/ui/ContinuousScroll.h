#pragma once

#include <base/qt/ScrollArea.h>


namespace base::qt::ui {
	class ContinuousScroll final : public ScrollArea {
		Q_OBJECT
	public:
		ContinuousScroll(QWidget* parent = nullptr);

		void setTrackingContent(bool value);
	Q_SIGNALS:
		void addContentRequest();
	protected:
		void wheelEvent(QWheelEvent* event) override;
	private:
		void reconnect();

		bool _tracking = false;
		QMetaObject::Connection _connection;
	};
} // namespace base::qt::ui