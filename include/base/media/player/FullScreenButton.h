#pragma once

#include <QPushButton>
#include <QMouseEvent>

#include <base/core/BaseNamespace.h>
#include <base/core/arch/KeywordSupport.h>


__BASE_QT_UI_NAMESPACE_BEGIN

class FullScreenButton : public QPushButton {
	Q_OBJECT
public:
	enum class State {
		FullScreenTo,
		FullScreenFrom
	};

	FullScreenButton(QWidget* parent = nullptr);

	NODISCARD State state() const noexcept;
protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
	QImage _fullScreenTo, _fullScreenFrom;
	State _state;
};

__BASE_QT_UI_NAMESPACE_END
