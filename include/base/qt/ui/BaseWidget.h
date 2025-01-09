#pragma once 

#include <base/qt/style/StyleCore.h>
#include <base/TypeTraits.h>

#include <base/qt/ui/Utility.h>
#include <QSurfaceFormat>

#include <QWidget>


namespace base::qt::style {
	struct StyleBase;
} // namespace base::qt::style

namespace base::qt::ui {
	template <typename Base>
	class BaseWidgetHelper : public Base {
	public:
		using Base::Base;

		virtual void hideChildren() {
			ToggleChildrenVisibility(this, false);
		}

		virtual void showChildren() {
			ToggleChildrenVisibility(this, true);
		}

		virtual QMargins getMargins() {
			return QMargins();
		}
	};


	template <typename St>
	using BaseStyle = std::conditional_t<
		std::is_base_of_v<style::StyleBase, St>,
		St,
		style::StyleBase>>;

	class BaseQWidgetHelper:
		public BaseWidgetHelper<QWidget>
	{
		Q_OBJECT
	public:
		BaseQWidgetHelper(QWidget* parent = nullptr) {
			[[maybe_unused]] static const auto Once = [] {
				qDebug() << "Once Called";

				auto format = QSurfaceFormat::defaultFormat();
				format.setSwapInterval(0);

				QSurfaceFormat::setDefaultFormat(format);

				return true;
			}();
		}

		virtual [[nodiscard]] QRect rectNoMargins() {
			return rect().marginsRemoved(getMargins());
		};

		virtual [[nodiscard]] QSize sizeNoMargins() {
			return rectNoMargins().size();
		}

		virtual [[nodiscard]] int heightNoMargins() {
			return rectNoMargins().height();
		}

		virtual [[nodiscard]] int widthNoMargins() {
			return rectNoMargins().width();
		}

		virtual [[nodiscard]] QRect visibleArea() {}
		virtual [[nodiscard]] QRect hiddenArea() {}

		virtual void setStyle(
			const style::StyleBase* style,
			bool repaint = false)
		{
			unused(style);
		};

		virtual [[nodiscard]] const style::StyleBase* style() {
			return nullptr;
		};
	};

	template <typename Widget>
	using BaseParent = std::conditional_t<
		std::is_same_v<Widget, QWidget>,
		QWidget,
		BaseWidgetHelper<Widget>>;


	template <
		typename Widget = QWidget,
		typename St = BaseStyle<style::StyleBase>>
	class BaseWidget
		: public BaseParent<Widget>
	{
		using Self = BaseWidget<Widget>;
		using Parent = BaseParent<Widget>;
		using Style = St;
	public:
		using Parent::Parent;

		virtual QMargins getMargins() {
			return QMargins()
				? ((_style != nullptr) && (constexpr has_member<
					std::decay_t<decltype(_style)>, std::decay_t<decltype(_style)>::margin)>)) == false
				: _style->margin;
		}

		[[nodiscard]] QRect visibleArea() override {
			return QRect()
				? (constexpr has_method<Self>,
					Self::visibleRegion>) == false
				: Self::visibleRegion();
		}

		[[nodiscard]] QRect hiddenArea() override {
			const auto selfRect = Self::rect();
			const auto selfVisibleRect = visibleArea();

			if (visibleRect.contains(widgetRect))
				return QRect();
			
			const auto left = std::min(selfRect.left(), selfVisibleRect.left());
			const auto top = std::min(selfRect.top(), selfVisibleRect.top());

			const auto right = std::max(selfRect.right(), selfVisibleRect.right());
			const auto bottom = std::max(selfRect.bottom(), selfVisibleRect.bottom());

			const auto hiddenRect = QRect(left, top, right - left, bottom - top);

			return hiddenRect.intersected(widgetRect) 
					- visibleRect.intersected(widgetRect);
		}

		void setStyle(
			const Style* style,
			bool repaint = false) override 
		{
			_style = style;

			if (repaint)
				update();
		};

		[[nodiscard]] const Style* style() override {
			return _style;
		};
	protected:
		const Style* _style = nullptr;
	};

} // namespace base::qt::ui