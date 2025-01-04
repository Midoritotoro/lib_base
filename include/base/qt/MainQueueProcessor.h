#pragma once 

#include <QObject>


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