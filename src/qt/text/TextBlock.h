#pragma once

#include <base/qt/style/StyleTypes.h>
#include <base/qt/style/StyleFont.h>

#include <base/core/system/Time.h>

#include <base/core/Types.h>
#include <src/qt/text/Types.h>

__BASE_QT_TEXT_NAMESPACE_BEGIN

using Blocks = std::vector<Block>;

base_nodiscard style::font WithFlags(
	const style::font& font,
	TextBlockFlags flags,
	style::FontFlags fontFlags = 0) ;

base_nodiscard Qt::LayoutDirection UnpackParagraphDirection(
	bool ltr,
	bool rtl);

class AbstractBlock {
public:
	base_nodiscard uint16 position() const;
	base_nodiscard TextBlockType type() const;
	base_nodiscard TextBlockFlags flags() const;

	base_nodiscard int objectWidth() const;
	base_nodiscard uint16 colorIndex() const;
	base_nodiscard uint16 linkIndex() const;

	void setLinkIndex(uint16 index);
protected:
	AbstractBlock(TextBlockType type, BlockDescriptor descriptor);

	uint16 _position = 0;
	uint16 _type : 4 = 0;
	uint16 _flags : 12 = 0;
	uint16 _linkIndex = 0;
	uint16 _colorIndex = 0;

};

class NewlineBlock final : public AbstractBlock {
public:
	NewlineBlock(BlockDescriptor descriptor, uint16 quoteIndex);

	void setQuoteIndex(uint16 index) {
		_quoteIndex = index;
	}
	base_nodiscard uint16 quoteIndex() const {
		return _quoteIndex;
	}
	void setParagraphDirection(Qt::LayoutDirection direction) {
		_paragraphLTR = (direction == Qt::LeftToRight);
		_paragraphRTL = (direction == Qt::RightToLeft);
	}
	base_nodiscard Qt::LayoutDirection paragraphDirection() const {
		return UnpackParagraphDirection(_paragraphLTR, _paragraphRTL);
	}

private:
	uint16 _quoteIndex = 0;
	bool _paragraphLTR : 1 = false;
	bool _paragraphRTL : 1 = false;

};

class TextBlock final : public AbstractBlock {
public:
	explicit TextBlock(BlockDescriptor descriptor);

};

class SkipBlock final : public AbstractBlock {
public:
	SkipBlock(BlockDescriptor descriptor, int width, int height);

	base_nodiscard int width() const;
	base_nodiscard int height() const;

private:
	int _width = 0;
	int _height = 0;

};

class Block final {
public:
	Block();
	Block(Block&& other);
	Block& operator=(Block&& other);
	~Block();

	base_nodiscard static Block Newline(
		BlockDescriptor descriptor,
		uint16 quoteIndex);
	base_nodiscard static Block Text(BlockDescriptor descriptor);
	base_nodiscard static Block Skip(
		BlockDescriptor descriptor,
		int width,
		int height);

	template <typename FinalBlock>
	base_nodiscard FinalBlock& unsafe() {
		return *reinterpret_cast<FinalBlock*>(&_data);
	}

	template <typename FinalBlock>
	base_nodiscard const FinalBlock& unsafe() const {
		return *reinterpret_cast<const FinalBlock*>(&_data);
	}

	base_nodiscard AbstractBlock* get();
	base_nodiscard const AbstractBlock* get() const;

	base_nodiscard AbstractBlock* operator->();
	base_nodiscard const AbstractBlock* operator->() const;

	base_nodiscard AbstractBlock& operator*();
	base_nodiscard const AbstractBlock& operator*() const;

private:
	struct Tag {
	};

	explicit Block(const Tag&) {
	}

	template <typename FinalType, typename ...Args>
	base_nodiscard static Block New(Args &&...args) {
		auto result = Block(Tag{});
		result.emplace<FinalType>(std::forward<Args>(args)...);
		return result;
	}

	template <typename FinalType, typename ...Args>
	void emplace(Args &&...args) {
		new (&_data) FinalType(std::forward<Args>(args)...);
	}

	void destroy();

	static_assert(sizeof(NewlineBlock) <= sizeof(SkipBlock));
	static_assert(alignof(NewlineBlock) <= alignof(void*));
	static_assert(sizeof(TextBlock) <= sizeof(SkipBlock));
	static_assert(alignof(TextBlock) <= alignof(void*));

	std::aligned_storage_t<sizeof(SkipBlock), alignof(void*)> _data;
};

base_nodiscard inline uint16 CountPosition(Blocks::const_iterator i) {
	return (*i)->position();
}

base_nodiscard int CountBlockHeight(
	const AbstractBlock* block,
	const style::font& font);

base_nodiscard inline bool IsMono(TextBlockFlags flags) {
	return (flags & TextBlockFlag::Pre) || (flags & TextBlockFlag::Code);
}

__BASE_QT_TEXT_NAMESPACE_END