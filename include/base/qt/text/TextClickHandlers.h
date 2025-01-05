#pragma once 

#include <base/qt/common/Types.h>
#include <base/qt/common/BasicClickHandlers.h>


namespace base::qt::text {
	class PreClickHandler final : public common::ClickHandler {
	public:
		PreClickHandler(not_null<String*> text, uint16 offset, uint16 length);

		[[nodiscard]] not_null<String*> text() const;
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

		[[nodiscard]] not_null<String*> text() const;
		void setText(not_null<String*> text);

		void onClick(common::ClickContext context) const override;

	private:
		not_null<String*> _text;
		uint16 _quoteIndex = 0;
	};
} // namespace base::qt::text