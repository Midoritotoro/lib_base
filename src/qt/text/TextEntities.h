#pragma once

#include <base/qt/common/ClickHandler.h>
#include <src/qt/text/Types.h>


__BASE_QT_TEXT_NAMESPACE_BEGIN

static constexpr TextSelection AllTextSelection = { 0, 0xFFFF };
using EntitiesInText = QVector<EntityInText>;

struct TextWithEntities {
	QString text;
	EntitiesInText entities;

	  bool empty() const;
	void reserve(int size, int entitiesCount = 0);

	TextWithEntities& append(TextWithEntities&& other);
	TextWithEntities& append(const TextWithEntities& other);

	TextWithEntities& append(const QString& other);
	TextWithEntities& append(QLatin1String other);
	TextWithEntities& append(QChar other);

	  static TextWithEntities Simple(const QString& simple);

	friend inline auto operator<=>(
		const TextWithEntities&,
		const TextWithEntities&) = default;
	friend inline bool operator==(
		const TextWithEntities&,
		const TextWithEntities&) = default;
};

struct TextForMimeData {
	QString expanded;
	TextWithEntities rich;

	  bool empty() const;
	void reserve(int size, int entitiesCount = 0);

	  TextForMimeData& append(TextForMimeData&& other);
	  TextForMimeData& append(TextWithEntities&& other);
	  TextForMimeData& append(const QString& other);

	  TextForMimeData& append(QLatin1String other);
	  TextForMimeData& append(QChar other);

	  static TextForMimeData Rich(TextWithEntities&& rich);
	  static TextForMimeData Simple(const QString& simple);
};

class EntityInText {
public:
	EntityInText(
		EntityType type,
		int offset,
		int length,
		const QString& data = QString());

	static   int FirstMonospaceOffset(
		const EntitiesInText& entities,
		int textLength);

	  EntityType type() const;
	  int offset() const;

	  int length() const;
	  QString data() const;

	void extendToLeft(int extent);
	void shrinkFromRight(int shrink);

	void shiftLeft(int shift);
	void shiftRight(int shift);

	void updateTextEnd(int textEnd);
	explicit operator bool() const {
		return type() != EntityType::Invalid;
	}
private:
	EntityType _type = EntityType::Invalid;

	int _offset = 0;
	int _length = 0;

	QString _data;
};

struct EntityLinkData {
	QString text;
	QString data;

	EntityType type = EntityType::Invalid;
	EntityLinkShown shown = EntityLinkShown::Full;

	friend inline auto operator<=>(
		const EntityLinkData&,
		const EntityLinkData&) = default;
	friend inline bool operator==(
		const EntityLinkData&,
		const EntityLinkData&) = default;
};

__BASE_QT_TEXT_NAMESPACE_END
