#pragma once

#include <base/qt/common/Timer.h>

#include <base/qt/style/StyleFont.h>
#include <base/qt/style/StyleWidgets.h>

#include <base/qt/ui/FlatButton.h>
#include <base/qt/ui/animations/OpacityAnimation.h>
#include <base/qt/ui/animations/VerticalGrowthAnimation.h>

#include <base/qt/ui/animations/CombinedGrowthAnimation.h>

#include <QWidget>
#include <QAction>


namespace base::qt::ui {
	class PopupMenu : public QWidget {
		Q_OBJECT
	public:
		using Action = FlatButton;
		using Actions = std::vector<Action*>;

		PopupMenu(
			QWidget* parent = nullptr,
			const style::PopupMenu* menuStyle = style::defaultPopupMenuStyle,
			const style::MenuAction* actionStyle = style::defaultActionStyle);
		~PopupMenu();

		[[nodiscard]] QSize sizeHint() const override;
		[[nodiscard]] QSize minimumSizeHint() const override;

		void addAction(
			const QString& title,
			Fn<void()> callback,
			const style::icon* icon = nullptr);

		[[nodiscard]] Action* action(int index) const;

		void setOpacity(float opacity);
		[[nodiscard]] float opacity() const noexcept;

		void setStyle(
			const style::PopupMenu* style,
			bool repaint = true);
		[[nodiscard]] const style::PopupMenu* style() const noexcept;

		void setDeleteOnHide(bool deleteOnHide);
		[[nodiscard]] bool deleteOnHide() const noexcept;

		[[nodiscard]] bool empty() const noexcept;

		void popup(const QPoint& point);
		void addSeparator();
	protected:
		void paintEvent(QPaintEvent* event) override;
		void hideEvent(QHideEvent* event) override;

		void focusOutEvent(QFocusEvent* event) override;
		bool event(QEvent* _event) override;

		void mousePressEvent(QMouseEvent* event) override;
	private:
		void updateGeometry();

		const style::PopupMenu* _st = nullptr;
		const style::MenuAction* _actionSt = nullptr;

		Actions _actions;
		animations::CombinedGrowthAnimation _animation;

		float _opacity = 1.f;
		bool _deleteOnHide;
	};
} // namespace base::qt::ui