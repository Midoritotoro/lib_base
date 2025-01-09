#pragma once 

#include <QWidget>
#include <base/TypeTraits.h>


namespace base::qt::style {
	struct StyleBase;
} // namespace base::qt::style

namespace base::qt::ui {
	template <typename Base>
	class BaseWidgetHelper : public Base {
	public:
		using Base::Base;

		virtual void hideChildren(bool animated = false) {}
		virtual void showChildren(bool animated = false) {}


	};

	template <typename Widget>
	using BaseParent = std::conditional_t<
		std::is_same_v<Widget, QWidget>,
		QWidget,
		BaseWidgetHelper<Widget>>;

	template <typename Widget>
	class BaseWidget {
		using Self = BaseWidget<Widget>;
		using Parent = BaseParent<Widget>;
	public:
		using Parent::Parent;

		virtual void getMargins() {
			const auto _style = style();

			return QMargins()
				? (constexpr base::struct_has_member<
					std::decay_t<decltype(_style)>, margin>) == false
				: _style->margin;
		}

		virtual void setStyle(const style::StyleBase* style) {};
		virtual [[nodiscard]] const style::StyleBase* style() {};

		virtual [[nodiscard]] QRect rectNoMargins() {
			return rect().marginsRemoved(getMargins());
		};
		virtual [[nodiscard]] QSize sizeNoMargins() {}

		virtual [[nodiscard]] int heightNoMargins() {}
		virtual [[nodiscard]] int widthNoMargins() {}

		virtual [[nodiscard]] QRect visibleArea() {}
		virtual [[nodiscard]] QRect hiddenArea() {}
	};

} // namespace base::qt::ui