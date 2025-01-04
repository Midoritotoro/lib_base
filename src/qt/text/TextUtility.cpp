#include "TextUtility.h"

#include "../../core/CoreUtility.h"
#include <QIODevice>

#include <QRegularExpression>
#include <ranges>

#include "TextWord.h"


Qt::LayoutDirection text::Direction(
	const QString& str,
	int from,
	int to) 
{
	auto pointer = reinterpret_cast<const ushort*>(str.unicode()) + from;
	const auto end = pointer + (to - from);

	while (pointer < end) {
		uint ucs4 = *pointer;

		if (QChar::isHighSurrogate(ucs4) && pointer < end - 1) {
			const auto low = pointer[1];
			if (QChar::isLowSurrogate(low)) {
				ucs4 = QChar::surrogateToUcs4(ucs4, low);
				++pointer;
			}
		}

		switch (QChar::direction(ucs4)) {
			case QChar::DirL:
				return Qt::LeftToRight;
			case QChar::DirR:
			case QChar::DirAL:
				return Qt::RightToLeft;
			default:
				break;
			}
		++pointer;
	}
	return Qt::LayoutDirectionAuto;
}

bool text::IsParagraphSeparator(QChar ch) {
	switch (ch.unicode()) {
		case QChar::LineFeed:
			return true;
		default:
			break;
	}
	return false;
}

bool text::IsWordSeparator(QChar ch) {
	switch (ch.unicode()) {
		case QChar::Space:
		case QChar::LineFeed:
		case '.':
		case ',':
		case '?':
		case '!':
		case '@':
		case '#':
		case '$':
		case ':':
		case ';':
		case '-':
		case '<':
		case '>':
		case '[':
		case ']':
		case '(':
		case ')':
		case '{':
		case '}':
		case '=':
		case '/':
		case '+':
		case '%':
		case '&':
		case '^':
		case '*':
		case '\'':
		case '"':
		case '`':
		case '~':
		case '|':
			return true;
		default:
			break;
	}
	return false;
}

bool text::IsSpace(QChar ch) {
	return ch.isSpace()
		|| (ch.unicode() < 32)
		|| (ch == QChar::ParagraphSeparator)
		|| (ch == QChar::LineSeparator)
		|| (ch == QChar::ObjectReplacementCharacter)
		|| (ch == QChar::CarriageReturn)
		|| (ch == QChar::Tabulation);
}

bool text::IsNewline(QChar ch) {
	return (ch == QChar::LineFeed)
		|| (ch.unicode() == 156);
}

bool text::IsBad(QChar ch) {
	return (ch.unicode() == 0)
		|| (ch.unicode() >= 8232 && ch.unicode() < 8237)
		|| (ch.unicode() >= 65024 && ch.unicode() < 65040 && ch.unicode() != 65039)
		|| (ch.unicode() >= 127 && ch.unicode() < 160 && ch.unicode() != 156);
}

bool text::IsTrimmed(QChar ch) {
	return IsSpace(ch)
		|| IsBad(ch)
		|| (ch == QChar(8203));
}

bool text::IsDiacritic(QChar ch) {
	return (ch.category() == QChar::Mark_NonSpacing)
		|| (ch.unicode() == 1652)
		|| (ch.unicode() >= 64606 && ch.unicode() <= 64611);
}

bool text::IsMentionLink(QStringView link) {
	return link.startsWith(kMentionTagStart);
}

bool text::IsSeparateTag(QStringView tag) {
	return (tag == kTagCode)
		|| (tag == kTagPre);
}

bool text::IsValidMarkdownLink(QStringView link) {
	return (link.indexOf('.') >= 0) || (link.indexOf(':') >= 0);
}

QStringView text::StringViewMid(
	QStringView view,
	qsizetype pos,
	qsizetype n) 
{
	const auto result = details::ContainerImplHelper::mid(
		view.size(),
		&pos,
		&n);

	return (result == details::ContainerImplHelper::Null)
		? QStringView()
		: view.mid(pos, n);
}

QString text::MentionEntityData(QStringView link) {
	const auto match = QRegularExpression("^(\\d+\\.\\d+:\\d+)(/|$)")
		.match(StringViewMid(link, kMentionTagStart.size()));
	return match.hasMatch() ? match.captured(1) : QString();
}

text::EntitiesInText text::ConvertTextTagsToEntities(const TextWithTags::Tags& tags) {
	auto result = EntitiesInText();
	if (tags.isEmpty())
		return result;

	constexpr auto kInMaskTypesInline = std::array{
		EntityType::Bold,
		EntityType::Italic,
		EntityType::Underline,
		EntityType::StrikeOut,
		EntityType::Spoiler,
		EntityType::Code,
	};

	constexpr auto kInMaskTypesBlock = std::array{
		EntityType::Pre,
		EntityType::Blockquote,
	};

	struct State {
		QString link;
		QString language;
		uint32 mask : 31 = 0;
		uint32 collapsed : 1 = 0;

		void set(EntityType type) {
			mask |= (1 << int(type));
		}
		void remove(EntityType type) {
			mask &= ~(1 << int(type));
		}
		[[nodiscard]] bool has(EntityType type) const {
			return (mask & (1 << int(type)));
		}
	};

	auto offset = 0;
	auto state = State();
	auto notClosedEntities = std::vector<int>(); // Stack of indices.
	const auto closeOne = [&] {
		Expects(!notClosedEntities.empty());

		auto& entity = result[notClosedEntities.back()];
		const auto type = entity.type();
		entity = {
			type,
			entity.offset(),
			offset - entity.offset(),
			entity.data(),
		};
		if (std::ranges::contains(kInMaskTypesInline, type)
			|| std::ranges::contains(kInMaskTypesBlock, type)) {
			state.remove(entity.type());
		}
		else {
			state.link = QString();
		}
		notClosedEntities.pop_back();
		};
	const auto closeType = [&](EntityType type) {
		auto closeCount = 0;
		const auto notClosedCount = notClosedEntities.size();
		while (closeCount < notClosedCount) {
			const auto index = notClosedCount - closeCount - 1;
			if (result[notClosedEntities[index]].type() == type) {
				for (auto i = 0; i != closeCount + 1; ++i) {
					closeOne();
				}
				break;
			}
			++closeCount;
		}
		};
	const auto openType = [&](EntityType type, const QString& data = {}) {
		notClosedEntities.push_back(result.size());
		result.push_back({ type, offset, -1, data });
		};

	const auto processState = [&](State nextState) {
		const auto linkChanged = (nextState.link != state.link);
		const auto closeLink = linkChanged && !state.link.isEmpty();
		for (const auto type : kInMaskTypesInline) {
			if (state.has(type) && !nextState.has(type)) {
				closeType(type);
			}
		}
		if (closeLink) {
			if (IsMentionLink(state.link)) {
				closeType(EntityType::MentionName);
			}
			else {
				closeType(EntityType::CustomUrl);
			}
		}
		for (const auto type : kInMaskTypesBlock) {
			if (state.has(type) && !nextState.has(type)) {
				closeType(type);
			}
		}

		const auto openLink = linkChanged && !nextState.link.isEmpty();
		for (const auto type : kInMaskTypesBlock | std::ranges::views::reverse) {
			if (nextState.has(type) && !state.has(type)) {
				openType(type, (type == EntityType::Pre)
					? nextState.language
					: (type == EntityType::Blockquote && nextState.collapsed)
					? u"1"_q
					: QString());
			}
		}
		if (openLink) {
			if (IsMentionLink(nextState.link)) {
				const auto data = MentionEntityData(nextState.link);
				if (!data.isEmpty()) {
					openType(EntityType::MentionName, data);
				}
			}
			else {
				openType(EntityType::CustomUrl, nextState.link);
			}
		}
		for (const auto type : kInMaskTypesInline | std::ranges::views::reverse) {
			if (nextState.has(type) && !state.has(type)) {
				openType(type);
			}
		}
		state = nextState;
		};
	const auto stateForTag = [&](const QString& tag) {
		auto result = State();
		const auto list = tag.split(kTagSeparator);
		const auto languageStart = kTagPre.size();
		for (const auto& single : list) {
			if (single == kTagBold) {
				result.set(EntityType::Bold);
			}
			else if (single == kTagItalic) {
				result.set(EntityType::Italic);
			}
			else if (single == kTagUnderline) {
				result.set(EntityType::Underline);
			}
			else if (single == kTagStrikeOut) {
				result.set(EntityType::StrikeOut);
			}
			else if (single == kTagCode) {
				result.set(EntityType::Code);
			}
			else if (single == kTagPre) {
				result.set(EntityType::Pre);
			}
			else if (single.size() > languageStart
				&& single.startsWith(kTagPre)) {
				result.set(EntityType::Pre);
				result.language = single.mid(languageStart);
			}
			else if (single == kTagBlockquote) {
				result.set(EntityType::Blockquote);
				result.collapsed = 0;
			}
			else if (single == kTagBlockquoteCollapsed) {
				result.set(EntityType::Blockquote);
				result.collapsed = 1;
			}
			else {
				result.link = single;
			}
		}
		return result;
		};
	
	for (const auto& tag : tags) {
		if (tag.offset > offset) {
			processState(State());
		}
		offset = tag.offset;
		processState(stateForTag(tag.id));
		offset += tag.length;
	}
	processState(State());

	result.erase(std::ranges::find_if(result, [](const EntityInText& entity) {
		return (entity.length() <= 0);
		}), result.end());

	return result;
}

QString text::ExpandCustomLinks(const TextWithTags& text) {
	const auto entities = ConvertTextTagsToEntities(text.tags);
	auto&& urls = std::ranges::subrange(
		entities.begin(),
		entities.end()
	) | std::ranges::views::filter([](const EntityInText& entity) {
		return entity.type() == EntityType::CustomUrl;
		});
	const auto& original = text.text;
	if (urls.begin() == urls.end()) {
		return original;
	}
	auto result = QString();
	auto offset = 0;
	for (const auto& entity : urls) {
		const auto till = entity.offset() + entity.length();
		if (till > offset) {
			result.append(StringViewMid(original, offset, till - offset));
		}
		result.append(QLatin1String(" (")).append(entity.data()).append(')');
		offset = till;
	}
	if (original.size() > offset) {
		result.append(StringViewMid(original, offset));
	}
	return result;
}

QString text::JoinTag(const QList<QStringView>& list) {
	if (list.isEmpty()) {
		return QString();
	}
	auto length = (list.size() - 1);
	for (const auto& entry : list) {
		length += entry.size();
	}
	auto result = QString();
	result.reserve(length);
	result.append(list.front());
	for (auto i = 1, count = int(list.size()); i != count; ++i) {
		if (!IsSeparateTag(list[i])) {
			result.append(kTagSeparator).append(list[i]);
		}
	}
	return result;
}

QList<QStringView> text::SplitTags(QStringView tag) {
	return tag.split(kTagSeparator);
}

QString text::TagWithRemoved(const QString& tag, const QString& removed) {
	if (tag == removed) {
		return QString();
	}
	auto list = SplitTags(tag);
	list.erase(std::ranges::find(list, QStringView(removed)), list.end());
	return JoinTag(list);
}

QString text::TagWithAdded(const QString& tag, const QString& added) {
	if (tag.isEmpty() || tag == added) {
		return added;
	}
	auto list = SplitTags(tag);
	const auto ref = QStringView(added);
	if (list.contains(ref)) {
		return tag;
	}
	list.push_back(ref);
	std::sort(list.begin(), list.end());
	return JoinTag(list);
}


text::TextWithTags::Tags text::ConvertEntitiesToTextTags(
	const EntitiesInText& entities) {
	auto result = TextWithTags::Tags();
	if (entities.isEmpty()) {
		return result;
	}

	auto offset = 0;
	auto current = QString();
	const auto updateCurrent = [&](int nextOffset, const QString& next) {
		if (next == current) {
			return;
		}
		else if (nextOffset > offset) {
			if (!current.isEmpty()) {
				result.push_back({ offset, nextOffset - offset, current });
			}
			offset = nextOffset;
		}
		current = next;
		};
	auto toRemove = std::vector<std::pair<int, QString>>();
	const auto removeTill = [&](int nextOffset) {
		while (!toRemove.empty() && toRemove.front().first <= nextOffset) {
			updateCurrent(
				toRemove.front().first,
				TagWithRemoved(current, toRemove.front().second));
			toRemove.erase(toRemove.begin());
		}
		};
	for (const auto& entity : entities) {
		const auto push = [&](const QString& tag) {
			removeTill(entity.offset());
			updateCurrent(entity.offset(), TagWithAdded(current, tag));
			toRemove.push_back({ offset + entity.length(), tag });
			std::ranges::sort(toRemove);
			};
		switch (entity.type()) {
		case EntityType::MentionName: {
			static const auto RegExp = QRegularExpression(
				"^(\\d+\\.\\d+:\\d+)$"
			);
			const auto match = RegExp.match(entity.data());
			if (match.hasMatch()) {
				push(kMentionTagStart + entity.data());
			}
		} break;
		case EntityType::CustomUrl: {
			const auto url = entity.data();
			if (IsValidMarkdownLink(url)
				&& !IsMentionLink(url)) {
				push(url);
			}
		} break;
		case EntityType::Bold: push(kTagBold); break;
		case EntityType::Italic: push(kTagItalic); break;
		case EntityType::Underline:
			push(kTagUnderline);
			break;
		case EntityType::StrikeOut:
			push(kTagStrikeOut);
			break;
		case EntityType::Code: push(kTagCode); break;
		case EntityType::Pre: {
			if (!entity.data().isEmpty()) {
				static const auto Language = QRegularExpression("^[a-zA-Z0-9\\-\\+]+$");
				if (Language.match(entity.data()).hasMatch()) {
					push(kTagPre + entity.data());
					break;
				}
			}
			push(kTagPre);
		} break;
		case EntityType::Blockquote:
			push(entity.data().isEmpty()
				? kTagBlockquote
				: kTagBlockquoteCollapsed);
			break;
		}
	}
	if (!toRemove.empty()) {
		removeTill(toRemove.back().first);
	}
	return result;
}

QByteArray text::SerializeTags(const TextWithTags::Tags& tags) {
	if (tags.isEmpty()) {
		return QByteArray();
	}

	QByteArray tagsSerialized;
	{
		QDataStream stream(&tagsSerialized, QIODevice::WriteOnly);
		stream.setVersion(QDataStream::Qt_5_1);
		stream << qint32(tags.size());
		for (const auto& tag : tags) {
			stream << qint32(tag.offset) << qint32(tag.length) << tag.id;
		}
	}
	return tagsSerialized;
}

constexpr text::details::ContainerImplHelper::CutResult text::details::ContainerImplHelper::mid(
	qsizetype originalLength,
	qsizetype* _position,
	qsizetype* _length) 
{
	qsizetype& position = *_position;
	qsizetype& length = *_length;

	if (position > originalLength) {
		position = 0;
		length = 0;
		return Null;
	}

	if (position < 0) {
		if (length < 0 || length + position >= originalLength) {
			position = 0;
			length = originalLength;
			return Full;
		}
		if (length + position <= 0) {
			position = length = 0;
			return Null;
		}
		length += position;
		position = 0;
	}
	else if (size_t(length) > size_t(originalLength - position))
		length = originalLength - position;

	if (position == 0 && length == originalLength)
		return Full;

	return length > 0 ? Subset : Empty;
}

text::TextWithEntities text::WithSingleEntity(
	const QString& text,
	EntityType type,
	const QString& data) 
{
	auto result = TextWithEntities{ text };
	result.entities.push_back({ type, 0, int(text.size()), data });

	return result;
}

text::TextWithEntities text::Bold(const QString& text) {
	return WithSingleEntity(text, EntityType::Bold);
}

text::TextWithEntities text::Semibold(const QString& text) {
	return WithSingleEntity(text, EntityType::Semibold);
}

text::TextWithEntities text::Italic(const QString& text) {
	return WithSingleEntity(text, EntityType::Italic);
}

text::TextWithEntities text::Link(const QString& text, const QString& url) {
	return WithSingleEntity(text, EntityType::CustomUrl, url);
}

text::TextWithEntities text::Link(const QString& text, int index) {
	return Link(text, u"internal:index"_q + QChar(index));
}

text::TextWithEntities text::Link(TextWithEntities text, const QString& url) {
	return Wrapped(std::move(text), EntityType::CustomUrl, url);
}

text::TextWithEntities text::Link(TextWithEntities text, int index) {
	return Link(std::move(text), u"internal:index"_q + QChar(index));
}

text::TextWithEntities text::Colorized(const QString& text, int index) {
	const auto data = index ? QString(QChar(index)) : QString();
	return WithSingleEntity(text, EntityType::Colorized, data);
}

text::TextWithEntities text::Colorized(TextWithEntities text, int index) {
	const auto data = index ? QString(QChar(index)) : QString();
	return Wrapped(std::move(text), EntityType::Colorized, data);
}

text::TextWithEntities text::Wrapped(
	TextWithEntities text,
	EntityType type,
	const QString& data) {
	text.entities.insert(
		text.entities.begin(),
		{ type, 0, int(text.text.size()), data });
	return text;
}

text::TextWithEntities text::RichLangValue(const QString & text) {
	static const auto kStart = QRegularExpression("(\\*\\*|__)");

	auto result = TextWithEntities();
	auto offset = 0;
	while (offset < text.size()) {
		const auto m = kStart.match(text, offset);
		if (!m.hasMatch()) {
			result.text.append(StringViewMid(text, offset));
			break;
		}
		const auto position = m.capturedStart();
		const auto from = m.capturedEnd();
		const auto tag = m.capturedView();
		const auto till = text.indexOf(tag, from + 1);
		if (till <= from) {
			offset = from;
			continue;
		}
		if (position > offset) {
			result.text.append(StringViewMid(text, offset, position - offset));
		}
		const auto type = (tag == QLatin1String("__"))
			? EntityType::Italic
			: EntityType::Bold;
		result.entities.push_back({ type, int(result.text.size()), int(till - from) });
		result.text.append(StringViewMid(text, from, till - from));
		offset = till + tag.size();
	}
	return result;
}


QString text::TagsMimeType() {
	return QString::fromLatin1("application/x-td-field-tags");
}

QString text::TagsTextMimeType() {
	return QString::fromLatin1("application/x-td-field-text");
}

std::unique_ptr<QMimeData> text::MimeDataFromText(
	TextWithTags&& text,
	const QString& expanded)
{
	if (expanded.isEmpty())
		return nullptr;

	auto result = std::make_unique<QMimeData>();
	result->setText(expanded);
	if (!text.tags.isEmpty()) {
		result->setData(
			TagsTextMimeType(),
			text.text.toUtf8());
		result->setData(
			TagsMimeType(),
			SerializeTags(text.tags));
	}

	return result;
}

std::unique_ptr<QMimeData> text::MimeDataFromText(const TextForMimeData& text) {
	qDebug() << "text.rich.entities: ";
	for (int i = 0; i < text.rich.entities.size(); ++i)
		qDebug() << text.rich.entities[i].data();

	qDebug() << "text.rich.text: " << text.rich.text;
	qDebug() << "text.expanded: " << text.expanded;

	return MimeDataFromText(
		{ 
			text.rich.text,
			ConvertEntitiesToTextTags(text.rich.entities) 
		},
		text.expanded);
}

std::unique_ptr<QMimeData> text::MimeDataFromText(TextWithTags&& text) {
	const auto expanded = ExpandCustomLinks(text);
	return MimeDataFromText(std::move(text), expanded);
}

void text::SetClipboardText(
	const TextForMimeData& text,
	QClipboard::Mode mode) 
{
	if (auto data = MimeDataFromText(text)) {
		qDebug() << "data->Text: " << data->text();
		QGuiApplication::clipboard()->setMimeData(data.release(), mode);
	}
}