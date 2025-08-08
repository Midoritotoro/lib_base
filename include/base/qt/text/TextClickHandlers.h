#pragma once 

#include <base/core/Types.h>
#include <base/qt/common/BasicClickHandlers.h>


__BASE_QT_TEXT_NAMESPACE_BEGIN

class PreClickHandler final : public common::ClickHandler {
public:
	PreClickHandler(not_null<String*> text, uint16 offset, uint16 length);

	base_nodiscard not_null<String*> text() const;
	void setText(not_null<String*> text);

	void onClick(common::ClickContext context) const override;

private:
	not_null<String*> _text;

	uint16 _offset = 0;
	uint16 _length = 0;
};

class BlockquoteClickHandler final : public common::ClickHandler {
public:
	BlockquoteClickHandler(not_null<String*> text, int quoteIndex);

	base_nodiscard not_null<String*> text() const;
	void setText(not_null<String*> text);

	void onClick(common::ClickContext context) const override;

private:
	not_null<String*> _text;
	uint16 _quoteIndex = 0;
};

__BASE_QT_TEXT_NAMESPACE_END
