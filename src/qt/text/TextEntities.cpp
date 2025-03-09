#include <src/qt/text/TextEntities.h>

#include <base/qt/text/String.h>
#include <base/core/utility/Algorithm.h>


__BASE_QT_TEXT_NAMESPACE_BEGIN

bool TextWithEntities::empty() const {
	return text.isEmpty();
}

void TextWithEntities::reserve(int size, int entitiesCount) {
	text.reserve(size);
	entities.reserve(entitiesCount);
}

TextWithEntities& TextWithEntities::append(TextWithEntities&& other) {
	const auto shift = text.size();
	for (auto& entity : other.entities) {
		entity.shiftRight(shift);
	}
	text.append(other.text);
	entities.append(other.entities);
	return *this;
}

TextWithEntities& TextWithEntities::append(const TextWithEntities& other) {
	const auto shift = text.size();
	text.append(other.text);
	entities.reserve(entities.size() + other.entities.size());
	for (auto entity : other.entities) {
		entity.shiftRight(shift);
		entities.append(entity);
	}
	return *this;
}

TextWithEntities& TextWithEntities::append(const QString& other) {
	TextWithEntities::text.append(other);
	return *this;
}

TextWithEntities& TextWithEntities::append(QLatin1String other) {
	text.append(other);
	return *this;
}

TextWithEntities& TextWithEntities::append(QChar other) {
	text.append(other);
	return *this;
}

TextWithEntities TextWithEntities::Simple(const QString& simple) {
	auto result = TextWithEntities();
	result.text = simple;
	return result;
}

bool TextForMimeData::empty() const {
	return expanded.isEmpty();
}

void TextForMimeData::reserve(int size, int entitiesCount) {
	expanded.reserve(size);
	rich.reserve(size, entitiesCount);
}

TextForMimeData& TextForMimeData::append(TextForMimeData&& other) {
	expanded.append(other.expanded);
	rich.append(std::move(other.rich));
	return *this;
}

TextForMimeData& TextForMimeData::append(TextWithEntities&& other) {
	expanded.append(other.text);
	rich.append(std::move(other));
	return *this;
}

TextForMimeData& TextForMimeData::append(const QString& other) {
	expanded.append(other);
	rich.append(other);
	return *this;
}

TextForMimeData& TextForMimeData::append(QLatin1String other) {
	expanded.append(other);
	rich.append(other);
	return *this;
}

TextForMimeData& TextForMimeData::append(QChar other) {
	expanded.append(other);
	rich.append(other);
	return *this;
}

TextForMimeData TextForMimeData::Rich(TextWithEntities&& rich) {
	auto result = TextForMimeData();
	result.expanded = rich.text;
	result.rich = std::move(rich);
	return result;
}

TextForMimeData TextForMimeData::Simple(const QString& simple) {
	auto result = TextForMimeData();
	result.expanded = result.rich.text = simple;
	return result;
}

EntityInText::EntityInText(
	EntityType type,
	int offset,
	int length,
	const QString& data
) :
	_type(type)
	, _offset(offset)
	, _length(length)
	, _data(data)
{}

EntityType EntityInText::type() const {
	return _type;
}

int EntityInText::offset() const {
	return _offset;
}

int EntityInText::length() const {
	return _length;
}

QString EntityInText::data() const {
	return _data;
}

void EntityInText::extendToLeft(int extent) {
	_offset -= extent;
	_length += extent;
}

void EntityInText::shrinkFromRight(int shrink) {
	_length -= shrink;
}

void EntityInText::shiftLeft(int shift) {
	_offset -= shift;

	if (_offset < 0) {
		_length += _offset;
		_offset = 0;

		if (_length < 0)
			_length = 0;
	}
}

void EntityInText::shiftRight(int shift) {
	_offset += shift;
}

void EntityInText::updateTextEnd(int textEnd) {
	if (_offset > textEnd) {
		_offset = textEnd;
		_length = 0;
	}
	else if (_offset + _length > textEnd)
		_length = textEnd - _offset;
}

int EntityInText::FirstMonospaceOffset(
	const EntitiesInText& entities,
	int textLength) 
{
	auto&& monospace = std::ranges::subrange(
		entities.begin(),
		entities.end()
	) | std::ranges::views::filter([](const EntityInText& entity) {
		return (entity.type() == EntityType::Pre)
			|| (entity.type() == EntityType::Code);
		});

	const auto i = std::ranges::max_element(
		monospace,
		std::greater<>(),
		&EntityInText::offset);

	return (i == monospace.end()) ? textLength : i->offset();
}

__BASE_QT_TEXT_NAMESPACE_END