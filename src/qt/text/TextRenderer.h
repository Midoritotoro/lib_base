#pragma once

#include <text/Types.h>
#include <text/TextBlock.h>

#include <base/qt/text/TextClickHandlers.h>

#include <private/qtextengine_p.h>

class QTextItemInt;
struct QScriptAnalysis;
struct QScriptLine;
struct QScriptItem;


namespace base::qt::text {
	inline constexpr auto kQuoteCollapsedLines = 3;

	struct FixedRange {
		QFixed from;
		QFixed till;

		[[nodiscard]] bool empty() const {
			return (till <= from);
		}
	};

	namespace {
		void InitTextItemWithScriptItem(
			QTextItemInt& ti,
			const QScriptItem& si);
		void AppendRange(
			QVarLengthArray<FixedRange>& ranges,
			FixedRange range);

	} // namespace

	[[nodiscard]] FixedRange Intersected(FixedRange a, FixedRange b);
	[[nodiscard]] bool Intersects(FixedRange a, FixedRange b);
	[[nodiscard]] FixedRange United(FixedRange a, FixedRange b);
	[[nodiscard]] bool Distinct(FixedRange a, FixedRange b);


	class Renderer final {
	public:
		explicit Renderer(const String& t);
		~Renderer();

		void draw(QPainter& p, const PaintContext& context);
		[[nodiscard]] TextState getState(
			QPoint point,
			GeometryDescriptor geometry,
			StateRequest request);
	private:
		static constexpr auto kSpoilersRectsSize = 512;
		struct BidiControl;

		[[nodiscard]] Time::time_t now() const;

		void initParagraphBidi();
		void initNextParagraph(
			Blocks::const_iterator i,
			int16 paragraphIndex,
			Qt::LayoutDirection direction);

		void initNextLine();

		void enumerate();
		void restoreAfterElided();

		bool drawLine(
			uint16 lineEnd,
			Blocks::const_iterator blocksEnd);

		[[nodiscard]] FixedRange findSelectTextRange(
			const QScriptItem& si,
			int itemStart,
			int itemEnd,
			QFixed x,
			QFixed itemWidth,
			const QTextItemInt& gf,
			TextSelection selection) const;

		void fillSelectRange(FixedRange range);
		void pushHighlightRange(FixedRange range);

		void applyBlockProperties(
			QTextEngine& e,
			not_null<const AbstractBlock*> block);
		[[nodiscard]] ClickHandlerPtr lookupLink(
			const AbstractBlock* block) const;

		void fillRectsFromRanges();
		void fillRectsFromRanges(
			QVarLengthArray<QRect, kSpoilersRectsSize>& rects,
			QVarLengthArray<FixedRange>& ranges);

		void composeHighlightPath();

		[[nodiscard]] const AbstractBlock* markBlockForElisionGetEnd(
			int blockIndex);
		void setElideBidi(int elideStart);

		void prepareElidedLine(
			QString& lineText,
			int lineStart,
			int& lineLength,
			const AbstractBlock*& endBlock,
			int recursed = 0);

		void prepareElisionAt(
			QString& lineText,
			int& lineLength,
			uint16 position);

		void fillParagraphBg(int paddingBottom);

		const String* _t = nullptr;

		GeometryDescriptor _geometry;
		QPainter* _p = nullptr;

		std::span<SpecialColor> _colors = {};
		const style::TextPalette* _palette = new style::TextPalette();

		style::font _f;

		style::align _align = style::alignTopLeft;
		QPen _originalPen;

		QPen _originalPenSelected;
		QPen _quoteLinkPenOverride;

		const QPen* _currentPen = nullptr;
		const QPen* _currentPenSelected = nullptr;

		struct {
			bool spoiler = false;
			bool selectActiveBlock = false; // For monospace.
		} _background;

		int _yFrom = 0;
		int _yTo = 0;

		TextSelection _selection = { 0, 0 };
		bool _fullWidthSelection = true;

		HighlightInfoRequest* _highlight = nullptr;
		const QChar* _str = nullptr;

		mutable Time::time_t _cachedNow = 0;
		double _spoilerOpacity = 0.;

		QVarLengthArray<FixedRange> _highlightRanges;
		QVarLengthArray<QRect, kSpoilersRectsSize> _highlightRects;

		int _customEmojiSkip = 0;
		int _indexOfElidedBlock = -1;

		Blocks::const_iterator _paragraphStartBlock;
		Qt::LayoutDirection _paragraphDirection = Qt::LayoutDirectionAuto;

		int _paragraphStart = 0;
		int _paragraphLength = 0;

		QVarLengthArray<QScriptAnalysis, 4096> _paragraphAnalysis;

		QuoteDetails* _quote = nullptr;

		Qt::LayoutDirection _quoteDirection = Qt::LayoutDirectionAuto;

		int _quoteShift = 0;
		int _quoteIndex = 0;

		QMargins _quotePadding;

		int _quoteLinesLeft = -1;
		int _quoteTop = 0;
		int _quoteLineTop = 0;

		QuotePaintCache* _quotePreCache = nullptr;
		QuotePaintCache* _quoteBlockquoteCache = nullptr;

		bool _quotePreValid = false;
		bool _quoteBlockquoteValid = false;

	    ClickHandlerPtr _quoteExpandLink;
		bool _quoteExpandLinkLookup = false;

		int _startLeft = 0;
		int _startTop = 0;
		int _startLineWidth = 0;

		QFixed _x, _wLeft, _last_rPadding;
		int _y = 0;

		int _yDelta = 0;
		int _lineIndex = 0;

		int _lineHeight = 0;
		int _fontHeight = 0;

		bool _breakEverywhere = false;
		bool _elidedLine = false;

		int _blocksSize = 0;
		int _elideSavedIndex = 0;

		std::optional<Block> _elideSavedBlock;

		int _lineStart = 0;
		int _localFrom = 0;
		int _lineStartBlock = 0;

		QFixed _lineStartPadding = 0;
		QFixed _lineWidth = 0;

		QFixed _lookupX = 0;
		int _lookupY = 0;

		bool _lookupSymbol = false;
		bool _lookupLink = false;

		StateRequest _lookupRequest;
		TextState _lookupResult;

		bool _elisionMiddle = false;
	};
} // namespace base::qt::text