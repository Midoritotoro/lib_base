#pragma once 

#include <QFont>
#include <QFontMetrics>

#include <base/core/Types.h>
#include <base/core/utility/Flags.h>

#include <base/qt/style/StyleTypes.h>

#include <QMap>
#include <cmath>

#include <QFontInfo>
#include <QFontDatabase>

#include <private/qfontengine_p.h>


__BASE_QT_STYLE_NAMESPACE_BEGIN

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

NODISCARD const QString& SystemFontTag();
void SetCustomFont(const QString& font);

NODISCARD QString CustomFont();
NODISCARD const FontResolveResult* FindAdjustResult(const QFont& font);

__BASE_QT_STYLE_NAMESPACE_END

__BASE_QT_STYLE_NAMESPACE_BEGIN

namespace internal {
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

		NODISCARD FontData* operator->() const;
		NODISCARD FontData* get() const;

		NODISCARD operator bool() const;
		NODISCARD operator const QFont& () const;
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
		NODISCARD int width(const QString& text) const;
		NODISCARD int width(
			const QString& text,
			int from,
			int to) const;
		NODISCARD int width(QChar ch) const;

		NODISCARD QString elided(
			const QString& str,
			int width,
			Qt::TextElideMode mode = Qt::ElideRight) const;

		NODISCARD Font bold(bool set = true) const;
		NODISCARD Font italic(bool set = true) const;
		NODISCARD Font underline(bool set = true) const;

		NODISCARD Font strikeout(bool set = true) const;
		NODISCARD Font semibold(bool set = true) const;
		NODISCARD Font monospace(bool set = true) const;

		NODISCARD int size() const;
		NODISCARD FontFlags flags() const;
		NODISCARD int family() const;

		QFont f;

		int height = 0;
		int ascent = 0;

		int descent = 0;
		int spacew = 0;

		int elidew = 0;
	private:
		NODISCARD Font otherFlagsFont(FontFlag flag, bool set) const;
		FontData(const FontResolveResult& data, FontVariants* modified);

		QFontMetricsF _m;
		mutable FontVariants _modified;

		int _size = 0;
		int _family = 0;

		FontFlags _flags = 0;

		friend class OwnedFont;
		friend struct ResolvedFont;
	};

	NODISCARD bool operator==(const Font& a, const Font& b);
	NODISCARD bool operator!=(const Font& a, const Font& b);

	class OwnedFont final {
	public:
		OwnedFont(const QString& custom, FontFlags flags, int size);
		OwnedFont(const OwnedFont& other);

		OwnedFont& operator=(const OwnedFont& other);

		NODISCARD const Font& font() const;
		NODISCARD FontData* operator->() const;
		NODISCARD FontData* get() const;
	private:
		FontData _data;
		Font _font;
	};

} // internal 

__BASE_QT_STYLE_NAMESPACE_END

__BASE_QT_STYLE_NAMESPACE_BEGIN

struct TextStyle {
	font _font;
	int lineHeight = 14;

	bool linkUnderLine = true;
	QuoteStyle blockquote = {};
};

inline const TextStyle* defaultTextStyle = nullptr;

__BASE_QT_STYLE_NAMESPACE_END