#pragma once 

#include <src/qt/text/BlockParser.h>
#include <src/qt/text/WordParser.h>

#include <src/qt/text/TextWord.h>

__BASE_QT_TEXT_NAMESPACE_BEGIN

inline constexpr auto kQFixedMax = (INT_MAX / 256);

inline const TextParseOptions kDefaultTextOptions = {
	TextParseLinks | TextParseMultiline, // flags
	0, // maxw
	0, // maxh
	Qt::LayoutDirectionAuto, // dir
};

inline const TextParseOptions kMarkupTextOptions = {
	TextParseLinks | TextParseMultiline | TextParseMarkdown, // flags
	0, // maxw
	0, // maxh
	Qt::LayoutDirectionAuto, // dir
};

inline const TextParseOptions kPlainTextOptions = {
	TextParseMultiline, // flags
	0, // maxw
	0, // maxh
	Qt::LayoutDirectionAuto, // dir
};


class String {
public:
	struct LineWidthsOptions {
		bool breakEverywhere = false;
		int reserve = 0;
	};

	struct DimensionsResult {
		int width = 0;
		int height = 0;
		std::vector<int> lineWidths;
	};

	struct DimensionsRequest {
		bool breakEverywhere = false;
		bool lineWidths = false;
		int reserve = 0;
	};

	String(int32 minResizeWidth = style::minimumMessageWidth);
	String(String&& other) = default;
	String(const String& other) = default;
	String(
		const QString& string,
		const style::TextStyle* style);

	String& operator=(String&& other) = default;

	~String() = default;

	NODISCARD int countWidth(
		int width,
		bool breakEverywhere = false) const;
	NODISCARD int countHeight(
		int width,
		bool breakEverywhere = false) const;

	NODISCARD DimensionsResult countDimensions(
		GeometryDescriptor geometry) const;
	NODISCARD DimensionsResult countDimensions(
		GeometryDescriptor geometry,
		DimensionsRequest request) const;

	NODISCARD std::vector<int> countLineWidths(int width) const;
	NODISCARD std::vector<int> countLineWidths(
		int width,
		LineWidthsOptions options) const;

	void setText(
		const style::TextStyle* style,
		const QString& text,
		const TextParseOptions& options = kMarkupTextOptions);

	void setMaximumWidth(int width) {
		_maxWidth = width;
	}

	void setMinimumHeight(int height) {
		_minHeight = height;
	}

	NODISCARD bool hasLinks() const;
	void setLink(uint16 index, const common::ClickHandlerPtr& lnk);

	NODISCARD bool hasCollapsedBlockquots() const;
	NODISCARD bool blockquoteCollapsed(int index) const;
	NODISCARD bool blockquoteExpanded(int index) const;
	void setBlockquoteExpanded(int index, bool expanded);
	void setBlockquoteExpandCallback(
		Fn<void(int index, bool expanded)> callback);

	NODISCARD bool hasSkipBlock() const;
	bool updateSkipBlock(int width, int height);
	bool removeSkipBlock();

	NODISCARD int maxWidth() const {
		return _maxWidth;
	}
	NODISCARD int minHeight() const {
		return _minHeight;
	}

	NODISCARD bool isFullSelection(TextSelection selection) const;
	NODISCARD int countMaxMonospaceWidth() const;

	void draw(
		QPainter& painter,
		const PaintContext& context) const;

	NODISCARD bool isEmpty() const;
	NODISCARD int length() const {
		return _text.size();
	}

	NODISCARD QString toQString() const;
	NODISCARD QString toString(
		TextSelection selection = AllTextSelection) const;
	NODISCARD TextWithEntities toTextWithEntities(
		TextSelection selection = AllTextSelection) const;
	NODISCARD TextForMimeData toTextForMimeData(
		TextSelection selection = AllTextSelection) const;

	NODISCARD int lineHeight() const;

	TextSelection adjustSelection(
		TextSelection selection,
		TextSelection::Type selectType);
	void recountNaturalSize(
		bool initial,
		Qt::LayoutDirection optionsDirection = Qt::LayoutDirectionAuto);

	NODISCARD const std::vector<Modification>& modifications() const;

	NODISCARD TextState getState(
		QPoint point,
		int width,
		StateRequest request) const;
	NODISCARD TextState getStateLeft(
		QPoint point,
		int width,
		int outerw,
		StateRequest request) const;

	NODISCARD TextState getStateElided(
		QPoint point,
		int width,
		StateRequestElided request) const;
	NODISCARD TextState getStateElidedLeft(
		QPoint point,
		int width,
		int outerw,
		StateRequestElided request) const;

	void clear();
	const style::TextStyle* style() const noexcept {
		return _st;
	}
private:
	class ExtendedWrap : public std::unique_ptr<ExtendedData> {
	public:
		ExtendedWrap() noexcept;
		ExtendedWrap(ExtendedWrap&& other) noexcept;
		ExtendedWrap& operator=(ExtendedWrap&& other) noexcept;
		~ExtendedWrap();

		ExtendedWrap(
			std::unique_ptr<ExtendedData>&& other) noexcept;
		ExtendedWrap& operator=(
			std::unique_ptr<ExtendedData>&& other) noexcept;

	private:
		void adjustFrom(const ExtendedWrap* other);

	};

	NODISCARD not_null<ExtendedData*> ensureExtended();
	NODISCARD not_null<QuotesData*> ensureQuotes();

	NODISCARD uint16 blockPosition(
		std::vector<Block>::const_iterator i,
		int fullLengthOverride = -1) const;
	NODISCARD uint16 blockEnd(
		std::vector<Block>::const_iterator i,
		int fullLengthOverride = -1) const;
	NODISCARD uint16 blockLength(
		std::vector<Block>::const_iterator i,
		int fullLengthOverride = -1) const;

	NODISCARD QuoteDetails* quoteByIndex(int index) const;
	NODISCARD const style::QuoteStyle& quoteStyle(
		not_null<QuoteDetails*> quote) const;
	NODISCARD QMargins quotePadding(QuoteDetails* quote) const;
	NODISCARD int quoteMinWidth(QuoteDetails* quote) const;
	NODISCARD const QString& quoteHeaderText(QuoteDetails* quote) const;

	NODISCARD int quoteLinesLimit(QuoteDetails* quote) const;

	NODISCARD int quoteIndex(const AbstractBlock* block) const;

	template <
		typename AppendPartCallback,
		typename ClickHandlerStartCallback,
		typename ClickHandlerFinishCallback,
		typename FlagsChangeCallback>
	void enumerateText(
		TextSelection selection,
		AppendPartCallback appendPartCallback,
		ClickHandlerStartCallback clickHandlerStartCallback,
		ClickHandlerFinishCallback clickHandlerFinishCallback,
		FlagsChangeCallback flagsChangeCallback) const;

	template <typename Callback>
	void enumerateLines(
		int w,
		bool breakEverywhere,
		Callback&& callback) const;
	template <typename Callback>
	void enumerateLines(
		GeometryDescriptor geometry,
		Callback&& callback) const;

	void insertModifications(int position, int delta);
	void removeModificationsAfter(int size);

	NODISCARD TextForMimeData toText(
		TextSelection selection,
		bool composeExpanded,
		bool composeEntities) const;

	const style::TextStyle* _st = nullptr;

	QString _text;
	Blocks _blocks;
	Words _words;
	ExtendedWrap _extended;

	int _minResizeWidth = 0;
	int _maxWidth = 0;
	int _minHeight = 0;
	uint16 _startQuoteIndex = 0;
	bool _startParagraphLTR : 1 = false;
	bool _startParagraphRTL : 1 = false;
	bool _hasCustomEmoji : 1 = false;
	bool _isIsolatedEmoji : 1 = false;
	bool _isOnlyCustomEmoji : 1 = false;
	bool _hasNotEmojiAndSpaces : 1 = false;
	bool _skipBlockAddedNewline : 1 = false;
	bool _endsWithQuoteOrOtherDirection : 1 = false;

	friend class BlockParser;
	friend class WordParser;
	friend class StackEngine;
	friend class BidiAlgorithm;
	friend class Renderer;
};

__BASE_QT_TEXT_NAMESPACE_END
