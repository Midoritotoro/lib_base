#pragma once 

#include <base/qt/style/StyleCore.h>
#include <base/core/utility/TypeTraits.h>

#include <base/qt/ui/Utility.h>
#include <base/core/utility/Assert.h>

#include <QSurfaceFormat>

#include <base/core/BaseNamespace.h>
#include <QWidget>


__BASE_QT_STYLE_NAMESPACE_BEGIN
struct StyleBase;
__BASE_QT_STYLE_NAMESPACE_END

__BASE_QT_UI_NAMESPACE_BEGIN

class BaseQWidgetHelper;

namespace {
	  std::vector<QPointer<QWidget>>
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

	virtual QMargins getMargins() const {
		return QMargins();
	}

	virtual int verticalMargins() const {
		return 0;
	}

	virtual int horizontalMargins() const {
		return 0;
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
		if (auto parent = _parent())
			parent->enterFromChildEvent(e, this);
			
		return leaveEventHook(e);
	}

	virtual void leaveEventHook(QEvent* e) {
		return Base::leaveEvent(e);
	}

	virtual void leaveToChildEvent(QEvent* e, QWidget* child) {
	}

	virtual void enterFromChildEvent(QEvent* e, QWidget* child) {
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
	BaseQWidgetHelper(QWidget* parent = nullptr):
		BaseWidgetHelper(parent)
	{
		[[maybe_unused]] static const auto Once = [] {
			auto format = QSurfaceFormat::defaultFormat();
			format.setSwapInterval(0);

			QSurfaceFormat::setDefaultFormat(format);

			return true;
		}();
	}

	  QRect rectNoMargins() {
		return rect().marginsRemoved(getMargins());
	};

	  QSize sizeNoMargins() {
		return rectNoMargins().size();
	}

	  int heightNoMargins() {
		return rectNoMargins().height();
	}

	  int widthNoMargins() {
		return rectNoMargins().width();
	}

	virtual   QRect visibleArea() {
		return QRect();
	}

	virtual   QRect hiddenArea() {
		return QRect();
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

	  QRect visibleArea() {
		return Parent::visibleRegion().boundingRect();
	}

	  QRect hiddenArea() {
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

	virtual   const SelfStyle* style()  {
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

__BASE_QT_UI_NAMESPACE_END