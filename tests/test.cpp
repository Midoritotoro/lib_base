#include <base/Utility.h>
#include <base/qt/common/Timer.h>

#include <QApplication>

int main(int argc, char* argv[]) {
	QApplication application(argc, argv);

	base::qt::common::Timer timer;
	timer.setCallback([=] { printf("printed with timeout "); });
	timer.callOnce(10000);

	return application.exec();
}