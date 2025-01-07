#pragma once 

#include <base/qt/ui/animations/AnimationManager.h>
#include <QFlags>


namespace base::qt::ui::animations {
	enum class DirectionFlag : uint8 {
		LeftToRight = 0x01,
		RightToLeft = 0x02,
		TopToBottom = 0x04,
		BottomToTop = 0x08
	};

	Q_DECLARE_FLAGS(Direction, DirectionFlag)
	Q_DECLARE_OPERATORS_FOR_FLAGS(Direction)


	class CombinedGrowthAnimation : public AnimationBase {
	public:
		enum class Corner : uchar {
			LeftTop = 0x01,
			LeftBottom = 0x02,
			RightTop = 0x04,
			RightBottom = 0x08
		};

		CombinedGrowthAnimation();
		CombinedGrowthAnimation& operator=(const CombinedGrowthAnimation& other) = default;

		void start(
			QRect rect,
			Direction direction,
			Corner start = Corner::LeftTop,
			Time::time_t duration = kDefaultAnimationDuration,
			Time::time_t updateTimeout = MinimumAnimationUpdateTimeout());

		void stop();

		void restart();
		void restartAfterFinished();

		[[nodiscard]] bool animating() const noexcept;
		[[nodiscard]] QRectF rect() const noexcept;
	private:
		mutable QRectF _rect;

		Direction _direction;
		Corner _startCorner;

		AnimationManager* _animationManager = nullptr;
		friend class AnimationManager;
	};
} // namespace base::qt::ui::animations