#pragma once

#include <text/Types.h>
#include <text/TextEntities.h>

#include <QString>
#include <QMimeData>

#include <QGuiApplication>
#include <QClipboard>


namespace base::qt::text {
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

		[[nodiscard]] bool empty() const {
			return text.isEmpty();
		}

		friend inline auto operator<=>(
			const TextWithTags&,
			const TextWithTags&) = default;
		friend inline bool operator==(
			const TextWithTags&,
			const TextWithTags&) = default;
	};

	[[nodiscard]] Qt::LayoutDirection Direction(
		const QString& str,
		int from,
		int to);

	[[nodiscard]] bool IsParagraphSeparator(QChar ch);
	[[nodiscard]] bool IsWordSeparator(QChar ch);

	[[nodiscard]] bool IsSpace(QChar ch);
	[[nodiscard]] bool IsNewline(QChar ch);

	[[nodiscard]] bool IsBad(QChar ch);
	[[nodiscard]] bool IsTrimmed(QChar ch);

	[[nodiscard]] bool IsDiacritic(QChar ch);
	[[nodiscard]] bool IsMentionLink(QStringView link);

	[[nodiscard]] bool IsSeparateTag(QStringView tag);
	[[nodiscard]] bool IsValidMarkdownLink(QStringView link);

	namespace details {
		struct ContainerImplHelper {
			enum CutResult { 
				Null,
				Empty,
				Full,
				Subset 
			};

			[[nodiscard]] static constexpr CutResult mid(
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

	[[nodiscard]] TextWithEntities WithSingleEntity(
		const QString& text,
		EntityType type,
		const QString& data = QString());

	[[nodiscard]] TextWithEntities Bold(const QString& text);
	[[nodiscard]] TextWithEntities Semibold(const QString& text);
	[[nodiscard]] TextWithEntities Italic(const QString& text);
	[[nodiscard]] TextWithEntities Link(
		const QString& text,
		const QString& url = u"internal:action"_q);

	[[nodiscard]] TextWithEntities Link(const QString& text, int index);
	[[nodiscard]] TextWithEntities Link(
		TextWithEntities text,
		const QString& url = u"internal:action"_q);
	[[nodiscard]] TextWithEntities Link(TextWithEntities text, int index);

	[[nodiscard]] TextWithEntities Colorized(
		const QString& text,
		int index = 0);
	[[nodiscard]] TextWithEntities Colorized(
		TextWithEntities text,
		int index = 0);

	[[nodiscard]] TextWithEntities Wrapped(
		TextWithEntities text,
		EntityType type,
		const QString& data = QString());

	[[nodiscard]] TextWithEntities RichLangValue(const QString& text);
	[[nodiscard]] TextWithEntities WithEntities(const QString& text);

	[[nodiscard]] QString TagsMimeType();
	[[nodiscard]] QString TagsTextMimeType();

	[[nodiscard]] QString MentionEntityData(QStringView link);
	[[nodiscard]] QList<QStringView> SplitTags(QStringView tag);

	[[nodiscard]] QString JoinTag(const QList<QStringView>& list);
	[[nodiscard]] QString TagWithRemoved(const QString& tag, const QString& removed);
	[[nodiscard]] QString TagWithAdded(const QString& tag, const QString& added);

	[[nodiscard]] EntitiesInText ConvertTextTagsToEntities(const TextWithTags::Tags& tags);

	[[nodiscard]] QString ExpandCustomLinks(const TextWithTags& text);
	[[nodiscard]] QStringView StringViewMid(
		QStringView view,
		qsizetype pos,
		qsizetype n = -1);

	[[nodiscard]] QByteArray SerializeTags(const TextWithTags::Tags& tags);


	[[nodiscard]] TextWithTags::Tags ConvertEntitiesToTextTags(
		const EntitiesInText& entities);

	std::unique_ptr<QMimeData> MimeDataFromText(
		TextWithTags&& text,
		const QString& expanded);
	std::unique_ptr<QMimeData> MimeDataFromText(const TextForMimeData& text);
	std::unique_ptr<QMimeData> MimeDataFromText(TextWithTags&& text);

	void SetClipboardText(
		const TextForMimeData& text,
		QClipboard::Mode mode = QClipboard::Clipboard);

} // namespace base::qt::text
