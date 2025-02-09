#pragma once

#include <QPushButton>
#include <QMouseEvent>

namespace base::qt::ui {
	class FullScreenButton : public QPushButton {
		Q_OBJECT
	public:
		enum class State {
			FullScreenTo,
			FullScreenFrom
		};

		FullScreenButton(QWidget* parent = nullptr);

		[[nodiscard]] State state() const noexcept;
	protected:
		void paintEvent(QPaintEvent* event) override;
		void mousePressEvent(QMouseEvent* event) override;
	private:
		QImage _fullScreenTo, _fullScreenFrom;
		State _state;
	};
} // namespace base::qt::ui