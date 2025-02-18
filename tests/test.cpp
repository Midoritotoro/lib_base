//#include <QApplication>
//#include <QWidget>

#include <base/images/ImagesIntegral.h>
#include <base/images/ImagesPrepare.h>

#include <stdio.h>
#include <corecrt.h>

#include <base/io/File.h>

int main(int argc, char* argv[]) {
	//QApplication application(argc, argv);
	//auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	setlocale(LC_ALL, "");

	base::io::File file;

	if (file.open("C:/Users/danya/Downloads/raidvid.mp4", "rb") == false)
		return -1;

	base::io::ReadResult data = file.readAll();
	base::io::File::write("D:/raidvid.mp4", data.data, data.sizeInBytes, "wb");
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