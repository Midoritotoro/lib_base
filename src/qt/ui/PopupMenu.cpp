#include <base/qt/ui/PopupMenu.h>

#include <base/qt/ui/Separator.h>
#include <base/Utility.h>

#include <QApplication>
#include <QMouseEvent>


namespace base::qt::ui {
	PopupMenu::PopupMenu(
		QWidget* parent,
		const style::PopupMenu* menuStyle,
		const style::MenuAction* actionStyle
	) :
		QWidget(parent)
		, _st(menuStyle)
		, _actionSt(actionStyle)
	{
		setAttribute(Qt::WA_OpaquePaintEvent, false);

		setAttribute(Qt::WA_TranslucentBackground);
		setAttribute(Qt::WA_NoSystemBackground);

		setDeleteOnHide(true);

		_animation.setAnimationCallback([this] {
			update();
		});
	}

	PopupMenu::~PopupMenu() {
		for (const auto& action : base::take(_actions))
			delete action;
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
		addSeparator();
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

	void PopupMenu::setDeleteOnHide(bool deleteOnHide) {
		_deleteOnHide = deleteOnHide;
	}

	bool PopupMenu::deleteOnHide() const noexcept {
		return _deleteOnHide;
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
		updateGeometry();
		move(mapFromGlobal(point));

		show();
		raise();

		if (_animation.animating())
			_animation.restart();
		else
			_animation.start(rect(), animations::VerticalGrowthAnimation::Direction::TopToBottom, 100, 1);

		qDebug() << "popup: " << pos() << size() << isHidden() << parent();
	}

	void PopupMenu::paintEvent(QPaintEvent* event) {
		auto painter = QPainter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

		painter.setPen(Qt::NoPen);
		painter.setBrush(_st->colorBg);

		//painter.setOpacity(_animation.opacity());
		painter.drawRect(_animation.rect());

		qDebug() << _animation.rect();

		//for (auto index = 0; index < _actions.size(); ++index)
		//	_actions[index]->setOpacity(_animation.opacity());
	}

	void PopupMenu::hideEvent(QHideEvent* event) {
		if (_deleteOnHide)
			deleteLater();
	}

	void PopupMenu::focusOutEvent(QFocusEvent* event) {
		hide();
	}

	bool PopupMenu::event(QEvent* _event) {
		return QWidget::event(_event);
	}

	void PopupMenu::mousePressEvent(QMouseEvent* event) {
		qDebug() << "menu press event";
	}

	void PopupMenu::updateGeometry() {
		const auto fullWidth = _st->maximumWidth
			+ _st->margin.left()
			+ _st->margin.right();

		const auto fullHeight = _st->maximumHeight
			+ _st->margin.top()
			+ _st->margin.bottom();

		resize(fullWidth, fullHeight);

		for (auto index = 0; index < _actions.size(); ++index) {
			_actions[index]->resize(_st->maximumWidth, _st->actionHeight);
			index != 0
				? _actions[index]->move(_actions[index]->rect().bottomLeft())
				: _actions[index]->move(0, _st->margin.top());
		}
	}
} // namespace base::qt::ui