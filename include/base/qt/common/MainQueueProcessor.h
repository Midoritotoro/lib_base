#pragma once 

#include <QObject>
#include <base/core/BaseNamespace.h>


__BASE_QT_COMMON_NAMESPACE_BEGIN

class MainQueueProcessor : public QObject {
public:
	MainQueueProcessor();
	~MainQueueProcessor();

protected:
	bool event(QEvent* event) override;

private:
	void acquire();
	void release();
};

__BASE_QT_COMMON_NAMESPACE_END
