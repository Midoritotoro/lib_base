#include <base/qt/ui/ScrollArea.h>

#include <QScrollBar>
#include <QEvent>

#include <QWheelEvent>
#include <QPainter>

#include <base/core/system/Time.h>

#include <base/qt/style/StyleWidgets.h>
#include <base/qt/style/StyleScale.h>


//namespace {
//	void SetWindowBlur(HWND hWnd)
//	{
//		qDebug() << "SetWindowBlur";
//		const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
//		if (hModule)
//		{
//			struct ACCENTPOLICY
//			{
//				int nAccentState;
//				int nFlags;
//				int nColor;
//				int nAnimationId;
//			};
//			struct WINCOMPATTRDATA
//			{
//				int nAttribute;
//				PVOID pData;
//				ULONG ulDataSize;
//			};
//			typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
//			const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hModule, "SetWindowCompositionAttribute");
//			if (SetWindowCompositionAttribute)
//			{
//				ACCENTPOLICY policy = { 3, 0, 0, 0 };
//				WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) };
//				SetWindowCompositionAttribute(hWnd, &data);
//			}
//			FreeLibrary(hModule);
//		}
//	}
//}
//
//static QWindow* windowForWidget(const QWidget* widget)
//{
//	QWindow* window = widget->windowHandle();
//	if (window)
//		return window;
//	const QWidget* nativeParent = widget->nativeParentWidget();
//	if (nativeParent)
//		return nativeParent->windowHandle();
//	return 0;
//}


__BASE_QT_UI_NAMESPACE_BEGIN

InnerWidget::InnerWidget(QWidget* parent) :
	QWidget(parent)
{
	setAutoFillBackground(false);

	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_TranslucentBackground);

	setWindowFlag(Qt::FramelessWindowHint);
}

void InnerWidget::setOpacity(double opacity) {
	_opacity = opacity;
}

double InnerWidget::opacity() const noexcept {
	return _opacity;
}

void InnerWidget::setBackgroundColor(const QColor& color) {
	_backgroundColor = color;
}

QColor InnerWidget::backgroundColor() const noexcept {
	return _backgroundColor;
}

void InnerWidget::paintEvent(QPaintEvent* event) {
	auto painter = QPainter(this);

	painter.setOpacity(_opacity);

	painter.setPen(Qt::NoPen);
	painter.setBrush(_backgroundColor);

	if (const auto fill = rect().intersected(event->rect()); fill.isNull() == false)
		painter.drawRect(fill);
}


ScrollArea::ScrollArea(QWidget* parent) :
	QScrollArea(parent)
{
	setLayoutDirection(Qt::LeftToRight);
	setOpacity(1);

	verticalScrollBar()->setSingleStep(style::ConvertScale(verticalScrollBar()->singleStep()));

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setFrameStyle(int(QFrame::NoFrame) | QFrame::Plain);
	viewport()->setAutoFillBackground(false);

	connect(verticalScrollBar(), &QAbstractSlider::valueChanged, this, &ScrollArea::scrolled);
	connect(verticalScrollBar(), &QAbstractSlider::rangeChanged, this, &ScrollArea::scrolled);

	setStyleSheet(style::ScrollAreaStyle());
	setAutoFillBackground(false);

	setContentsMargins(0, 0, 0, 0);
	setWidgetResizable(true);

	_verticalValue = verticalScrollBar()->value();
}

void ScrollArea::scrolled() {
	const int verticalValue = verticalScrollBar()->value();

	if (_verticalValue == verticalValue)
		return;

	_disabled
		? verticalScrollBar()->setValue(_verticalValue)
		: (void)(_verticalValue = verticalValue);
}

int ScrollArea::scrollHeight() const {
	QWidget* w(widget());
	return w ? qMax(w->height(), height()) : height();
}

int ScrollArea::scrollTopMax() const {
	return scrollHeight() - height();
}

int ScrollArea::scrollTop() const {
	return _verticalValue;
}

bool ScrollArea::viewportEvent(QEvent* event) {
	if (event->type() == QEvent::Wheel) {
		const auto _wheelEvent = static_cast<QWheelEvent*>(event);

		_wheelEvent->angleDelta().y() > 0
			? verticalScrollBar()->setValue(verticalScrollBar()->value() - verticalScrollBar()->singleStep())
			: verticalScrollBar()->setValue(verticalScrollBar()->value() + verticalScrollBar()->singleStep());
		return true;
	}

	return QScrollArea::viewportEvent(event);
}

void ScrollArea::keyPressEvent(QKeyEvent* event) {
	if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
		&& (event->modifiers().testFlag(Qt::AltModifier)
			|| event->modifiers().testFlag(Qt::ControlModifier))
		|| event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
		event->ignore();
	else if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Back)
		((QObject*)widget())->event(event);
	else
		QScrollArea::keyPressEvent(event);
}

void ScrollArea::scrollToWidget(not_null<QWidget*> widget) {
	if (const auto local = this->widget()) {
		const auto globalPosition = widget->mapToGlobal(QPoint(0, 0));
		const auto localPosition = local->mapFromGlobal(globalPosition);

		const auto localTop = localPosition.y();
		const auto localBottom = localTop + widget->height();

		scrollToY(localTop, localBottom);
	}
}

int ScrollArea::computeScrollTo(int toTop, int toBottom) {
	const auto toMin = 0;
	const auto toMax = scrollTopMax();

	if (toTop < toMin)
		toTop = toMin;
	else if (toTop > toMax)
		toTop = toMax;

	const auto exact = (toBottom < 0);

	const auto curTop = scrollTop();
	const auto curHeight = height();
	const auto curBottom = curTop + curHeight;

	auto scToTop = toTop;

	if (!exact && toTop >= curTop) {
		if (toBottom < toTop)
			toBottom = toTop;

		if (toBottom <= curBottom)
			return curTop;

		scToTop = toBottom - curHeight;
		if (scToTop > toTop)

			scToTop = toTop;
		if (scToTop == curTop)

			return curTop;
	}
	else {
		scToTop = toTop;
	}
	return scToTop;
}

void ScrollArea::scrollToY(int toTop, int toBottom) {
	verticalScrollBar()->setValue(computeScrollTo(toTop, toBottom));
}

void ScrollArea::disableScroll(bool dis) {
	_disabled = dis;
}

void ScrollArea::addItem(QWidget* item, Qt::Alignment align) {
	const auto itemHeight =
		item->sizeHint().height()
		+ _scrollLayout->contentsMargins().bottom();

	_itemsHeight += itemHeight;

	const auto fullHeight =
		_scrollLayout->count() > 0
		? widget()->height()
		+ itemHeight
		: itemHeight;

	const auto newHeight =
		widget()->height() != widget()->minimumHeight()
		? qMax(widget()->minimumHeight(), fullHeight)
		: qMax(widget()->minimumHeight(),
			qMin(itemsHeight(), fullHeight));

	widget()->setFixedHeight(newHeight);
	_scrollLayout->addWidget(item, 0, align);
}

void ScrollArea::setWidget(InnerWidget* widget) {
	QScrollArea::setWidget(widget);

	_scrollLayout = new QVBoxLayout(widget);

	_scrollLayout->setContentsMargins(10, 5, 10 + verticalScrollBar()->width(), 15);
	_scrollLayout->setSpacing(15);
}

InnerWidget* ScrollArea::widget() const noexcept {
	return static_cast<InnerWidget*>(QScrollArea::widget());
}

void ScrollArea::setOpacity(double opacity) {
	_opacity = opacity;
	update();
}

void ScrollArea::paintEvent(QPaintEvent* event)
{}

int ScrollArea::itemsHeight() const {
	return _itemsHeight;
}

bool ScrollArea::focusNextPrevChild(bool next) {
	if (QWidget::focusNextPrevChild(next))
		return true;
	return false;
}

__BASE_QT_UI_NAMESPACE_END
