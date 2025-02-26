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



void worker(sizetype end, const char* name) {
	for (sizetype i = 0; i < end; ++i) {
		if ( i % 10000 == 0)
			printf(name, i);
	}
}

void wk(int a, int* r) {
	*r = a * 2;
}

void worker1() {

}


int main(int argc, char* argv[]) {
	//QApplication application(argc, argv);
	//auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	//int res = 0;

	//base::AtomicInteger atomicInt;

	//std::thread th(worker, 254);
	//th.join();

	base::Thread thread;
	thread.start(worker, 210000, "Iter thread: %lli\n");

	base::Thread other;
	other.start(worker, 210000, "Iter other: %lli\n");

	std::cout << "p";
	thread.join();
	other.join();
//	std::cout << res << '\n';

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