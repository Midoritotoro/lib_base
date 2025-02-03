#include <base/Utility.h>
#include <base/qt/common/Timer.h>

#include <base/qt/ui/FlatLabel.h>
#include <base/qt/common/MainQueueProcessor.h>

#include <base/qt/style/StyleScale.h>

#include <QApplication>
#include <QWidget>

#include <base/images/ImagesIntegral.h>
#include <base/images/ImagesPrepare.h>

#include <stdio.h>
#include <corecrt.h>


int main(int argc, char* argv[]) {
	QApplication application(argc, argv);
	auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	setlocale(LC_ALL, "");

	//const auto ratio = application.devicePixelRatio();
	//const auto useRatio = std::clamp(qCeil(ratio), 1, 3);

	//base::qt::style::SetDevicePixelRatio(useRatio);

	//base::qt::style::SetCustomFont(u"OpenSans-Regular"_q);
	//base::qt::style::internal::StartFonts();

	//base::qt::style::RegisterStyles();

	//base::qt::style::Start();

	//QSize _targetSize(1920, 1080);
 //   auto result = QImage(_targetSize, QImage::Format_ARGB32_Premultiplied);
 //   result = base::images::Prepare(std::move(result), _targetSize);

 //   result = result.scaled(
 //       result.width() * base::qt::style::DevicePixelRatio(),
 //       result.height() * base::qt::style::DevicePixelRatio(),
 //       Qt::IgnoreAspectRatio,
 //       Qt::SmoothTransformation);

 //   auto font = QFont("Arial", 8);

 //   auto painter = QPainter(&result);
 //   painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing | QPainter::Antialiasing);

 //   painter.fillRect(result.rect(), Qt::black);

  //  auto integralOccupancy = IntegralOccupancyMap(result.size());
  //  auto integralImage = IntegralImage(result);

  //  integralOccupancy.update(integralImage, QPoint(50, 50));

	/*QWidget widget;

	widget.resize(1280, 720);
	widget.setMouseTracking(true);

	base::qt::ui::FlatLabel label(&widget);

	label.setText("Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст Длинный текст *Длинный текст* ```Длинный текст```");
	widget.show();
	label.move((widget.width() - label.width()) / 2,
		(widget.height() - label.height()) / 2);*/

	base::images::IntegralImage image;
	

	return application.exec();
}