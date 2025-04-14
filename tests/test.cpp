#include <base/core/thread/AtomicInteger.h>
#include <base/core/thread/Thread.h>

#include <base/core/io/File.h>

#ifdef LIB_BASE_ENABLE_QT
#include <base/qt/common/MainQueueProcessor.h>
#include <base/media/player/MediaPlayer.h>
#endif

#include <base/core/async/Concurrent.h>
#include <base/qt/text/TextUtility.h>

#include <base/core/utility/CommonMacros.h>

#include <base/core/string/String.h>
#include <base/core/memory/MemoryAllocator.h>
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

using iti32 = container::VectorIterator<container::Vector<int32>>;

void fvmax(iti32 _First, iti32 _Last, int32* o) {
	int32 tmp = 0;

	for (iti32 _Cur = _First; _Cur != _Last; ++_Cur) {
		if (*_Cur > tmp) {
			tmp = *_Cur;
		}
	}

	*o = tmp;
}

int main(int argc, char* argv[]) {
	container::Vector<int32> _vec = { 1, 2, 3, 4, 5 } ;

	//std::cout << (_vec.data() + _vec.size()) - _vec.data();

	
	for (int32 i = 0; i < 120; ++i)
		_vec.pushBack(i);

	std::cout << "_vec.length(): " << _vec.length() << " _vec.capacity(): " << _vec.capacity() << " _vec.unusedCapacity(): " << _vec.unusedCapacity() << '\n';
	int32 max = 0;
	fvmax(_vec.begin(), _vec.end(), &max);
	std::cout << "max: " << max;

	//QApplication application(argc, argv);
	//auto mainQueueProcessor = std::make_unique<base::qt::common::MainQueueProcessor>();

	//base::concurrent::invokeAsync([&]() {
	//	const auto ratio = application.devicePixelRatio();
	//	const auto useRatio = std::clamp(qCeil(ratio), 1, 3);

	//	base::qt::style::SetDevicePixelRatio(useRatio);

	//	base::qt::style::SetCustomFont("OpenSans-Regular");
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


	//return application.exec();
	return 0;
}
