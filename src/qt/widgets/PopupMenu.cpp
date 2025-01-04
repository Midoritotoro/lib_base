#include "PopupMenu.h"

#include <QMenu>
#include "Separator.h"


PopupMenu::PopupMenu(QWidget* parent):
	QWidget(parent)
	, _st(style::defaultPopupMenuStyle)
{
	setAttribute(Qt::WA_OpaquePaintEvent, false);

	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_NoSystemBackground);
}

QSize PopupMenu::sizeHint() const {
	return size();
}

QSize PopupMenu::minimumSizeHint() const {
	return size();
}

void PopupMenu::addAction(
	const QString& title,
	Fn<void()> callback,
	const style::icon* icon)
{
	if (callback == nullptr)
		return;

	auto action = new Action(this);

	action->setText(title);
	action->setColor(_st->colorBg);

	connect(action, &QAbstractButton::clicked, callback);
	_actions.push_back(action);
}

PopupMenu::Action* PopupMenu::action(int index) const {
	return _actions.at(index);
}

void PopupMenu::setOpacity(float opacity) {
	_opacity = opacity;
	update();
}

float PopupMenu::opacity() const noexcept {
	return _opacity;
}

void PopupMenu::setStyle(const style::PopupMenu* style, bool repaint) {
	_st = style;
	update();
}

const style::PopupMenu* PopupMenu::style() const noexcept {
	return _st;
}

bool PopupMenu::empty() const noexcept {
	return _actions.empty();
}

void PopupMenu::addSeparator() {
	const auto separator = new Separator(this,
		new style::Separator{
			.width = width() - _st->margin.left()
				- _st->margin.right(),
			.padding = { 5, 5, 5, 5 },
			.colorFg = Qt::white
		},
		_st);
}

void PopupMenu::popup(const QPoint& point) {
	/*show();
	raise();

	updateGeometry();
	move(point);*/

	qDebug() << "popup: " << pos() << size() << isHidden();

}

void PopupMenu::paintEvent(QPaintEvent* event) {
	auto painter = QPainter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	painter.setPen(Qt::NoPen);
	painter.setBrush(_st->colorBg);

	painter.setOpacity(_opacity);

	//if (const auto fill = rect().intersected(event->rect()); fill.isNull() == false)
	painter.drawRect(rect());
}

void PopupMenu::focusOutEvent(QFocusEvent* event) {

}

void PopupMenu::focusInEvent(QFocusEvent* event) {

}

bool PopupMenu::event(QEvent* _event) {
	return QWidget::event(_event);
}

void PopupMenu::updateGeometry() {
	const auto fullWidth = _st->maximumWidth
		+ _st->margin.left() 
		+ _st->margin.right();
	
	const auto fullHeight = _st->maximumHeight
		+ _st->margin.top()
		+ _st->margin.bottom();

	const auto actionHeight = (_st->maximumHeight
		/ _actions.size() != 0 ? _actions.size() : 1) 
			- _st->margin.top() 
			- _st->margin.bottom();

	resize(fullWidth, fullHeight);

	for (auto index = 0; index < _actions.size(); ++index) {
		_actions[index]->resize(_st->maximumWidth, actionHeight);
		index != 0
			? _actions[index]->move(_actions[index]->rect().bottomLeft())
			: _actions[index]->move(0, _st->margin.top());
	}
}