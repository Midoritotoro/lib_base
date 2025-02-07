#pragma once 

#include <QFont>
#include <QFontMetrics>

#include <base/Types.h>
#include <base/Flags.h>

#include <base/qt/style/StyleTypes.h>

#include <QMap>
#include <cmath>

#include <QFontInfo>
#include <QFontDatabase>

#include <private/qfontengine_p.h>


namespace base::qt::style {
	enum FontFlag : uchar {
		Bold = 0x01,
		Italic = 0x02,
		Underline = 0x04,
		StrikeOut = 0x08,
		Semibold = 0x10,
		Monospace = 0x20,
	};

	DECLARE_FLAGS(FontFlags, FontFlag)

	struct FontResolveResult {
		QFont font;

		double ascent = 0.;
		double height = 0.;

		int iascent = 0;
		int iheight = 0;

		int requestedFamily = 0;
		int requestedSize = 0;

		FontFlags requestedFlags;
	};

	struct QuoteStyle {
		int radius = 10;
		bool scrollable = false;

		int outlineShift = 2;

		QPoint headerPosition = { 10, 2 };

		QImage corners;
		QImage expand;
		QImage collapse;

		mutable QImage bottomCorner;
		mutable QImage bottomRounding;
		mutable QImage collapsedLine;

		int outlinesCached = 3;
		QColor headerCached;
		QColor bgCached;

		int outline = 3;
		int header = 20;

		int verticalSkip = 5;
		QColor bg = QColor(24, 37, 51);
	};

	struct TextPalette {
		QColor linkFg;
		QColor monoFg;

		QColor selectBg;
		QColor selectFg;

		QColor selectLinkFg;
		QColor selectMonoFg;
		QColor selectOverlay;

		bool linkAlwaysActive;
	};

	inline const TextPalette defaultTextPalette = TextPalette{
		.linkFg = QColor(132, 220, 240),
		.monoFg = QColor(132, 220, 240),
		.selectBg = QColor(46, 112, 165),
		.selectFg = Qt::transparent,
		.selectLinkFg = QColor(46, 112, 165),
		.selectMonoFg = QColor(46, 112, 165),
		.selectOverlay = QColor(46, 112, 165),
		.linkAlwaysActive = true
	};

	[[nodiscard]] const QString& SystemFontTag();
	void SetCustomFont(const QString& font);

	[[nodiscard]] QString CustomFont();
	[[nodiscard]] const FontResolveResult* FindAdjustResult(const QFont& font);

	inline constexpr bool is_flag_type(FontFlag) {
		return true;
	}
} // namespace base::qt::style

namespace base::qt::style::internal{
	inline constexpr auto kFontVariants = 0x40;

	class Font;
	class FontData;

	using FontVariants = std::array<Font, kFontVariants>;

	void StartFonts();

	void DestroyFonts();
	int RegisterFontFamily(const QString& family);

	class Font final {
	public:
		Font(Qt::Initialization = Qt::Uninitialized);

		Font(
			int size,
			FontFlags flags,
			const QString& family);
		Font(
			int size,
			FontFlags flags,
			int family);

		[[nodiscard]] FontData* operator->() const;
		[[nodiscard]] FontData* get() const;

		[[nodiscard]] operator bool() const;
		[[nodiscard]] operator const QFont&() const;
	private:
		FontData* _data = nullptr;

		explicit Font(FontData* data);
		Font(
			int size,
			FontFlags flags,
			int family,
			FontVariants* modified);

		void init(
			int size,
			FontFlags flags,
			int family,
			FontVariants* modified);

		friend void Start();

		friend class FontData;
		friend class OwnedFont;
	};

	class FontData {
	public:
		[[nodiscard]] int width(const QString& text) const;
		[[nodiscard]] int width(
			const QString& text,
			int from,
			int to) const;
		[[nodiscard]] int width(QChar ch) const;

		[[nodiscard]] QString elided(
			const QString& str,
			int width,
			Qt::TextElideMode mode = Qt::ElideRight) const;

		[[nodiscard]] Font bold(bool set = true) const;
		[[nodiscard]] Font italic(bool set = true) const;
		[[nodiscard]] Font underline(bool set = true) const;

		[[nodiscard]] Font strikeout(bool set = true) const;
		[[nodiscard]] Font semibold(bool set = true) const;
		[[nodiscard]] Font monospace(bool set = true) const;

		[[nodiscard]] int size() const;
		[[nodiscard]] FontFlags flags() const;
		[[nodiscard]] int family() const;

		QFont f;

		int height = 0;
		int ascent = 0;

		int descent = 0;
		int spacew = 0;

		int elidew = 0;
	private:
		[[nodiscard]] Font otherFlagsFont(FontFlag flag, bool set) const;
		FontData(const FontResolveResult& data, FontVariants* modified);

		QFontMetricsF _m;
		mutable FontVariants _modified;

		int _size = 0;
		int _family = 0;

		FontFlags _flags = 0;

		friend class OwnedFont;
		friend struct ResolvedFont;
	};

	[[nodiscard]] bool operator==(const Font& a, const Font& b);
	[[nodiscard]] bool operator!=(const Font& a, const Font& b);

	class OwnedFont final {
	public:
		OwnedFont(const QString& custom, FontFlags flags, int size);
		OwnedFont(const OwnedFont& other);

		OwnedFont& operator=(const OwnedFont& other);

		[[nodiscard]] const Font& font() const;
		[[nodiscard]] FontData* operator->() const;
		[[nodiscard]] FontData* get() const;
	private:
		FontData _data;
		Font _font;
	};
} // namespace base::qt::style::internal

namespace base::qt::style{
	struct TextStyle {
		font _font;
		int lineHeight = 14;

		bool linkUnderLine = true;
		QuoteStyle blockquote = {};
	};

	inline const TextStyle* defaultTextStyle = nullptr;
} // namespace base::qt::style