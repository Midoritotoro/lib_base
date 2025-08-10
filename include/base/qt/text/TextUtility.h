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

	  bool empty() const {
		return text.isEmpty();
	}

	friend inline auto operator<=>(
		const TextWithTags&,
		const TextWithTags&) = default;
	friend inline bool operator==(
		const TextWithTags&,
		const TextWithTags&) = default;
};

  Qt::LayoutDirection Direction(
	const QString& str,
	int from,
	int to);

  bool IsParagraphSeparator(QChar ch);
  bool IsWordSeparator(QChar ch);

  bool IsSpace(QChar ch);
  bool IsNewline(QChar ch);

  bool IsBad(QChar ch);
  bool IsTrimmed(QChar ch);

  bool IsDiacritic(QChar ch);
  bool IsMentionLink(QStringView link);

  bool IsSeparateTag(QStringView tag);
  bool IsValidMarkdownLink(QStringView link);

namespace details {
	struct ContainerImplHelper {
		enum CutResult { 
			Null,
			Empty,
			Full,
			Subset 
		};

		  static constexpr CutResult mid(
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

  TextWithEntities WithSingleEntity(
	const QString& text,
	EntityType type,
	const QString& data = QString());

  TextWithEntities Bold(const QString& text);
  TextWithEntities Semibold(const QString& text);
  TextWithEntities Italic(const QString& text);
  TextWithEntities Link(
	const QString& text,
	const QString& url = u"internal:action"_q);

  TextWithEntities Link(const QString& text, int index);
  TextWithEntities Link(
	TextWithEntities text,
	const QString& url = u"internal:action"_q);
  TextWithEntities Link(TextWithEntities text, int index);

  TextWithEntities Colorized(
	const QString& text,
	int index = 0);
  TextWithEntities Colorized(
	TextWithEntities text,
	int index = 0);

  TextWithEntities Wrapped(
	TextWithEntities text,
	EntityType type,
	const QString& data = QString());

  TextWithEntities RichLangValue(const QString& text);
  TextWithEntities WithEntities(const QString& text);

  QString TagsMimeType();
  QString TagsTextMimeType();

  QString MentionEntityData(QStringView link);
  QList<QStringView> SplitTags(QStringView tag);

  QString JoinTag(const QList<QStringView>& list);
  QString TagWithRemoved(const QString& tag, const QString& removed);
  QString TagWithAdded(const QString& tag, const QString& added);

  EntitiesInText ConvertTextTagsToEntities(const TextWithTags::Tags& tags);

  QString ExpandCustomLinks(const TextWithTags& text);
  QStringView StringViewMid(
	QStringView view,
	qsizetype pos,
	qsizetype n = -1);

  QByteArray SerializeTags(const TextWithTags::Tags& tags);


  TextWithTags::Tags ConvertEntitiesToTextTags(
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