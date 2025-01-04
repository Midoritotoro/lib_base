#pragma once

#include "Types.h"
#include "TextStackEngine.h"


namespace text {
	class WordParser {
	public:
		explicit WordParser(not_null<String*> string);
	private:
		struct ScriptLine {
			int length = 0;
			QFixed textWidth;
		};

		struct LineBreakHelper {
			ScriptLine tmpData;
			ScriptLine spaceData;

			QGlyphLayout glyphs;

			int glyphCount = 0;
			int maxGlyphs = INT_MAX;
			int currentPosition = 0;

			glyph_t previousGlyph = 0;
			QExplicitlySharedDataPointer<QFontEngine> previousGlyphFontEngine;

			QFixed rightBearing;

			QExplicitlySharedDataPointer<QFontEngine> fontEngine;
			const unsigned short* logClusters = nullptr;

			bool whiteSpaceOrObject = true;

			glyph_t currentGlyph() const;

			void saveCurrentGlyph();
			void calculateRightBearing(QFontEngine* engine, glyph_t glyph);

			void calculateRightBearing();
			void calculateRightBearingForPreviousGlyph();

			QFixed negativeRightBearing() const;

		};
		struct BidiInitedAnalysis {
			explicit BidiInitedAnalysis(not_null<String*> text);

			QVarLengthArray<QScriptAnalysis, 4096> list;
		};

		void parse();

		const QCharAttributes* moveToNewItemGetAttributes();

		void pushAccumulatedWord();
		void processSingleGlyphItem(QFixed added = 0);

		void wordProcessed(int nextWordStart, bool spaces = false);
		void wordContinued(int nextPartStart, bool spaces = false);

		void accumulateWhitespaces();
		void ensureWordForRightPadding();
		void maybeStartUnfinishedWord();

		void pushFinishedWord(uint16 position, QFixed width, QFixed rbearing);
		void pushUnfinishedWord(uint16 position, QFixed width, QFixed rbearing);
		void pushNewline(uint16 position, int newlineBlockIndex);

		void addNextCluster(
			int& pos,
			int end,
			ScriptLine& line,
			int& glyphCount,
			const QScriptItem& current,
			const unsigned short* logClusters,
			const QGlyphLayout& glyphs);

		[[nodiscard]] bool isLineBreak(
			const QCharAttributes* attributes,
			int index) const;
		[[nodiscard]] bool isSpaceBreak(
			const QCharAttributes* attributes,
			int index) const;

		const not_null<String*> _t;

		QString& _tText;

		std::vector<Block>& _tBlocks;
		std::vector<Word>& _tWords;

		BidiInitedAnalysis _analysis;
		StackEngine _engine;

		QTextEngine& _e;
		LineBreakHelper _lbh;

		const QCharAttributes* _attributes = nullptr;

		int _wordStart = 0;

		bool _addingEachGrapheme = false;
		int _lastGraphemeBoundaryPosition = -1;

		ScriptLine _lastGraphemeBoundaryLine;

		int _item = -1;
		int _newItem = -1;
		int _itemEnd = 0;
	};
} // namespace text
