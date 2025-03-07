#include <base/qt/ui/PopupMenu.h>

#include <base/qt/ui/Separator.h>
#include <base/core/utility/Algorithm.h>

#include <QApplication>
#include <QMouseEvent>

#include <QPainterPath>


__BASE_QT_UI_NAMESPACE_BEGIN

PopupMenu::PopupMenu(
	QWidget* parent,
	const style::PopupMenu* menuStyle,
	const style::MenuAction* actionStyle
) :
	BaseWidget(parent)
	, _actionSt(actionStyle)
{
	setStyle(menuStyle, true);
	setAttribute(Qt::WA_OpaquePaintEvent, false);

	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_NoSystemBackground);

	setWindowFlags(Qt::WindowFlags(Qt::FramelessWindowHint)
		| Qt::BypassWindowManagerHint 
		| Qt::Popup
		| Qt::NoDropShadowWindowHint);
	setMouseTracking(true);

	setDeleteOnHide(true);
	installEventFilter(this);

	_animation.setAnimationCallback([this] {
		update();
	});

	_opacityAnimation.setAnimationCallback([this] {
		update();
	});

	_blur = std::make_unique<effects::BlurBehindEffect>(this);
	_blur->setBlurMethod(effects::BlurBehindEffect::BlurMethod::StackBlur);

	_blur->setDownsampleFactor(2.0);
	_blur->setBlurRadius(5);

	_blur->setBlurOpacity(0.7);
	_blur->setSourceOpacity(1.0);

	_blur->setBackgroundBrush(Qt::NoBrush);
	_blur->setCoordinateSystem(Qt::DeviceCoordinates);

	setGraphicsEffect(_blur.get());


	connect(_blur.get(), &effects::BlurBehindEffect::repaintRequired, this, qOverload<>(&PopupMenu::repaint));
//	connect(_blur.get(), &QGraphicsEffect::enabledChanged, this, &PopupMenu::setVisible);

	QPalette pal;
	pal.setBrush(QPalette::Window, Qt::black);
	setPalette(pal);
}

PopupMenu::~PopupMenu() {
	for (auto& action : _actions)
		delete base::take(action);
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

	action->setStyle(_actionSt);
	action->setText(title);

	connect(action, &QAbstractButton::clicked, callback);
	_actions.push_back(action);

	// addSeparator();
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

void PopupMenu::setDeleteOnHide(bool deleteOnHide) {
	_deleteOnHide = deleteOnHide;
}

bool PopupMenu::deleteOnHide() const noexcept {
	return _deleteOnHide;
}

bool PopupMenu::empty() const noexcept {
	return _actions.empty();
}

void PopupMenu::hideMenu(bool animated) {
	if (_animation.animating())
		_animation.stop();

	if (_opacityAnimation.animating())
		return _opacityAnimation.restartAfterFinished();

	_opacityAnimation.start(_opacity, 0, 100);
}

void PopupMenu::addSeparator() {
	const auto separator = new Separator(this,
		new style::Separator{
			.width = width()
				- _style->margin.left()
				- _style->margin.right(),
			.padding = { 5, 5, 5, 5 },
			.colorFg = Qt::white
		},
	_style);
}

void PopupMenu::popup(const QPoint& point) {
	updateGeometry();
	move(mapFromGlobal(point));

	show();
	raise();

	if (_animation.animating())
		_animation.restart();
	else
		_animation.start(
			rect(),
			animations::DirectionFlag::TopToBottom |
			animations::DirectionFlag::LeftToRight,
			animations::CombinedGrowthAnimation::Corner::LeftTop,
			100);
}

void PopupMenu::paintEvent(QPaintEvent* event) {
	auto painter = QPainter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	painter.setPen(Qt::NoPen);
	painter.setBrush(_style->colorBg);

	if (_opacityAnimation.animating()) {
		painter.setOpacity(_opacityAnimation.opacity());
		for (auto index = 0; index < _actions.size(); ++index)
			_actions[index]->setOpacity(_opacityAnimation.opacity());
	}

	QPainterPath path;
	path.addRect(rect().adjusted(0, 0, -1, -1));

	painter.setClipPath(path);
	//path.translate(
	//	().topLeft());

	painter.drawRect(_animation.rect());
//	_blur->setRegion(rect());
	_blur->render(&painter, path);

}

void PopupMenu::hideEvent(QHideEvent* event) {
	if (_deleteOnHide)
		deleteLater();
}

void PopupMenu::focusOutEvent(QFocusEvent* event) {
	hideMenu();
}

bool PopupMenu::event(QEvent* _event) {
	return QWidget::event(_event);
}

void PopupMenu::updateGeometry() {
	const auto maximumWidth = _style->_size ? _style->_size->maximumWidth : _style->prefferedWidth;
	const auto maximumHeight = _style->_size ? _style->_size->maximumHeight : _style->prefferedHeight;

	const auto fullWidth = _style->_size->maximumWidth
		+ _style->margin.left()
		+ _style->margin.right();

	const auto fullHeight = _style->_size->maximumHeight
		+ _style->margin.top()
		+ _style->margin.bottom();

	resize(fullWidth, fullHeight);

	for (auto index = 0; index < _actions.size(); ++index) {
		_actions[index]->resize(_style->_size->maximumWidth, _style->actionHeight);
		index != 0
			? _actions[index]->move(_actions[index]->rect().bottomLeft())
			: _actions[index]->move(0, _style->margin.top());
	}
}

__BASE_QT_UI_NAMESPACE_END