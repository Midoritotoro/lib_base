#pragma once

#include <base/qt/common/ClickHandler.h>
#include <text/Types.h>


namespace base::qt::text {
	static constexpr TextSelection AllTextSelection = { 0, 0xFFFF };
	using EntitiesInText = QVector<EntityInText>;

	struct TextWithEntities {
		QString text;
		EntitiesInText entities;

		[[nodiscard]] bool empty() const;
		void reserve(int size, int entitiesCount = 0);

		TextWithEntities& append(TextWithEntities&& other);
		TextWithEntities& append(const TextWithEntities& other);

		TextWithEntities& append(const QString& other);
		TextWithEntities& append(QLatin1String other);
		TextWithEntities& append(QChar other);

		[[nodiscard]] static TextWithEntities Simple(const QString& simple);

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

		[[nodiscard]] bool empty() const;
		void reserve(int size, int entitiesCount = 0);

		[[nodiscard]] TextForMimeData& append(TextForMimeData&& other);
		[[nodiscard]] TextForMimeData& append(TextWithEntities&& other);
		[[nodiscard]] TextForMimeData& append(const QString& other);

		[[nodiscard]] TextForMimeData& append(QLatin1String other);
		[[nodiscard]] TextForMimeData& append(QChar other);

		[[nodiscard]] static TextForMimeData Rich(TextWithEntities&& rich);
		[[nodiscard]] static TextForMimeData Simple(const QString& simple);
	};

	class EntityInText {
	public:
		EntityInText(
			EntityType type,
			int offset,
			int length,
			const QString& data = QString());

		static [[nodiscard]] int FirstMonospaceOffset(
			const EntitiesInText& entities,
			int textLength);

		[[nodiscard]] EntityType type() const;
		[[nodiscard]] int offset() const;

		[[nodiscard]] int length() const;
		[[nodiscard]] QString data() const;

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
} // namespace base::qt::text