#include <base/core/thread/AtomicInteger.h>
#include <base/core/thread/Thread.h>

#include <base/core/io/File.h>
#include <base/core/utility/CommonMacros.h>

#include <base/core/string/String.h>
#include <base/core/memory/MemoryAllocator.h>

#include <base/core/memory/DefaultMemoryAllocatorStrategy.h>
#include <base/core/container/Vector.h>

#include <base/core/math/VectorizedAlgorithm.h>

using namespace base;

void fmax(int32* s, int32* e, int32* o) {
	sizetype l = e - s;
	int32 t = *s;
	for (sizetype i = 0; i < l; ++i)
		if (*(s + i) > t)
			t = *(s + i);
	*o = t;
}

int main(int argc, char* argv[]) {
	container::Vector<int32> _vec = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		-16, -15, -14, -13, -12, -123, 24, 8, 124, 128, 2357, 12, 99, 55, 1299, -1294,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		-16, -15, -14, -13, -12, -123, 24, 8, 124, 128, 2357, 12, 99, 55, 1299, -1294 } ;

	//std::cout << (_vec.data() + _vec.size()) - _vec.data();

	int32 max = 0;
	fmax(_vec.data(), _vec.data() + _vec.size(), &max);
	
	std::cout << "max: " << max;

//	QApplication application(argc, argv);
//	auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	//base::concurrent::invokeAsync([&]() {
	//	const auto ratio = application.devicePixelRatio();
	//	const auto useRatio = std::clamp(qCeil(ratio), 1, 3);

	//	base::qt::style::SetDevicePixelRatio(useRatio);

	//	base::qt::style::SetCustomFont(u"OpenSans-Regular"_q);
	//	base::qt::style::internal::StartFonts();

	//	base::qt::style::RegisterStyles();

	//	base::qt::style::Start();
	//});

	//// QString videoPath = "C:\\Users\\danya\\Downloads\\videotestvertical.mp4";
	//QString videoPath = "C:\\Users\\danya\\Downloads\\War Thunder - В бою 2025-01-02 22-41-32.mp4";
	//std::unique_ptr<base::media::MediaPlayer> _mediaPlayer = std::make_unique<base::media::MediaPlayer>();

	//_mediaPlayer->show();

	//_mediaPlayer->setFullScreen();

	//_mediaPlayer->setMedia(videoPath);

	return 0;
//	return application.exec();
}
