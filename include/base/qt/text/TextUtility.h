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

	base_nodiscard bool empty() const {
		return text.isEmpty();
	}

	friend inline auto operator<=>(
		const TextWithTags&,
		const TextWithTags&) = default;
	friend inline bool operator==(
		const TextWithTags&,
		const TextWithTags&) = default;
};

base_nodiscard Qt::LayoutDirection Direction(
	const QString& str,
	int from,
	int to);

base_nodiscard bool IsParagraphSeparator(QChar ch);
base_nodiscard bool IsWordSeparator(QChar ch);

base_nodiscard bool IsSpace(QChar ch);
base_nodiscard bool IsNewline(QChar ch);

base_nodiscard bool IsBad(QChar ch);
base_nodiscard bool IsTrimmed(QChar ch);

base_nodiscard bool IsDiacritic(QChar ch);
base_nodiscard bool IsMentionLink(QStringView link);

base_nodiscard bool IsSeparateTag(QStringView tag);
base_nodiscard bool IsValidMarkdownLink(QStringView link);

namespace details {
	struct ContainerImplHelper {
		enum CutResult { 
			Null,
			Empty,
			Full,
			Subset 
		};

		base_nodiscard static constexpr CutResult mid(
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

base_nodiscard TextWithEntities WithSingleEntity(
	const QString& text,
	EntityType type,
	const QString& data = QString());

base_nodiscard TextWithEntities Bold(const QString& text);
base_nodiscard TextWithEntities Semibold(const QString& text);
base_nodiscard TextWithEntities Italic(const QString& text);
base_nodiscard TextWithEntities Link(
	const QString& text,
	const QString& url = u"internal:action"_q);

base_nodiscard TextWithEntities Link(const QString& text, int index);
base_nodiscard TextWithEntities Link(
	TextWithEntities text,
	const QString& url = u"internal:action"_q);
base_nodiscard TextWithEntities Link(TextWithEntities text, int index);

base_nodiscard TextWithEntities Colorized(
	const QString& text,
	int index = 0);
base_nodiscard TextWithEntities Colorized(
	TextWithEntities text,
	int index = 0);

base_nodiscard TextWithEntities Wrapped(
	TextWithEntities text,
	EntityType type,
	const QString& data = QString());

base_nodiscard TextWithEntities RichLangValue(const QString& text);
base_nodiscard TextWithEntities WithEntities(const QString& text);

base_nodiscard QString TagsMimeType();
base_nodiscard QString TagsTextMimeType();

base_nodiscard QString MentionEntityData(QStringView link);
base_nodiscard QList<QStringView> SplitTags(QStringView tag);

base_nodiscard QString JoinTag(const QList<QStringView>& list);
base_nodiscard QString TagWithRemoved(const QString& tag, const QString& removed);
base_nodiscard QString TagWithAdded(const QString& tag, const QString& added);

base_nodiscard EntitiesInText ConvertTextTagsToEntities(const TextWithTags::Tags& tags);

base_nodiscard QString ExpandCustomLinks(const TextWithTags& text);
base_nodiscard QStringView StringViewMid(
	QStringView view,
	qsizetype pos,
	qsizetype n = -1);

base_nodiscard QByteArray SerializeTags(const TextWithTags::Tags& tags);


base_nodiscard TextWithTags::Tags ConvertEntitiesToTextTags(
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