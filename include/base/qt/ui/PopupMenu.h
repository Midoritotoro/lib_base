#pragma once

#include <base/qt/common/Timer.h>

#include <base/qt/style/StyleFont.h>
#include <base/qt/style/StyleWidgets.h>

#include <base/qt/ui/FlatButton.h>

#include <QWidget>
#include <QAction>


namespace base::qt::ui {
	class PopupMenu : public QWidget {
		Q_OBJECT
	public:
		using Action = FlatButton;
		using Actions = std::vector<Action*>;

		PopupMenu(QWidget* parent = nullptr);
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

		[[nodiscard]] bool empty() const noexcept;

		void popup(const QPoint& point);
		void addSeparator();
	protected:
		void paintEvent(QPaintEvent* event) override;

		void focusOutEvent(QFocusEvent* event) override;
		void focusInEvent(QFocusEvent* event) override;

		bool event(QEvent* _event) override;
	private:
		void updateGeometry();

		const style::PopupMenu* _st = nullptr;
		Actions _actions;

		float _opacity = 1.f;
	};
} // namespace base::qt::ui