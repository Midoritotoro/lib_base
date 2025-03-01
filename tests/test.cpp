#include <QApplication>
#include <QWidget>

#include <base/core/AtomicInteger.h>
#include <base/core/Thread.h>

#include <base/core/BaseNamespace.h>

#include <stdio.h>
#include <corecrt.h>

#include <base/qt/style/StyleCore.h>
#include <base/qt/common/MainQueueProcessor.h>

#include <base/io/File.h>
#include <thread>

#include <base/qt/ui/FlatLabel.h>

#include <base/utility/CommonMacros.h>
#include <memory>

#include <base/core/Thread.h>
#include <iostream>

#include <base/media/player/MediaPlayer.h>


int main(int argc, char* argv[]) {
	QApplication application(argc, argv);
	auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	const auto ratio = application.devicePixelRatio();
	const auto useRatio = std::clamp(qCeil(ratio), 1, 3);

	base::qt::style::SetDevicePixelRatio(useRatio);

	base::qt::style::SetCustomFont(u"OpenSans-Regular"_q);
	base::qt::style::internal::StartFonts();

	base::qt::style::RegisterStyles();

	base::qt::style::Start();

	QString videoPath = "C:\\Users\\danya\\Downloads\\videotestvertical.mp4";
	std::unique_ptr<base::media::MediaPlayer> _mediaPlayer = std::make_unique<base::media::MediaPlayer>();

	_mediaPlayer->show();

	_mediaPlayer->setNormal();
	_mediaPlayer->showFullScreen();

	_mediaPlayer->setMedia(videoPath);

	return application.exec();
}
