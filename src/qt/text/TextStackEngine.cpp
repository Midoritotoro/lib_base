#include <src/qt/text/TextStackEngine.h>

#include <src/qt/text/TextBlock.h>
#include <base/qt/text/String.h>

__BASE_QT_TEXT_NAMESPACE_BEGIN
namespace {
	constexpr auto kMaxItemLength = 4096;
} // namespace

StackEngine::StackEngine(
	not_null<const String*> t,
	gsl::span<QScriptAnalysis> analysis,
	int from,
	int till,
	int blockIndexHint)
	: StackEngine(
		t,
		from,
		((from > 0 || (till >= 0 && till < t->_text.size()))
			? QString::fromRawData(
				t->_text.constData() + from,
				((till < 0) ? int(t->_text.size()) : till) - from)
			: t->_text),
		analysis,
		blockIndexHint) 
{}

StackEngine::StackEngine(
	not_null<const String*> t,
	int offset,
	const QString& text,
	gsl::span<QScriptAnalysis> analysis,
	int blockIndexHint,
	int blockIndexLimit
)
	: _t(t)
	, _text(text)
	, _analysis(analysis.data())
	, _offset(offset)
	, _positionEnd(_offset + _text.size())
	, _font(_t->_st->_font)
	, _engine(_text, _font->f)
	, _tBlocks(_t->_blocks)
	, _bStart(begin(_tBlocks) + blockIndexHint)
	, _bEnd((blockIndexLimit >= 0)
		? (begin(_tBlocks) + blockIndexLimit)
		: end(_tBlocks))
	, _bCached(_bStart) 
{
	Expects(analysis.size() >= _text.size());

	_engine.validate();
	itemize();
}

std::vector<Block>::const_iterator StackEngine::adjustBlock(
	int offset) const 
{
	Expects(offset < _positionEnd);

	if (blockPosition(_bCached) > offset)
		_bCached = begin(_tBlocks);

	assert(_bCached != end(_tBlocks));
	for (auto i = _bCached + 1; blockPosition(i) <= offset; ++i)
		_bCached = i;

	return _bCached;
}

int StackEngine::blockPosition(std::vector<Block>::const_iterator i) const {
	return (i == _bEnd) ? _positionEnd : (*i)->position();
}

int StackEngine::blockEnd(std::vector<Block>::const_iterator i) const {
	return (i == _bEnd) ? _positionEnd : blockPosition(i + 1);
}

void StackEngine::itemize() {
	const auto layoutData = _engine.layoutData;
	if (layoutData->items.size())
		return;

	const auto length = layoutData->string.length();
	if (!length)
		return;

	_bStart = adjustBlock(_offset);
	const auto chars = _engine.layoutData->string.constData();

	{

		QUnicodeTools::ScriptItemArray scriptItems;
		QUnicodeTools::initScripts(_engine.layoutData->string, &scriptItems);

		for (int i = 0; i < scriptItems.length(); ++i) {
			const auto& item = scriptItems.at(i);
			int end = i < scriptItems.length() - 1 ? scriptItems.at(i + 1).position : length;

			for (int j = item.position; j < end; ++j)
				_analysis[j].script = item.script;
		}

		const auto end = _offset + length;

		for (auto block = _bStart; blockPosition(block) < end; ++block) {
			const auto type = (*block)->type();

			const auto from = std::max(_offset, int(blockPosition(block)));
			const auto till = std::min(int(end), int(blockEnd(block)));

			if (till > from) {
				if (type == TextBlockType::Emoji
					|| type == TextBlockType::CustomEmoji
					|| type == TextBlockType::Skip) 
				{
					for (auto i = from - _offset, count = till - _offset; i != count; ++i) {
						_analysis[i].script = QChar::Script_Common;
						_analysis[i].flags = (chars[i] == QChar::Space)
							? QScriptAnalysis::None
							: QScriptAnalysis::Object;
					}
				}
				else {
					for (auto i = from - _offset, count = till - _offset; i != count; ++i) {
						if (chars[i] == QChar::LineFeed)
							_analysis[i].flags = QScriptAnalysis::LineOrParagraphSeparator;
						else
							_analysis[i].flags = QScriptAnalysis::None;
					}
				}
			}
		}

		{
			auto& m_string = _engine.layoutData->string;
			auto m_analysis = _analysis;
			auto& m_items = _engine.layoutData->items;

			auto start = 0;
			auto startBlock = _bStart;

			auto currentBlock = startBlock;
			auto nextBlock = currentBlock + 1;

			for (int i = 1; i != length; ++i) {
				while (blockPosition(nextBlock) <= _offset + i)
					currentBlock = nextBlock++;

				if (currentBlock != startBlock
					|| m_analysis[i].flags != m_analysis[start].flags) {
				}
				else if ((*startBlock)->type() != TextBlockType::Text
					&& m_analysis[i].flags == m_analysis[start].flags) {
					assert(i - start < kMaxItemLength);
					continue;
				}
				else if (m_analysis[i].bidiLevel == m_analysis[start].bidiLevel
					&& m_analysis[i].flags == m_analysis[start].flags
					&& (m_analysis[i].script == m_analysis[start].script || m_string[i] == u'.')
					&& i - start < kMaxItemLength) {
					continue;
				}
				m_items.append(QScriptItem(start, m_analysis[start]));
				start = i;
				startBlock = currentBlock;
			}
			m_items.append(QScriptItem(start, m_analysis[start]));
		}
	}
}

void StackEngine::updateFont(not_null<const AbstractBlock*> block) {
	const auto flags = block->flags();
	const auto newFont = WithFlags(_t->_st->_font, flags);

	if (_font != newFont) {
		_font = (newFont->family() == _t->_st->_font->family())
			? WithFlags(_t->_st->_font, flags, newFont->flags())
			: newFont;

		_engine.fnt = _font->f;
		_engine.resetFontEngineCache();
	}
}

std::vector<Block>::const_iterator StackEngine::shapeGetBlock(int item) {
	auto& si = _engine.layoutData->items[item];

	const auto blockIt = adjustBlock(_offset + si.position);
	const auto block = blockIt->get();

	updateFont(block);

	_engine.shape(item);
	if (si.analysis.flags == QScriptAnalysis::Object)
		si.width = block->objectWidth();

	return blockIt;
}

int StackEngine::blockIndex(int position) const {
	return int(adjustBlock(_offset + position) - begin(_tBlocks));
}

__BASE_QT_TEXT_NAMESPACE_END

