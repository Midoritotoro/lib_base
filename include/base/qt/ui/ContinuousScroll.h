#pragma once

#include <base/qt/ui/ScrollArea.h>


__BASE_QT_UI_NAMESPACE_BEGIN

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

__BASE_QT_UI_NAMESPACE_END
