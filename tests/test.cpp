#include <base/Utility.h>
#include <base/qt/common/Timer.h>

#include <base/qt/ui/FlatLabel.h>
#include <base/qt/common/MainQueueProcessor.h>

#include <QApplication>
#include <QWidget>


int main(int argc, char* argv[]) {
	QApplication application(argc, argv);

	auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	setlocale(LC_ALL, "");

	const auto ratio = application.devicePixelRatio();
	const auto useRatio = std::clamp(qCeil(ratio), 1, 3);

	base::qt::style::SetDevicePixelRatio(useRatio);

	base::qt::style::SetCustomFont(u"OpenSans-Regular"_q);
	base::qt::style::internal::StartFonts();

	base::qt::style::RegisterStyles();

	base::qt::style::Start();


	QWidget widget;
	widget.resize(1280, 720);
	widget.setMouseTracking(true);
	base::qt::ui::FlatLabel label(&widget);


	label.setText("Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст *Длинный текст* ```Длинный текст```");
	widget.show();
	label.move((widget.width() - label.width()) / 2,
		(widget.height() - label.height()) / 2);
	return application.exec();
}