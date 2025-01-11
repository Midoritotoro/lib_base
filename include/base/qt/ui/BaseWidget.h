#pragma once 

#include <base/qt/style/StyleCore.h>
#include <base/TypeTraits.h>

#include <base/qt/ui/Utility.h>
#include <base/Assert.h>

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
		style::StyleBase>;

	class BaseQWidgetHelper :
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

		virtual [[nodiscard]] int verticalMargins() {}
		virtual [[nodiscard]] int horizontalMargins() {}

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
		std::is_same_v<QWidget, Widget>,
		BaseQWidgetHelper,
		BaseWidgetHelper<Widget>>;

	template <
		typename Widget = QWidget,
		typename St = BaseStyle<style::StyleBase>>
	class BaseWidget
		: public BaseParent<Widget>
	//	Q_OBJECT
	{
	protected:
		using Self = BaseWidget<Widget>;
		using Parent = BaseParent<Widget>;
		using SelfStyle = St;
	public:
		using Parent::Parent;

		virtual QMargins getMargins() {
			Expects(_style != nullptr);
			return _style->margin;
		}

		[[nodiscard]] int verticalMargins() {
			Expects(_style != nullptr);
			return _style->margin.bottom() + _style.margin.top();
		}

		[[nodiscard]] int horizontalMargins() {
			Expects(_style != nullptr);
			return _style->margin.left() + _style.margin.right();
		}

		[[nodiscard]] QRect visibleArea() {
			return QRect()
				? (has_method<Self>,
					Self::visibleRegion>) == false
				: Self::visibleRegion();
		}

		[[nodiscard]] QRect hiddenArea() {
			const auto selfRect = Self::rect();
			const auto selfVisibleRect = visibleArea();

			if (selfVisibleRect.isNull() || selfVisibleRect.contains(selfRect))
				return QRect();
			
			const auto left = std::min(selfRect.left(), selfVisibleRect.left());
			const auto top = std::min(selfRect.top(), selfVisibleRect.top());

			const auto right = std::max(selfRect.right(), selfVisibleRect.right());
			const auto bottom = std::max(selfRect.bottom(), selfVisibleRect.bottom());

			const auto hiddenRect = QRect(left, top, right - left, bottom - top);

			return hiddenRect.intersected(selfRect)
					- selfVisibleRect.intersected(selfRect);
		}

		void setStyle(
			const SelfStyle* style,
			bool repaint = false)  
		{
			_style = style;

			if (repaint)
				update();
		};

		[[nodiscard]] const SelfStyle* style()  {
			return _style;
		};
	protected:
		const SelfStyle* _style = nullptr;
	};

	template <typename St = BaseStyle<style::StyleBase>>
	class CoreWidget: 
		public BaseWidget<QWidget> 
	{
	protected:
		using SelfStyle = St;
	public:
		using BaseWidget<QWidget>::BaseWidget;
	};

} // namespace base::qt::ui