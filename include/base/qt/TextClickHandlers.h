#pragma once 

#include <base/qt/Types.h>
#include <base/qt/ClickHandler.h>


class PreClickHandler final : public ClickHandler {
	public:
		PreClickHandler(not_null<text::String*> text, uint16 offset, uint16 length);

		[[nodiscard]] not_null<text::String*> text() const;
		void setText(not_null<text::String*> text);

		void onClick(ClickContext context) const override;

	private:
		not_null<text::String*> _text;

		uint16 _offset = 0;
		uint16 _length = 0;
	};

class BlockquoteClickHandler final : public ClickHandler {
public:
	BlockquoteClickHandler(not_null<text::String*> text, int quoteIndex);

	[[nodiscard]] not_null<text::String*> text() const;
	void setText(not_null<text::String*> text);

	void onClick(ClickContext context) const override;

private:
	not_null<text::String*> _text;
	uint16 _quoteIndex = 0;
};