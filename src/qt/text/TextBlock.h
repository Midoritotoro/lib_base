#pragma once

#include <base/qt/style/StyleTypes.h>
#include <base/qt/style/StyleFont.h>

#include <base/core/system/Time.h>

#include <base/core/Types.h>
#include <src/qt/text/Types.h>

__BASE_QT_TEXT_NAMESPACE_BEGIN

using Blocks = std::vector<Block>;

  style::font WithFlags(
	const style::font& font,
	TextBlockFlags flags,
	style::FontFlags fontFlags = 0) ;

  Qt::LayoutDirection UnpackParagraphDirection(
	bool ltr,
	bool rtl);

class AbstractBlock {
public:
	  uint16 position() const;
	  TextBlockType type() const;
	  TextBlockFlags flags() const;

	  int objectWidth() const;
	  uint16 colorIndex() const;
	  uint16 linkIndex() const;

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
	  uint16 quoteIndex() const {
		return _quoteIndex;
	}
	void setParagraphDirection(Qt::LayoutDirection direction) {
		_paragraphLTR = (direction == Qt::LeftToRight);
		_paragraphRTL = (direction == Qt::RightToLeft);
	}
	  Qt::LayoutDirection paragraphDirection() const {
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

	  int width() const;
	  int height() const;

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

	  static Block Newline(
		BlockDescriptor descriptor,
		uint16 quoteIndex);
	  static Block Text(BlockDescriptor descriptor);
	  static Block Skip(
		BlockDescriptor descriptor,
		int width,
		int height);

	template <typename FinalBlock>
	  FinalBlock& unsafe() {
		return *reinterpret_cast<FinalBlock*>(&_data);
	}

	template <typename FinalBlock>
	  const FinalBlock& unsafe() const {
		return *reinterpret_cast<const FinalBlock*>(&_data);
	}

	  AbstractBlock* get();
	  const AbstractBlock* get() const;

	  AbstractBlock* operator->();
	  const AbstractBlock* operator->() const;

	  AbstractBlock& operator*();
	  const AbstractBlock& operator*() const;

private:
	struct Tag {
	};

	explicit Block(const Tag&) {
	}

	template <typename FinalType, typename ...Args>
	  static Block New(Args &&...args) {
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

  inline uint16 CountPosition(Blocks::const_iterator i) {
	return (*i)->position();
}

  int CountBlockHeight(
	const AbstractBlock* block,
	const style::font& font);

  inline bool IsMono(TextBlockFlags flags) {
	return (flags & TextBlockFlag::Pre) || (flags & TextBlockFlag::Code);
}

__BASE_QT_TEXT_NAMESPACE_END