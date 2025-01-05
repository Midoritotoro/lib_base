#pragma once

#include <text/TextBlock.h>
#include <text/TextEntities.h>

#include <base/qt/style/StyleCore.h>
#include <text/Types.h>

#include <any>


namespace base::qt::text {
	class String;
	struct QuoteDetails;

	class BlockParser {
	public:
		BlockParser(
			not_null<String*> string,
			const TextWithEntities& textWithEntities,
			const TextParseOptions& options,
			const std::any& context);

	private:
		struct ReadyToken {
		};

		class StartedEntity {
		public:
			enum class Type {
				Flags,
				Link,
				IndexedLink,
				Colorized,
			};

			explicit StartedEntity(TextBlockFlags flags);
			explicit StartedEntity(uint16 index, Type type);

			[[nodiscard]] Type type() const;
			[[nodiscard]] std::optional<TextBlockFlags> flags() const;
			[[nodiscard]] std::optional<uint16> linkIndex() const;
			[[nodiscard]] std::optional<uint16> colorIndex() const;

		private:
			const int _value = 0;
			const Type _type;

		};

		BlockParser(
			not_null<String*> string,
			TextWithEntities&& source,
			const TextParseOptions& options,
			const std::any& context,
			ReadyToken);

		void trimSourceRange();
		void createBlock(int skipBack = 0);
		void createNewlineBlock(bool fromOriginalText);
		void ensureAtNewline(QuoteDetails quote);

		bool checkEntities();
		void parseCurrentChar();
		void finalize(const TextParseOptions& options);

		void finishEntities();
		void skipPassedEntities();
		void skipBadEntities();

		bool isInvalidEntity(const EntityInText& entity) const;
		bool isLinkEntity(const EntityInText& entity) const;

		bool processCustomIndex(uint16 index);

		void parse(const TextParseOptions& options);
		void computeLinkText(
			const QString& linkData,
			QString* outLinkText,
			EntityLinkShown* outShown);

		const not_null<String*> _t;
		QString& _tText;
		std::vector<Block>& _tBlocks;
		const TextWithEntities _source;
		const std::any& _context;
		const QChar* const _start = nullptr;
		const QChar* _end = nullptr;
		const QChar* _ptr = nullptr;
		const EntitiesInText::const_iterator _entitiesEnd;
		EntitiesInText::const_iterator _waitingEntity;
		const bool _multiline = false;

		const bool _checkTilde = false; // do we need a special text block for tilde symbol

		std::vector<uint16> _linksIndexes;

		std::vector<EntityLinkData> _links;
		std::vector<EntityLinkData> _monos;
		std::map<
			const QChar*,
			std::vector<StartedEntity>> _startedEntities;

		uint16 _maxLinkIndex = 0;
		uint16 _maxShiftedLinkIndex = 0;

		// current state
		TextBlockFlags _flags;
		uint16 _linkIndex = 0;
		uint16 _colorIndex = 0;
		uint16 _monoIndex = 0;
		uint16 _quoteIndex = 0;
		int _quoteStartPosition = 0;
		
		int _blockStart = 0; // offset in result, from which current parsed block is started
		int _diacritics = 0; // diacritic chars skipped without good char
		bool _newlineAwaited = false;
		QChar _ch; // current char (low surrogate, if current char is surrogate pair)
		bool _allowDiacritic = false; // did we add last char to the current block

	};
} // namespace base::qt::text