#include <base/qt/ui/PopupMenu.h>
#include <base/qt/style/StyleCore.h>

#include <base/core/async/CommonAsync.h>
#include <base/qt/common/MainQueueProcessor.h>

int main(int argc, char** argv) {
	QApplication application(argc, argv);
	auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	base::concurrent::invokeAsync([&]() {
		const auto ratio = application.devicePixelRatio();
		const auto useRatio = std::clamp(qCeil(ratio), 1, 3);

		base::qt::style::SetDevicePixelRatio(useRatio);

		base::qt::style::SetCustomFont("OpenSans-Regular");
		base::qt::style::internal::StartFonts();

		base::qt::style::RegisterStyles();
		base::qt::style::Start();
	});

	QWidget widget;
	widget.resize(1280, 720);

	base::qt::ui::PopupMenu* menu = new base::qt::ui::PopupMenu(&widget);
	menu->move(widget.rect().center());
	menu->show();

	menu->addAction("Action", [] { qDebug() << "action Clicked"; });
	menu->addAction("Action2", [] { qDebug() << "action2 Clicked"; });
	menu->addAction("Action3", [] { qDebug() << "action3 Clicked"; });
	menu->addAction("Action4", [] { qDebug() << "action4 Clicked"; });

	widget.show();

	return application.exec();
}