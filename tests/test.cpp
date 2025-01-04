#include <base/Utility.h>
#include <base/qt/Timer.h>

#include <QApplication>

int main(int argc, char* argv[]) {
	QApplication application(argc, argv);

	base::Timer timer;
	timer.callOnce();

	return application.exec();
}