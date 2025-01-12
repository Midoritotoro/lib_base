#include <base/qt/ui/FlatLabel.h>

#include <QMouseEvent>
#include <QDesktopServices>

#include <QPainter>
#include <QClipboard>

#include <QMenu>
#include <QApplication>

#include <QMouseEvent>
#include <QDrag>

#include <text/TextEntities.h>
#include <base/qt/style/StyleTypes.h>

#include <QPainterPath>
#include <base/Concurrent.h>

#include <base/qt/text/TextUtility.h>
#include <base/qt/common/Size.h>


namespace base::qt::ui {
	FlatLabel::FlatLabel(QWidget* parent) :
		CoreWidget(parent)
	{
		init();

		setContentsMargins(0, 0, 0, 0);
		setStyle(style::defaultFlatLabelStyle, true);

		setSelectable(true);
		setDoubleClickSelectsParagraph(true);

		setBreakEverywhere(true);
		setCornerRoundMode(style::CornersRoundMode::All);

		setTextAlignment(Qt::AlignLeft);
		setLinksTrusted();

		setAttribute(Qt::WA_AcceptTouchEvents);
	}

	void FlatLabel::init() {
		_contextCopyText = phraseContextCopySelected;
	}


	QMargins FlatLabel::getMargins() const {
		Expects(_style != nullptr);
		return _style->margin;
	}

	int FlatLabel::verticalMargins() const {
		Expects(_style != nullptr);
		return _style->margin.bottom() + _style->margin.top();
	}

	int FlatLabel::horizontalMargins() const {
		Expects(_style != nullptr);
		return _style->margin.left() + _style->margin.right();
	}


	void FlatLabel::setText(const QString& text) {
		_text.setText(_style->textStyle, text, _labelOptions);
		textUpdated();
	}

	const text::String& FlatLabel::text() const noexcept {
		return _text;
	}

	void FlatLabel::setSelectable(bool selectable) {
		if (_selectable == selectable)
			return;

		_selection = { 0, 0 };
		_savedSelection = { 0, 0 };

		_selectable = selectable;
		setMouseTracking(_selectable || _text.hasLinks());
	}

	bool FlatLabel::selectable() const noexcept {
		return _selectable;
	}

	void FlatLabel::setDoubleClickSelectsParagraph(bool doubleClickSelectsParagraph) {
		_doubleClickSelectsParagraph = doubleClickSelectsParagraph;
	}

	bool FlatLabel::doubleClickSelectsParagraph() const noexcept {
		return _doubleClickSelectsParagraph;
	}

	void FlatLabel::setBreakEverywhere(bool breakEverywhere) {
		_breakEverywhere = breakEverywhere;
	}

	bool FlatLabel::breakEverywhere() const noexcept {
		return _breakEverywhere;
	}

	void FlatLabel::setOpacity(float opacity) {
		if (_opacity == opacity)
			return;

		_opacity = opacity;
		update();
	}

	float FlatLabel::opacity() const noexcept {
		return _opacity;
	}

	QSize FlatLabel::sizeHint() const {
		return size();
	}

	QSize FlatLabel::minimumSizeHint() const {
		return size();
	}

	int FlatLabel::textMaxWidth() const noexcept {
		return (_style->_size && _style->_size->maximumWidth)
			? _style->_size->maximumWidth
				- horizontalMargins()
			: _text.maxWidth();
	}

	bool FlatLabel::hasLinks() const noexcept {
		return _text.hasLinks();
	}

	int FlatLabel::fullHeight() const noexcept {
		return _fullTextHeight + verticalMargins();
	}

	void FlatLabel::setTextAlignment(Qt::Alignment alignment) {
		_alignment = alignment;
	}

	Qt::Alignment FlatLabel::alignment() const noexcept {
		return _alignment;
	}

	void FlatLabel::setCornerRoundMode(style::CornersRoundMode cornersRoundMode) {
		_cornersRoundMode = cornersRoundMode;
	}

	style::CornersRoundMode FlatLabel::cornerRoundMode() const noexcept {
		return _cornersRoundMode;
	}

	void FlatLabel::setStyle(const SelfStyle* style, bool repaint) {
		if (style == nullptr)
			return;

		_style = style;

		const auto parentWidget = qobject_cast<QWidget*>(parent());

		if (const auto maxWidth = textMaxWidth(); maxWidth)
			_text.setMaximumWidth(maxWidth);

		if (style->_size && _style->_size->minimumHeight)
			_text.setMinimumHeight(_style->_size->minimumHeight);

		if (style->_size && (!style->_size->maximumHeight || !style->_size->maximumWidth)) {
			auto _Style = *_style;

			if (!style->_size->maximumHeight)
				_Style._size->maximumHeight = parentWidget
					? parentWidget->height()
					? parentWidget->height()
					: common::ScreenResolution().height()
					: 0;
				
			if (!style->_size->maximumWidth) {
				_Style._size->maximumWidth = parentWidget
					? parentWidget->width()
					? parentWidget->width()
					: common::ScreenResolution().width()
					: 0;


				if (_Style._size->maximumWidth)
					_text.setMaximumWidth((_Style._size && _Style._size->maximumWidth)
						? _Style._size->maximumWidth
							- horizontalMargins()
						: _text.maxWidth());
			}

			_style = new SelfStyle(_Style);
		}

		if (repaint == false || _text.toQString().isEmpty())
			return;

		update();
		refreshSize();
	}


	void FlatLabel::setContextMenuHook(Fn<void(ContextMenuRequest)> hook) {
		_contextMenuHook = std::move(hook);
	}

	void FlatLabel::setLink(
		quint16 index,
		const common::ClickHandlerPtr& lnk)
	{
		_text.setLink(index, lnk);
		textUpdated();
	}

	void FlatLabel::setLinksTrusted() {
		static const auto TrustedLinksFilter = [](
			const common::ClickHandlerPtr& link,
			Qt::MouseButton button)
			{
				if (const auto url = dynamic_cast<common::UrlClickHandler*>(link.get())) {
					url->common::UrlClickHandler::onClick({ button });
					return false;
				}
				return true;
			};
		setClickHandlerFilter(TrustedLinksFilter);
	}

	void FlatLabel::setClickHandlerFilter(ClickHandlerFilter&& filter) {
		_clickHandlerFilter = std::move(filter);
	}

	void FlatLabel::overrideLinkClickHandler(Fn<void()> handler) {
		setClickHandlerFilter([=](
			const common::ClickHandlerPtr& link,
			Qt::MouseButton button)
			{
				if (button != Qt::LeftButton)
					return true;

				handler();
				return false;
			});
	}

	void FlatLabel::overrideLinkClickHandler(Fn<void(QString url)> handler) {
		setClickHandlerFilter([=](
			const common::ClickHandlerPtr& link,
			Qt::MouseButton button) {
				if (button != Qt::LeftButton) {
					return true;
				}
				handler(link->url());
				return false;
			});
	}

	void FlatLabel::paintEvent(QPaintEvent* event) {
		auto painter = QPainter(this);

		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
		painter.setOpacity(_opacity);

		const auto textWidth = textMaxWidth() && _text.maxWidth()
			? qMin(textMaxWidth(), _text.maxWidth()
				+ horizontalMargins())
					: _textWidth && _text.maxWidth()
			? qMin(_textWidth, _text.maxWidth() 
				+ horizontalMargins())
					: width()
						- horizontalMargins();

		const auto recountedSize = QSize(textWidth, height());
		style::RoundCorners(painter, recountedSize, _style->borderRadius, _cornersRoundMode);

		painter.fillRect(QRect(QPoint(), recountedSize), _style->colorBg);

		painter.setPen(Qt::white);

		const auto textLeft = _textWidth
			? ((_alignment & Qt::AlignLeft)
				? _style->margin.left()
				: (_alignment & Qt::AlignHCenter)
				? ((width() - _textWidth) / 2)
				: (width() - _style->margin.right() - _textWidth))
			: _style->margin.left();

		const auto selection = !_selection.empty()
			? _selection
			: _contextMenu
			? _savedSelection
			: _selection;

		const auto maximumHeight = _style->_size
			? _style->_size->maximumHeight
			: 0;

		const auto heightExceeded = maximumHeight < _fullTextHeight
			|| textMaxWidth() ? textWidth < textMaxWidth() : textWidth;

		const auto renderElided = _breakEverywhere || heightExceeded;

		const auto lineHeight = _text.style()
			? qMax(_text.style()->lineHeight, _text.style()->_font->height)
			: 0;

		const auto elisionHeight = !renderElided
			? 0
			: maximumHeight
			? qMax(maximumHeight, lineHeight)
			: height();

		_text.draw(painter, {
			.position = { textLeft, _style->margin.top()},
			.availableWidth = textWidth,
			.align = _alignment,
			.clip = event->rect(),
			.palette = &style::defaultTextPalette,
			.now = Time::now(),
			.selection = selection,
			.elisionHeight = elisionHeight,
			.elisionBreakEverywhere = renderElided && _breakEverywhere,
		});
	} 

	void FlatLabel::mouseMoveEvent(QMouseEvent* event) {
		_lastMousePos = event->globalPos();
		dragActionUpdate();
	}

	void FlatLabel::mousePressEvent(QMouseEvent* event) {
		if (_contextMenu)
			return event->accept();

		dragActionStart(event->globalPos(), event->button());
	}

	void FlatLabel::mouseReleaseEvent(QMouseEvent* event) {
		dragActionFinish(event->globalPos(), event->button());

		if (!rect().contains(event->pos()))
			leaveEvent(event);
	}

	void FlatLabel::mouseDoubleClickEvent(QMouseEvent* event) {
		const auto state = dragActionStart(event->globalPos(), event->button());

		if (((_dragAction != Selecting) &&
			(_dragAction != NoDrag)) ||
			_selectionType != text::TextSelection::Type::Letters)
			return;

		if (state.uponSymbol == false)
			return;

		_dragSymbol = state.symbol;
		_selectionType = _doubleClickSelectsParagraph
			? text::TextSelection::Type::Paragraphs
			: text::TextSelection::Type::Words;

		if (_dragAction == NoDrag) {
			_dragAction = Selecting;

			_selection = { state.symbol, state.symbol };
			_savedSelection = { 0, 0 };
		}

		mouseMoveEvent(event);

		_trippleClickPoint = event->globalPos();
		_trippleClickTimer.callOnce(QApplication::doubleClickInterval());
	}

	void FlatLabel::focusOutEvent(QFocusEvent* event) {
		if (_selection.empty())
			return;

		if (_contextMenu)
			_savedSelection = _selection;

		_selection = text::TextSelection(0, 0);
		update();
	}

	void FlatLabel::focusInEvent(QFocusEvent* event) {
		if (_savedSelection.empty())
			return;

		_selection = _savedSelection;
		_savedSelection = { 0, 0 };

		update();
	}

	void FlatLabel::keyPressEvent(QKeyEvent* event) {
		event->ignore();
		if (event->key() == Qt::Key_Copy || (event->key() == Qt::Key_C
			&& event->modifiers().testFlag(Qt::ControlModifier)))
			if (!_selection.empty()) {
				copySelectedText();
				event->accept();
			}
	}

	int FlatLabel::resizeGetHeight(int newWidth) {
		_allowedWidth = newWidth;
		_textWidth = countTextWidth();

		return countTextHeight(_textWidth);
	}

	void FlatLabel::contextMenuEvent(QContextMenuEvent* event) {
		if (!_contextMenuHook && !_selectable && !hasLinks())
			return;

		showContextMenu(event, ContextMenuReason::FromEvent);
	}

	void FlatLabel::copyContextText() {
		text::SetClipboardText(_text.toTextForMimeData());
	}

	void FlatLabel::copySelectedText() {
		const auto selection = _selection.empty()
			? (_contextMenu
				? _savedSelection
				: _selection)
			: _selection;

		qDebug() << selection.from << selection.to;

		if (!selection.empty())
			text::SetClipboardText(_text.toTextForMimeData(selection));
	}

	text::TextState FlatLabel::dragActionUpdate() {
		const auto map = mapFromGlobal(_lastMousePos);
		const auto state = getTextState(map);

		updateHover(state);

		if (_dragAction == PrepareDrag && (map - _dragStartPosition).manhattanLength() >= QApplication::startDragDistance()) {
			_dragAction = Dragging;
			common::InvokeQueued(this, [=] { executeDrag(); });
		}

		return state;
	}


	text::TextState FlatLabel::dragActionStart(const QPoint& p, Qt::MouseButton button) {
		_lastMousePos = p;
		const auto state = dragActionUpdate();

		if (button != Qt::LeftButton)
			return state;

		common::ClickHandler::pressed();
		_dragAction = NoDrag;

		if (common::ClickHandler::getPressed()) {
			_dragStartPosition = mapFromGlobal(_lastMousePos);
			_dragAction = PrepareDrag;
		}
		if (!_selectable || _dragAction != NoDrag)
			return state;

		if (_trippleClickTimer.isActive() && (_lastMousePos - _trippleClickPoint).manhattanLength() < QApplication::startDragDistance()) {
			if (state.uponSymbol) {
				_selection = { state.symbol, state.symbol };
				_savedSelection = { 0, 0 };

				_dragSymbol = state.symbol;
				_dragAction = Selecting;

				_selectionType = text::TextSelection::Type::Paragraphs;

				updateHover(state);

				_trippleClickTimer.callOnce(QApplication::doubleClickInterval());
				update();
			}
		}
		if (_selectionType != text::TextSelection::Type::Paragraphs) {
			_dragSymbol = state.symbol;
			auto uponSelected = state.uponSymbol;

			if (uponSelected)
				if (_dragSymbol < _selection.from || _dragSymbol >= _selection.to)
					uponSelected = false;

			if (uponSelected) {
				_dragStartPosition = mapFromGlobal(_lastMousePos);
				_dragAction = PrepareDrag;
			}
			else {
				if (state.afterSymbol) ++_dragSymbol;
				_selection = { _dragSymbol, _dragSymbol };
				_savedSelection = { 0, 0 };
				_dragAction = Selecting;
				update();
			}
		}
		return state;
	}

	text::TextState FlatLabel::dragActionFinish(const QPoint& p, Qt::MouseButton button) {
		_lastMousePos = p;
		const auto state = dragActionUpdate();

		auto activated = common::ClickHandler::unpressed();

		if (_dragAction == Dragging)
			activated = nullptr;
		else if (_dragAction == PrepareDrag) {
			_selection = { 0, 0 };
			_savedSelection = { 0, 0 };
			update();
		}

		_dragAction = NoDrag;
		_selectionType = text::TextSelection::Type::Letters;

		if (activated) {
			concurrent::invokeAsync([=] {
				const auto guard = window();
				if (!_clickHandlerFilter
					|| _clickHandlerFilter(activated, button))
					common::ActivateClickHandler(guard, activated, button);
				});
		}

		if (QGuiApplication::clipboard()->supportsSelection()
			&& !_selection.empty())
			text::SetClipboardText(
				_text.toTextForMimeData(_selection),
				QClipboard::Selection);

		return state;
	}

	void FlatLabel::updateHover(const text::TextState& state) {
		const auto linkChanged = common::ClickHandler::setActive(state.link, this);

		if (!_selectable) {
			refreshCursor(state.uponSymbol);
			return;
		}

		auto cur = style::cursorDefault;
		if (_dragAction == NoDrag) {

			if (state.link)
				cur = style::cursorPointer;
			else if (state.uponSymbol)
				cur = style::cursorText;
		}
		else {
			if (_dragAction == Selecting) {
				auto second = state.symbol;

				if (state.afterSymbol && _selectionType == text::TextSelection::Type::Letters)
					++second;

				const auto selection = _text.adjustSelection({ qMin(second, _dragSymbol), qMax(second, _dragSymbol) }, _selectionType);

				if (_selection != selection) {
					_selection = selection;
					_savedSelection = { 0, 0 };

					setFocus();
					update();
				}
			}

			if (common::ClickHandler::getPressed())
				cur = style::cursorPointer;
			else if (_dragAction == Selecting)
				cur = style::cursorText;
		}

		if (_dragAction == NoDrag && (linkChanged || cur != _cursor))
			setCursor(_cursor = cur);
	}

	bool FlatLabel::event(QEvent* e) {
		if (e->type() == QEvent::TouchBegin || e->type() == QEvent::TouchUpdate
			|| e->type() == QEvent::TouchEnd || e->type() == QEvent::TouchCancel)
		{
			QTouchEvent* ev = static_cast<QTouchEvent*>(e);
			if (ev->device()->type() == QInputDevice::DeviceType::TouchScreen) {
				touchEvent(ev);
				return true;
			}
		}

		return QWidget::event(e);
	}

	void FlatLabel::touchEvent(QTouchEvent* event) {
		if (event->type() == QEvent::TouchCancel) {
			if (!_touchInProgress)
				return;

			_touchInProgress = false;
			_touchSelectTimer.cancel();

			_touchSelect = false;
			_dragAction = NoDrag;

			return;
		}

		if (!event->touchPoints().isEmpty()) {
			_touchPrevPos = _touchPos;
			_touchPos = event->touchPoints().cbegin()->screenPos().toPoint();
		}

		switch (event->type()) {
		case QEvent::TouchBegin: {
			if (_contextMenu) {
				event->accept();
				return;
			}

			if (_touchInProgress)
				return;

			if (event->touchPoints().isEmpty())
				return;

			_touchInProgress = true;
			_touchSelectTimer.callOnce(QApplication::startDragTime());

			_touchSelect = false;
			_touchStart = _touchPrevPos = _touchPos;
		} break;

		case QEvent::TouchUpdate: {
			if (!_touchInProgress)
				return;

			if (_touchSelect) {
				_lastMousePos = _touchPos;
				dragActionUpdate();
			}
		} break;

		case QEvent::TouchEnd: {
			if (!_touchInProgress)
				return;

			_touchInProgress = false;

			auto weak = QPointer<QWidget>(this);

			if (_touchSelect) {
				dragActionFinish(_touchPos, Qt::RightButton);
				QContextMenuEvent contextMenu(QContextMenuEvent::Mouse, mapFromGlobal(_touchPos), _touchPos);

				showContextMenu(&contextMenu, ContextMenuReason::FromTouch);
			}
			else {
				dragActionStart(_touchPos, Qt::LeftButton);
				dragActionFinish(_touchPos, Qt::LeftButton);
			}

			if (weak) {
				_touchSelectTimer.cancel();
				_touchSelect = false;
			}
		} break;
		}
	}

	text::TextState FlatLabel::getTextState(const QPoint& m) const {
		auto request = text::StateRequestElided();
		request.align = _alignment;

		if (_selectable)
			request.flags |= text::StateRequest::StateFlag::LookupSymbol;

		const auto textWidth = width() - horizontalMargins();

		const auto maximumHeight = _style->_size
			? _style->_size->maximumHeight
			: 0;

		auto state = text::TextState();

		const auto heightExceeded = (maximumHeight < _fullTextHeight
			|| textWidth < _text.maxWidth());
		const auto renderElided = _breakEverywhere || heightExceeded;

		if (renderElided) {
			const auto lineHeight = _text.style() && _text.style()->_font
				? qMax(_text.style()->lineHeight, _text.style()->_font->height)
				: 1;

			const auto lines = qMax(maximumHeight / lineHeight, 1);
			request.lines = lines;

			if (_breakEverywhere)
				request.flags |= text::StateRequest::StateFlag::BreakEverywhere;

			state = _text.getStateElided(m - QPoint(_style->margin.left(),
				_style->margin.top()), textWidth, request);
		}
		else
			state = _text.getState(m - QPoint(_style->margin.left(),
				_style->margin.top()), textWidth, request);


		return state;
	}

	void FlatLabel::touchSelect() {
		_touchSelect = true;
		dragActionStart(_touchPos, Qt::LeftButton);
	}

	void FlatLabel::executeDrag() {
		if (_dragAction != Dragging)
			return;

		auto state = getTextState(_dragStartPosition);
		bool uponSelected = state.uponSymbol && _selection.from <= state.symbol;

		if (uponSelected)
			if (_dragSymbol < _selection.from || _dragSymbol >= _selection.to)
				uponSelected = false;

		const auto pressedHandler = common::ClickHandler::getPressed();
		const auto selectedText = [&] {
			if (uponSelected)
				return _text.toTextForMimeData(_selection);
			else if (pressedHandler)
				return text::TextForMimeData::Simple(pressedHandler->dragText());

			return text::TextForMimeData();
			}();

		if (auto mimeData = text::MimeDataFromText(selectedText)) {
			auto drag = new QDrag(window());
			drag->setMimeData(mimeData.release());
			drag->exec(Qt::CopyAction);

			common::ClickHandler::unpressed();
		}
	}


	void FlatLabel::showContextMenu(QContextMenuEvent* e, ContextMenuReason reason) {
		if (e->reason() == QContextMenuEvent::Mouse)
			_lastMousePos = e->globalPos();
		else
			_lastMousePos = QCursor::pos();

		const auto state = dragActionUpdate();
		const auto hasSelection = _selectable && !_selection.empty();
		const auto uponSelection = _selectable
			&& ((reason == ContextMenuReason::FromTouch && hasSelection)
				|| (state.uponSymbol
					&& (state.symbol >= _selection.from)
					&& (state.symbol < _selection.to)));

		/*if (_contextMenu)
			delete base::take(_contextMenu);*/

		_contextMenu = new PopupMenu(parent() ?
			qobject_cast<QWidget*>(parent())
			: this);
		
		auto request = ContextMenuRequest({
			.menu = _contextMenu,
			.selection = _selectable ? _selection : text::TextSelection(),
			.link = common::ClickHandler::getActive(),
			.uponSelection = uponSelection,
			.fullSelection = _selectable && _text.isFullSelection(_selection)
		});

		_contextMenuHook
			? _contextMenuHook(request)
			: fillContextMenu(request);

		if (_contextMenu->empty())
			_contextMenu = nullptr;
		else {
			_contextMenu->popup(e->globalPos());
			e->accept();
		}
	}

	void FlatLabel::fillContextMenu(ContextMenuRequest request) {
		if (request.fullSelection && !_contextCopyText.isEmpty())
			request.menu->addAction(
				_contextCopyText,
				[=] { copyContextText(); });
		else if (request.uponSelection && !request.fullSelection)
			request.menu->addAction(
				_contextCopyText,
				[=] { copySelectedText(); });
		else if (_selectable
			&& request.selection.empty()
			&& !_contextCopyText.isEmpty())
			request.menu->addAction(
				_contextCopyText,
				[=] { copyContextText(); });

		if (request.link) {
			const auto label = request.link->copyToClipboardContextItemText();
			if (!label.isEmpty())
				request.menu->addAction(
					label,
					[text = request.link->copyToClipboardText()] {
						QGuiApplication::clipboard()->setText(text);
					});
		}
	}

	int FlatLabel::countTextWidth() const noexcept {
		const auto available = _allowedWidth
			? _allowedWidth
			: textMaxWidth();

		if (_allowedWidth > 0
			&& _allowedWidth < _text.maxWidth())
		{
			auto large = _allowedWidth;
			auto _small = _allowedWidth / 2;

			const auto largeHeight = _text.countHeight(large, _breakEverywhere);

			while (large - _small > 1) {
				const auto middle = (large + _small) / 2;

				largeHeight == _text.countHeight(middle, _breakEverywhere)
					? large = middle
					: _small = middle;
			}

			return large;
		}

		return available;
	}

	int FlatLabel::countTextHeight(int textWidth) {
		const auto maximumHeight = _style->_size
			? _style->_size->maximumHeight
			: 0;

		_fullTextHeight = _text.countHeight(textWidth, _breakEverywhere);

		return maximumHeight
			? qMin(_fullTextHeight, maximumHeight)
			: _fullTextHeight;
	}

	void FlatLabel::refreshSize() {
		const auto textWidth = countTextWidth();
		const auto textHeight = countTextHeight(textWidth);

		const auto fullWidth = textWidth + horizontalMargins();
		const auto fullHeight = textHeight + verticalMargins();

		resize(fullWidth, fullHeight);
	}

	void FlatLabel::refreshCursor(bool uponSymbol) {
		if (_dragAction != NoDrag)
			return;

		const auto needTextCursor = _selectable && uponSymbol;
		auto newCursor = needTextCursor ? style::cursorText : style::cursorDefault;

		if (common::ClickHandler::getActive())
			newCursor = style::cursorPointer;

		if (newCursor != _cursor) {
			_cursor = newCursor;
			setCursor(_cursor);
		}
	}

	void FlatLabel::textUpdated() {
		refreshSize();
		update();
		setMouseTracking(_selectable || hasLinks());
	}
} // namespace base::qt::ui