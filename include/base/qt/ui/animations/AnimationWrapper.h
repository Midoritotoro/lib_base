#pragma once 

#include <base/TypeWrapper.h>
#include <base/qt/ui/animations/AnimationBase.h>


namespace base::qt::ui::animations {
	using AnimationTypeWrapper = base::Wrapper <
		base::ValueToType<int, 0>, std::function<int(int, int)>
		/*base::ValueToType<OpacityAnimation, OpacityAnimation()>, std::function<int(int, int)>,
		base::ValueToType<HorizontalGrowthAnimation, HorizontalGrowthAnimation()>, std::function<int(int, int)>,
		base::ValueToType <VerticalGrowthAnimation, VerticalGrowthAnimation()> , std::function<int(int, int)>*/
	>;

	class AnimationWrapper
	{
	protected:
		std::unique_ptr<AnimationTypeWrapper> m_wrapperPtr;
	public:
		virtual ~AnimationWrapper() {};
		void initialize()
		{
			/*m_test_ptr.reset(new test);
			std::function<int(int, int)> _func2 = std::bind(&test::fn2, m_test_ptr.get(), std::placeholders::_1, std::placeholders::_2);
			std::function<int(int, int)> _func3 = std::bind(&test::fn3, m_test_ptr.get(), std::placeholders::_1, std::placeholders::_2);

			m_wrapperPtr.reset(new SuperWrapper(
				ValueToType<int, 0>(), std::move(_func2),
				ValueToType<int, 1>(), std::move(_func3)
			));*/
		}

		template <int command_id, typename ResType, typename ... Args>
		ResType callMethod(Args&& ... args)
		{
			return m_wrapperPtr->call<int, command_id>(std::forward<Args>(args)...);
		}
	};

} // namespace base::qt::ui::animations