#pragma once

#include <base/qt/common/Timer.h>

#include <base/qt/ui/FlatButton.h>

#include <base/qt/ui/animations/OpacityAnimation.h>
#include <base/qt/ui/animations/CombinedGrowthAnimation.h>

#include <base/qt/ui/BaseWidget.h>
#include <base/qt/ui/effects/BlurBehind.h>

#include <QAction>


__BASE_QT_UI_NAMESPACE_BEGIN

class PopupMenu : 
	public BaseWidget<
		QWidget,
		style::PopupMenu>
{
public:
	using Action = FlatButton;
	using Actions = std::vector<Action*>;

	PopupMenu(
		QWidget* parent = nullptr,
		const style::PopupMenu* menuStyle = style::defaultPopupMenuStyle,
		const style::MenuAction* actionStyle = style::defaultActionStyle);
	~PopupMenu();

	  QSize sizeHint() const override;
	  QSize minimumSizeHint() const override;

	void addAction(
		const QString& title,
		Fn<void()> callback,
		const style::icon* icon = nullptr);
	  Action* action(int index) const;

	void setOpacity(float opacity);
	  float opacity() const noexcept;

	void setDeleteOnHide(bool deleteOnHide);
	  bool deleteOnHide() const noexcept;

	  bool empty() const noexcept;
	void popup(const QPoint& point);

	void hideMenu(bool animated = true);
	void addSeparator();
protected:
	void paintEvent(QPaintEvent* event) override;
	void hideEvent(QHideEvent* event) override;

	void focusOutEvent(QFocusEvent* event) override;
	bool event(QEvent* _event) override;
private:
	void updateGeometry();

	const style::MenuAction* _actionSt = nullptr;

	Actions _actions;

	animations::CombinedGrowthAnimation _animation;
	animations::OpacityAnimation _opacityAnimation;

	std::unique_ptr<effects::BlurBehindEffect> _blur = nullptr;

	float _opacity = 1.f;
	bool _deleteOnHide;
};

__BASE_QT_UI_NAMESPACE_END