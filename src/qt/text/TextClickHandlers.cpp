#include <base/qt/text/TextClickHandlers.h>
#include <base/qt/text/String.h>

#include <base/qt/text/TextUtility.h>


__BASE_QT_TEXT_NAMESPACE_BEGIN

PreClickHandler::PreClickHandler(
	not_null<String*> text,
	uint16 offset,
	uint16 length)
	: _text(text)
	, _offset(offset)
	, _length(length)
{}

not_null<String*> PreClickHandler::text() const {
	return _text;
}

void PreClickHandler::setText(not_null<String*> text) {
	_text = text;
}

void PreClickHandler::onClick(common::ClickContext context) const {
	if (context.button != Qt::LeftButton)
		return;

	const auto till = uint16(_offset + _length);
	auto text = _text->toTextForMimeData({ _offset, till });

	if (text.empty())
		return;
	else if (!text.rich.text.endsWith('\n'))
		text.rich.text.append('\n');
	if (!text.expanded.endsWith('\n'))
		text.expanded.append('\n');

	SetClipboardText(std::move(text));
}

BlockquoteClickHandler::BlockquoteClickHandler(
	not_null<String*> text,
	int quoteIndex)
	: _text(text)
	, _quoteIndex(quoteIndex)
{}

not_null<String*> BlockquoteClickHandler::text() const {
	return _text;
}

void BlockquoteClickHandler::setText(not_null<String*> text) {
	_text = text;
}

void BlockquoteClickHandler::onClick(common::ClickContext context) const {
	if (context.button != Qt::LeftButton)
		return;

	_text->setBlockquoteExpanded(
		_quoteIndex,
		!_text->blockquoteExpanded(_quoteIndex));
}

__BASE_QT_TEXT_NAMESPACE_END
