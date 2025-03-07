#pragma once

#include <src/qt/text/Types.h>
#include <src/qt/text/TextEntities.h>

#include <QString>
#include <QMimeData>

#include <QGuiApplication>
#include <QClipboard>


__BASE_QT_TEXT_NAMESPACE_BEGIN

const auto kTagBold = u"**"_q;
const auto kTagItalic = u"__"_q;

const auto kTagUnderline = u"^^"_q;
const auto kTagStrikeOut = u"~~"_q;

const auto kTagCode = u"`"_q;
const auto kTagPre = u"```"_q;

const auto kTagBlockquote = u">"_q;
const auto kTagBlockquoteCollapsed = u">^"_q;
constexpr auto kTagSeparator = '\\';

inline const auto kMentionTagStart = QLatin1String("mention://");


struct TextWithTags {
	struct Tag {
		int offset = 0;
		int length = 0;
		QString id;

		friend inline auto operator<=>(const Tag&, const Tag&) = default;
		friend inline bool operator==(const Tag&, const Tag&) = default;
	};
	using Tags = QVector<Tag>;

	QString text;
	Tags tags;

	NODISCARD bool empty() const {
		return text.isEmpty();
	}

	friend inline auto operator<=>(
		const TextWithTags&,
		const TextWithTags&) = default;
	friend inline bool operator==(
		const TextWithTags&,
		const TextWithTags&) = default;
};

NODISCARD Qt::LayoutDirection Direction(
	const QString& str,
	int from,
	int to);

NODISCARD bool IsParagraphSeparator(QChar ch);
NODISCARD bool IsWordSeparator(QChar ch);

NODISCARD bool IsSpace(QChar ch);
NODISCARD bool IsNewline(QChar ch);

NODISCARD bool IsBad(QChar ch);
NODISCARD bool IsTrimmed(QChar ch);

NODISCARD bool IsDiacritic(QChar ch);
NODISCARD bool IsMentionLink(QStringView link);

NODISCARD bool IsSeparateTag(QStringView tag);
NODISCARD bool IsValidMarkdownLink(QStringView link);

namespace details {
	struct ContainerImplHelper {
		enum CutResult { 
			Null,
			Empty,
			Full,
			Subset 
		};

		NODISCARD static constexpr CutResult mid(
			qsizetype originalLength,
			qsizetype* _position,
			qsizetype* _length);
	};

struct ToUpperType {
	inline QString operator()(const QString& text) const {
		return text.toUpper();
	}
	inline QString operator()(QString&& text) const {
		return std::move(text).toUpper();
	}
};

} // namespace details

inline constexpr auto Upper = details::ToUpperType{};

NODISCARD TextWithEntities WithSingleEntity(
	const QString& text,
	EntityType type,
	const QString& data = QString());

NODISCARD TextWithEntities Bold(const QString& text);
NODISCARD TextWithEntities Semibold(const QString& text);
NODISCARD TextWithEntities Italic(const QString& text);
NODISCARD TextWithEntities Link(
	const QString& text,
	const QString& url = u"internal:action"_q);

NODISCARD TextWithEntities Link(const QString& text, int index);
NODISCARD TextWithEntities Link(
	TextWithEntities text,
	const QString& url = u"internal:action"_q);
NODISCARD TextWithEntities Link(TextWithEntities text, int index);

NODISCARD TextWithEntities Colorized(
	const QString& text,
	int index = 0);
NODISCARD TextWithEntities Colorized(
	TextWithEntities text,
	int index = 0);

NODISCARD TextWithEntities Wrapped(
	TextWithEntities text,
	EntityType type,
	const QString& data = QString());

NODISCARD TextWithEntities RichLangValue(const QString& text);
NODISCARD TextWithEntities WithEntities(const QString& text);

NODISCARD QString TagsMimeType();
NODISCARD QString TagsTextMimeType();

NODISCARD QString MentionEntityData(QStringView link);
NODISCARD QList<QStringView> SplitTags(QStringView tag);

NODISCARD QString JoinTag(const QList<QStringView>& list);
NODISCARD QString TagWithRemoved(const QString& tag, const QString& removed);
NODISCARD QString TagWithAdded(const QString& tag, const QString& added);

NODISCARD EntitiesInText ConvertTextTagsToEntities(const TextWithTags::Tags& tags);

NODISCARD QString ExpandCustomLinks(const TextWithTags& text);
NODISCARD QStringView StringViewMid(
	QStringView view,
	qsizetype pos,
	qsizetype n = -1);

NODISCARD QByteArray SerializeTags(const TextWithTags::Tags& tags);


NODISCARD TextWithTags::Tags ConvertEntitiesToTextTags(
	const EntitiesInText& entities);

std::unique_ptr<QMimeData> MimeDataFromText(
	TextWithTags&& text,
	const QString& expanded);
std::unique_ptr<QMimeData> MimeDataFromText(const TextForMimeData& text);
std::unique_ptr<QMimeData> MimeDataFromText(TextWithTags&& text);

void SetClipboardText(
	const TextForMimeData& text,
	QClipboard::Mode mode = QClipboard::Clipboard);

__BASE_QT_TEXT_NAMESPACE_END