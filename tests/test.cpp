//#include <QApplication>
//#include <QWidget>

#include <base/images/ImagesIntegral.h>
#include <base/images/ImagesPrepare.h>

#include <base/core/AtomicInteger.h>
#include <base/core/Thread.h>

#include <stdio.h>
#include <corecrt.h>

#include <base/io/File.h>
#include <thread>


#define MAX_ITER LONG_MAX

void worker(sizetype start = 0) {
	for (sizetype i = start; i < MAX_ITER; ++i) {
		printf("Iter: %i", i);
	}
}

void wk(int a, int* r) {
	*r = a * 2;
}

int main(int argc, char* argv[]) {
	//QApplication application(argc, argv);
	//auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	int res;

	base::AtomicInteger atomicInt;

	std::thread r(&worker, MAX_ITER - 543);
	std::thread rk(&wk, 4, &res);

	r.join();

	base::Thread thread;
	


	return 0;

	//const auto ratio = application.devicePixelRatio();
	//const auto useRatio = std::clamp(qCeil(ratio), 1, 3);

	//base::qt::style::SetDevicePixelRatio(useRatio);

	//base::qt::style::SetCustomFont(u"OpenSans-Regular"_q);
	//base::qt::style::internal::StartFonts();

	//base::qt::style::RegisterStyles();

	//base::qt::style::Start();

	/*QWidget widget;

	widget.resize(1280, 720);
	widget.setMouseTracking(true);

	base::qt::ui::FlatLabel label(&widget);

	label.setText("Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст *Длинный текст* ```Длинный текст```");
	widget.show();
	label.move((widget.width() - label.width()) / 2,
		(widget.height() - label.height()) / 2);*/

//	return application.exec();
}