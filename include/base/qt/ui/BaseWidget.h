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
	class BaseQWidgetHelper;

	namespace {
		[[nodiscard]] std::vector<QPointer<QWidget>>
			GetChildWidgets(not_null<QWidget*> widget)
		{
			const auto& children = widget->children();
			auto result = std::vector<QPointer<QWidget>>();

			result.reserve(children.size());

			for (const auto child : children)
				if (child && child->isWidgetType())
					result.push_back(static_cast<QWidget*>(child));

			return result;
		}

		void ToggleChildrenVisibility(
			not_null<QWidget*> widget,
			bool visible)
		{
			for (const auto& child : GetChildWidgets(widget))
				if (child)
					child->setVisible(visible);
		}
	}

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

	protected:
		void enterEvent(QEnterEvent* e) final override {
			if (auto parent = _parent())
				parent->leaveToChildEvent(e, this);
			
			return enterEventHook(static_cast<QEnterEvent*>(e));
		}

		virtual void enterEventHook(QEnterEvent* e) {
			return Base::enterEvent(e);
		}

		void leaveEvent(QEvent* e) final override {
			if (auto parent = tparent())
				parent->enterFromChildEvent(e, this);
			
			return leaveEventHook(e);
		}

		virtual void leaveEventHook(QEvent* e) {
			return Base::leaveEvent(e);
		}
	private:
		BaseQWidgetHelper* _parent() {
			return qobject_cast<BaseQWidgetHelper*>(Base::parentWidget());
		}

		const BaseQWidgetHelper* _parent() const {
			return qobject_cast<const BaseQWidgetHelper*>(Base::parentWidget());
		}

		template <typename OtherBase>
		friend class BaseWidgetHelper;
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

		virtual [[nodiscard]] QRect visibleArea() {
			return QRect();
		}

		virtual [[nodiscard]] QRect hiddenArea() {
			return QRect();
		}

		virtual [[nodiscard]] int verticalMargins() {
			return 0;
		}

		virtual [[nodiscard]] int horizontalMargins() {
			return 0;
		}
	};

	template <typename Widget>
	using BaseParent = std::conditional_t<
		std::is_same_v<QWidget, Widget>,
		BaseQWidgetHelper,
		BaseWidgetHelper<Widget>>;

	template <
		typename Widget,
		typename St = BaseStyle<style::StyleBase>>
	class BaseWidget
		: public BaseParent<Widget>
	{
	protected:
		using Self = BaseWidget<Widget, St>;
		using Parent = BaseParent<Widget>;
		using SelfStyle = St;
	public:
		using Parent::Parent;

	
		virtual [[nodiscard]] QRect visibleArea() {
			return Parent::visibleRegion().boundingRect();
		}

		virtual [[nodiscard]] QRect hiddenArea() {
			const auto selfRect = Parent::rect();
			const auto selfVisibleRect = visibleArea();

			if (selfVisibleRect.isNull() || selfVisibleRect.contains(selfRect))
				return QRect();
			
			const auto left = std::min(selfRect.left(), selfVisibleRect.left());
			const auto top = std::min(selfRect.top(), selfVisibleRect.top());

			const auto right = std::max(selfRect.right(), selfVisibleRect.right());
			const auto bottom = std::max(selfRect.bottom(), selfVisibleRect.bottom());

			const auto hiddenRect = QRect(left, top, right - left, bottom - top);

			return hiddenRect.intersected(selfRect).intersected(selfVisibleRect.intersected(selfRect));
		}

		virtual void setStyle(
			const SelfStyle* style,
			bool repaint = false)  
		{
			_style = style;

			if (repaint)
				Parent::update();
		};

		virtual [[nodiscard]] const SelfStyle* style()  {
			return _style;
		};
	protected:
		const SelfStyle* _style = nullptr;
	};

	template <typename St = BaseStyle<style::StyleBase>>
	class CoreWidget: 
		public BaseWidget<QWidget, St> 
	{
	public:
		using SelfStyle = St;
		using BaseWidget<QWidget, SelfStyle>::BaseWidget;
	};

} // namespace base::qt::ui