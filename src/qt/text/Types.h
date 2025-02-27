#pragma once 

#include <base/core/Types.h>
#include <base/utility/Flags.h>

#include <base/system/Time.h>

#include <base/qt/style/StyleFont.h>
#include <base/qt/common/ClickHandler.h>

#include <private/qfixed_p.h>
#include <private/qfontengine_p.h>

#include <QImage>
#include <ranges>

#include <span>

namespace base::qt::text {
	class PreClickHandler;
	class BlockquoteClickHandler;
}

namespace base::qt::common {
	class ClickHandler;
	using ClickHandlerPtr = std::shared_ptr<ClickHandler>;
}

namespace base::qt::text {
	namespace {
		[[nodiscard]] QImage imageFromColor(
			const QColor& color,
			const QSize& size)
		{
			auto image = QImage(size, QImage::Format_ARGB32_Premultiplied);
			image.fill(color);

			return image;
		}
	}

	inline constexpr auto kMaxQuoteOutlines = 3;

	enum class EntityType : uchar {
		Invalid = 0,

		Url,
		CustomUrl,
		Email,
		Hashtag,
		Cashtag,
		Mention,
		MentionName,
		CustomEmoji,
		BotCommand,
		MediaTimestamp,
		Colorized,
		Phone,

		Bold,
		Semibold,
		Italic,
		Underline,
		StrikeOut,
		Code, // inline
		Pre,  // block
		Blockquote,
		Spoiler
	};

	enum class EntityLinkShown : uchar {
		Full,
		Partial,
	};

	enum class TextBlockType : uint16 {
		Newline = 0x01,
		Text = 0x02,
		Emoji = 0x03,
		CustomEmoji = 0x04,
		Skip = 0x05,
	};

	enum class TextBlockFlag : uint16 {
		Bold = 0x001,
		Italic = 0x002,
		Underline = 0x004,
		StrikeOut = 0x008,
		Tilde = 0x010,
		Semibold = 0x020,
		Code = 0x040,
		Pre = 0x080,
		Spoiler = 0x100,
		Blockquote = 0x200
	};

	enum {
		TextParseMultiline = 0x001,
		TextParseLinks = 0x002,
		TextParseMentions = 0x004,
		TextParseHashtags = 0x008,
		TextParseBotCommands = 0x010,
		TextParseMarkdown = 0x020,
		TextParseColorized = 0x040,
	};


	DECLARE_FLAGS(TextBlockFlags, TextBlockFlag)

	struct TextSelection {
		enum class Type {
			Letters = 0x01,
			Words = 0x02,
			Paragraphs = 0x03,
		};

		constexpr TextSelection() = default;
		constexpr TextSelection(uint16 from, uint16 to) : from(from), to(to) {
		}

		[[nodiscard]] constexpr bool empty() const noexcept {
			return from == to;
		}

		[[nodiscard]] bool isFullSelection(const QString& text) const {
			return (from == 0) && (to >= text.size());
		}

		uint16 from = 0;
		uint16 to = 0;
	};

	inline bool operator==(TextSelection a, TextSelection b) {
		return a.from == b.from && a.to == b.to;
	}

	inline bool operator!=(TextSelection a, TextSelection b) {
		return !(a == b);
	}

	struct TextParseOptions {
		int32 flags;
		int32 maxw;
		int32 maxh;
		Qt::LayoutDirection dir;
	};

	struct SkipBlockPaintParts {
		uint32 skippedTop : 29 = 0;
		uint32 skipBottom : 1 = 0;
		uint32 expandIcon : 1 = 0;
		uint32 collapseIcon : 1 = 0;
	};

	struct BlockDescriptor {
		TextBlockFlags flags;
		uint16 position = 0;
		uint16 linkIndex = 0;
		uint16 colorIndex = 0;
	};

	struct QuotePaintCache {
		QImage corners = imageFromColor(QColor(24, 37, 51), QSize(10, 10));
		QImage outline = imageFromColor(QColor(24, 37, 51), QSize(10, 10));

		QImage expand = imageFromColor(QColor(24, 37, 51), QSize(10, 10));
		QImage collapse = imageFromColor(QColor(24, 37, 51), QSize(10, 10));

		mutable QImage bottomCorner = imageFromColor(QColor(24, 37, 51), QSize(10, 10));
		mutable QImage bottomRounding = imageFromColor(QColor(24, 37, 51), QSize(10, 10));
		mutable QImage collapsedLine = imageFromColor(QColor(24, 37, 51), QSize(10, 10));

		std::array<QColor, kMaxQuoteOutlines> outlinesCached;

		QColor headerCached = QColor(24, 37, 51);

		QColor bgCached = QColor(24, 37, 51);
		QColor iconCached = QColor(24, 37, 51);

		std::array<QColor, kMaxQuoteOutlines> outlines = { QColor(24, 37, 51), QColor(24, 37, 51), QColor(24, 37, 51) };

		QColor header = QColor(24, 37, 51);
		QColor bg = QColor(24, 37, 51);

		QColor icon;
	};

	struct SpecialColor {
		const QPen* pen = nullptr;
		const QPen* penSelected = nullptr;
	};

	struct HighlightInfoRequest {
		TextSelection range;
		QRect interpolateTo;
		double interpolateProgress = 0.;
		QPainterPath* outPath = nullptr;
	};

	struct LineGeometry {
		int left = 0;
		int width = 0;
		bool elided = false;
	};

	struct GeometryDescriptor {
		Fn<LineGeometry(int line)> layout;
		bool breakEverywhere = false;
		bool* outElided = nullptr;
	};

	struct PaintContext {
		QPoint position;
		int outerWidth = 0;
		int availableWidth = 0;
		GeometryDescriptor geometry;
		Qt::Alignment align = Qt::AlignLeft;
		QRect clip;

		const style::TextPalette* palette = new style::TextPalette();

		QuotePaintCache* pre = new QuotePaintCache();
		QuotePaintCache* blockquote = new QuotePaintCache();
		gsl::span<SpecialColor> colors = {};
		Time::time_t now = 0;

		bool paused = false;
		bool pausedEmoji = false;
		bool pausedSpoiler = false;

		bool fullWidthSelection = true;
		TextSelection selection;

		HighlightInfoRequest* highlight = nullptr;

		int elisionHeight = 0;
		int elisionLines = 0;
		int elisionRemoveFromEnd = 0;
		bool elisionBreakEverywhere = false;

		bool elisionMiddle = false;
		bool useFullWidth = false; // !(width = min(availableWidth, maxWidth()))
	};

	struct QuoteDetails {
		QString language;

		std::shared_ptr<PreClickHandler> copy;
		std::shared_ptr<BlockquoteClickHandler> toggle;

		int copyWidth = 0;
		int maxWidth = 0;

		int minHeight = 0;
		int scrollLeft = 0;

		bool blockquote = true;
		bool collapsed = true;
		bool expanded = true;
		bool pre = true;
	};

	struct QuotesData {
		std::vector<QuoteDetails> list;
		Fn<void(int index, bool expanded)> expandCallback;
	};

	struct Modification {
		int position = 0;
		uint16 skipped = 0;
		bool added = false;
	};

	struct ExtendedData {
		std::vector<common::ClickHandlerPtr> links;
		std::unique_ptr<QuotesData> quotes;
		std::vector<Modification> modifications;
	};


	struct StateRequest {
		enum class StateFlag {
			BreakEverywhere = (1 << 0),
			LookupSymbol = (1 << 1),
			LookupLink = (1 << 2),
			LookupCustomTooltip = (1 << 3),
		};

		DECLARE_FLAGS(StateFlags, StateFlag);
		friend inline constexpr auto is_flag_type(StateFlag) {
			return true;
		};

		StateRequest()
		{}

		style::align align = style::alignLeft;
		StateFlags flags = StateFlag::LookupLink;

	};

	struct TextState {
		common::ClickHandlerPtr link;
		bool uponSymbol = false;
		bool afterSymbol = false;
		uint16 symbol = 0;
	};

	struct StateRequestElided : StateRequest {
		StateRequestElided() {
		}
		StateRequestElided(const StateRequest& other) : StateRequest(other) {
		}

		int lines = 1;
		int removeFromEnd = 0;
	};


	class Word;
	class Block;
	class EntityInText;
	class String;
	class WordParser;
	class BlockParser;
	class AbstractBlock;
	class StackEngine;
	class BidiAlgorithm;
} // namespace base::qt::text