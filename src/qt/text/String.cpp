#include <base/qt/text/String.h>

#include <src/qt/text/TextRenderer.h>
#include <src/qt/text/BlockParser.h>

#include <src/qt/text/TextWord.h>
#include <base/qt/text/TextClickHandlers.h>

#include <src/qt/text/TextUtility.h>
#include <src/qt/text/WordParser.h>

#include <src/qt/text/TextDrawUtility.h>
#include <src/qt/text/Types.h>

#include <base/qt/common/BasicClickHandlers.h>
#include <base/Utility.h>


namespace base::qt::text {
	String::String(int32 minResizeWidth) :
		_minResizeWidth(minResizeWidth)
	{}

	String::String(
		const QString& string,
		const style::TextStyle* style)
	{
		setText(style, string);
	}

	int String::countWidth(int width, bool breakEverywhere) const {
		if (width >= _maxWidth) {
			return _maxWidth;
		}

		auto maxLineWidth = 0;
		enumerateLines(width, breakEverywhere, [&](int lineWidth, int) {
			if (lineWidth > maxLineWidth) {
				maxLineWidth = lineWidth;
			}
			});
		return maxLineWidth;
	}

	int String::countHeight(int width, bool breakEverywhere) const {
		if (width >= _maxWidth) {
			return _minHeight;
		}
		int result = 0;
		enumerateLines(width, breakEverywhere, [&](auto, int lineBottom) {
			result = lineBottom;
			});
		return result;
	}

	String::DimensionsResult String::countDimensions(
		GeometryDescriptor geometry) const {
		return countDimensions(std::move(geometry), {});
	}

	String::DimensionsResult String::countDimensions(
		GeometryDescriptor geometry,
		DimensionsRequest request) const {
		auto result = DimensionsResult();
		if (request.lineWidths && request.reserve)
			result.lineWidths.reserve(request.reserve);

		enumerateLines(geometry, [&](int lineWidth, int lineBottom) {
			const auto width = lineWidth;

			if (request.lineWidths)
				result.lineWidths.push_back(width);
			
			result.width = std::max(result.width, width);
			result.height = lineBottom;
			});

		return result;
	}

	bool String::isFullSelection(TextSelection selection) const {
		return (selection.from == 0) && (selection.to >= _text.size());
	}

	void String::setText(
		const style::TextStyle* style,
		const QString& text,
		const TextParseOptions& options) 
	{
		#pragma warning(push)
		#pragma warning(disable: 6262)

		_st = style;
		clear();

		BlockParser block(this, { text }, options, {});
		WordParser word(this);

		recountNaturalSize(true, options.dir);

		#pragma warning(pop)
	}

	bool String::hasLinks() const {
		return _extended && !_extended->links.empty();
	}

	void String::setLink(uint16 index, const ClickHandlerPtr& link) {
		const auto extended = _extended.get();
		// () << "extended == nullptr: " << (extended == nullptr);

		if (extended && index > 0 && index <= extended->links.size())
			extended->links[index - 1] = link;
	}

	bool String::hasCollapsedBlockquots() const {
		return _extended
			&& _extended->quotes
			&& std::ranges::any_of(_extended->quotes->list, &QuoteDetails::collapsed);
	}

	bool String::blockquoteCollapsed(int index) const {
		Expects(_extended && _extended->quotes);
		Expects(index > 0 && index <= _extended->quotes->list.size());

		return _extended->quotes->list[index - 1].collapsed;
	}

	bool String::blockquoteExpanded(int index) const {
		Expects(_extended && _extended->quotes);
		Expects(index > 0 && index <= _extended->quotes->list.size());

		return _extended->quotes->list[index - 1].expanded;
	}

	void String::setBlockquoteExpanded(int index, bool expanded) {
		Expects(_extended && _extended->quotes);
		Expects(index > 0 && index <= _extended->quotes->list.size());

		auto& quote = _extended->quotes->list[index - 1];
		if (quote.expanded == expanded) {
			return;
		}
		quote.expanded = expanded;
		recountNaturalSize(false);
		if (const auto onstack = _extended->quotes->expandCallback) {
			onstack(index, expanded);
		}
	}

	void String::setBlockquoteExpandCallback(
		Fn<void(int index, bool expanded)> callback) {
		Expects(_extended && _extended->quotes);

		_extended->quotes->expandCallback = std::move(callback);
	}

	bool String::hasSkipBlock() const {
		return !_blocks.empty()
			&& (_blocks.back()->type() == TextBlockType::Skip);
	}

	bool String::updateSkipBlock(int width, int height) {
		if (!width || !height) {
			return removeSkipBlock();
		}
		if (!_blocks.empty() && _blocks.back()->type() == TextBlockType::Skip) {
			const auto& block = _blocks.back().unsafe<SkipBlock>();
			if (block.width() == width && block.height() == height) {
				return false;
			}
			const auto size = block.position();
			_text.resize(size);
			_blocks.pop_back();
			_words.pop_back();
			removeModificationsAfter(size);
		}
		else if (_endsWithQuoteOrOtherDirection) {
			insertModifications(_text.size(), 1);
			_words.push_back(Word(
				uint16(_text.size()),
				int(_blocks.size())));
			_blocks.push_back(Block::Newline({
				.position = _words.back().position(),
				}, 0));
			_text.push_back(QChar::LineFeed);
			_skipBlockAddedNewline = true;
		}
		insertModifications(_text.size(), 1);
		const auto unfinished = false;
		const auto rbearing = 0;
		_words.push_back(Word(
			uint16(_text.size()),
			unfinished,
			width,
			rbearing));
		_blocks.push_back(Block::Skip({
			.position = _words.back().position(),
			}, width, height));
		_text.push_back('_');
		recountNaturalSize(false);
		return true;
	}

	bool String::removeSkipBlock() {
		if (_blocks.empty() || _blocks.back()->type() != TextBlockType::Skip) {
			return false;
		}
		else if (_skipBlockAddedNewline) {
			const auto size = _blocks.back()->position() - 1;
			_text.resize(size);
			_blocks.pop_back();
			_blocks.pop_back();
			_words.pop_back();
			_words.pop_back();
			_skipBlockAddedNewline = false;
			removeModificationsAfter(size);
		}
		else {
			const auto size = _blocks.back()->position();
			_text.resize(size);
			_blocks.pop_back();
			_words.pop_back();
			removeModificationsAfter(size);
		}
		recountNaturalSize(false);
		return true;
	}

	int String::countMaxMonospaceWidth() const {
		auto result = 0;
		if (const auto quotes = _extended ? _extended->quotes.get() : nullptr) {
			for (const auto& quote : quotes->list) {
				if (quote.pre) {
					core::utility::accumulateMax(result, quote.maxWidth);
				}
			}
		}
		return result;
	}

	void String::draw(
		QPainter& painter,
		const PaintContext& context) const 
	{
		#pragma warning(push)
		#pragma warning(disable: 6262)

		Renderer(*this).draw(painter, context);
		#pragma warning(pop)
	}


	bool String::isEmpty() const {
		return _blocks.empty() || _blocks[0]->type() == TextBlockType::Skip;
	}

	QString String::toQString() const {
		return _text;
	}

	QString String::toString(TextSelection selection) const {
		return toText(selection, false, false).rich.text;
	}

	TextWithEntities String::toTextWithEntities(TextSelection selection) const {
		return toText(selection, false, true).rich;
	}

	TextForMimeData String::toTextForMimeData(TextSelection selection) const {
		return toText(selection, true, true);
	}

	TextForMimeData String::toText(
		TextSelection selection,
		bool composeExpanded,
		bool composeEntities) const
	{
		struct MarkdownTagTracker {
			TextBlockFlags flag = TextBlockFlags();
			EntityType type = EntityType();
			int start = 0;
		};
		auto result = TextForMimeData();
		result.rich.text.reserve(_text.size());
		if (composeExpanded) {
			result.expanded.reserve(_text.size());
		}
		const auto insertEntity = [&](EntityInText&& entity) {
			auto i = result.rich.entities.end();
			while (i != result.rich.entities.begin()) {
				auto j = i;
				if ((--j)->offset() <= entity.offset()) {
					break;
				}
				i = j;
			}
			result.rich.entities.insert(i, std::move(entity));
			};
		using Flag = TextBlockFlag;
		using Flags = TextBlockFlags;
		auto linkStart = 0;
		auto markdownTrackers = composeEntities
			? std::vector<MarkdownTagTracker>{
				{ Flag::Italic, EntityType::Italic },
				{ Flag::Bold, EntityType::Bold },
				{ Flag::Semibold, EntityType::Semibold },
				{ Flag::Underline, EntityType::Underline },
				{ Flag::StrikeOut, EntityType::StrikeOut },
				{ Flag::Code, EntityType::Code },
				{ Flag::Pre, EntityType::Pre },
				{ Flag::Blockquote, EntityType::Blockquote },
		} : std::vector<MarkdownTagTracker>();
		const auto flagsChangeCallback = [&](
			Flags oldFlags,
			int oldQuoteIndex,
			Flags newFlags,
			int newQuoteIndex) {
				if (!composeEntities) {
					return;
				}
				for (auto& tracker : markdownTrackers) {
					const auto flag = tracker.flag;
					const auto quoteWithCollapseChanged = (flag == Flag::Blockquote)
						&& (oldFlags & flag)
						&& (newFlags & flag)
						&& (oldQuoteIndex != newQuoteIndex);
					const auto quoteWithLanguageChanged = (flag == Flag::Pre)
						&& (oldFlags & flag)
						&& (newFlags & flag)
						&& (oldQuoteIndex != newQuoteIndex);
					const auto quote = !oldQuoteIndex
						? nullptr
						: &_extended->quotes->list[oldQuoteIndex - 1];
					const auto data = !quote
						? QString()
						: quote->pre
						? quote->language
						: quote->blockquote
						? (quote->collapsed ? u"1"_q : QString())
						: QString();
					if (((oldFlags & flag) && !(newFlags & flag))
						|| quoteWithLanguageChanged
						|| quoteWithCollapseChanged) {
						insertEntity({
							tracker.type,
							tracker.start,
							int(result.rich.text.size()) - tracker.start,
							data,
							});
					}
					if (((newFlags & flag) && !(oldFlags & flag))
						|| quoteWithLanguageChanged
						|| quoteWithCollapseChanged) {
						tracker.start = result.rich.text.size();
					}
				}
			};
		const auto clickHandlerStartCallback = [&](EntityType type) {
			linkStart = result.rich.text.size();
			};
		const auto clickHandlerFinishCallback = [&](
			QStringView inText,
			const ClickHandlerPtr& handler,
			EntityType type) {
				if (!handler || (!composeExpanded && !composeEntities))
					return;
				
				// This logic is duplicated in TextForMimeData::WithExpandedLinks.
				const auto entity = handler->getTextEntity();
				const auto plainUrl = (entity.type == EntityType::Url)
					|| (entity.type == EntityType::Email)
					|| (entity.type == EntityType::Phone);

				const auto full = plainUrl
					? QStringView(entity.data).mid(0, entity.data.size())
					: inText;

				const auto customTextLink = (entity.type == EntityType::CustomUrl);
				const auto internalLink = customTextLink
					&& entity.data.startsWith(QLatin1String("internal:"));
				if (composeExpanded) {
					const auto sameAsTextLink = customTextLink
						&& (entity.data
							== UrlClickHandler::EncodeForOpening(full.toString()));
					if (customTextLink && !internalLink && !sameAsTextLink) {
						const auto& url = entity.data;
						result.expanded.append(QLatin1String(" (")).append(url).append(')');
					}

					if (composeEntities && !internalLink) {
						insertEntity({
							entity.type,
							linkStart,
							int(result.rich.text.size() - linkStart),
							plainUrl ? QString() : entity.data });
					}
				}
				if (composeEntities && !internalLink) {
					insertEntity({
						entity.type,
						linkStart,
						int(result.rich.text.size() - linkStart),
						plainUrl ? QString() : entity.data });
				}
			};
		const auto appendPartCallback = [&](
			QStringView part,
			const QString& customEmojiData) {
				result.rich.text += part;
				if (composeExpanded) {
					result.expanded += part;
				}
			};

		enumerateText(
			selection,
			appendPartCallback,
			clickHandlerStartCallback,
			clickHandlerFinishCallback,
			flagsChangeCallback);

		if (composeEntities) {
			const auto proj = [](const EntityInText& entity) {
				const auto type = entity.type();
				const auto isUrl = (type == EntityType::Url)
					|| (type == EntityType::CustomUrl)
					|| (type == EntityType::BotCommand)
					|| (type == EntityType::Mention)
					|| (type == EntityType::MentionName)
					|| (type == EntityType::Hashtag)
					|| (type == EntityType::Cashtag);
				return std::pair{ entity.offset(), isUrl ? 0 : 1 };
				};
			const auto pred = [&](const EntityInText& a, const EntityInText& b) {
				return proj(a) < proj(b);
				};
			std::sort(
				result.rich.entities.begin(),
				result.rich.entities.end(),
				pred);
		}

		return result;
	}


	void String::insertModifications(int position, int delta) {
		auto& modifications = ensureExtended()->modifications;
		auto i = end(modifications);
		while (i != begin(modifications) && (i - 1)->position >= position) {
			--i;
			if (i->position < position) {
				break;
			}
			else if (delta > 0) {
				++i->position;
			}
			else if (i->position == position) {
				break;
			}
		}
		if (i != end(modifications) && i->position == position) {
			++i->skipped;
		}
		else {
			modifications.insert(i, {
				.position = position,
				.skipped = uint16(delta < 0 ? (-delta) : 0),
				.added = (delta > 0),
				});
		}
	}

	void String::removeModificationsAfter(int size) {
		if (!_extended) {
			return;
		}
		auto& modifications = _extended->modifications;
		for (auto i = end(modifications); i != begin(modifications);) {
			--i;
			if (i->position > size) {
				i = modifications.erase(i);
			}
			else if (i->position == size) {
				i->added = false;
				if (!i->skipped) {
					i = modifications.erase(i);
				}
			}
			else {
				break;
			}
		}
	}

	std::vector<int> String::countLineWidths(int width) const {
		return countLineWidths(width, {});
	}

	std::vector<int> String::countLineWidths(
		int width,
		LineWidthsOptions options) const {
		auto result = std::vector<int>();
		if (options.reserve) {
			result.reserve(options.reserve);
		}
		enumerateLines(width, options.breakEverywhere, [&](QFixed lineWidth, int) {
			result.push_back(lineWidth.ceil().toInt());
			});
		return result;
	}

	void String::recountNaturalSize(
		bool initial,
		Qt::LayoutDirection optionsDirection) 
	{
		auto lastNewlineBlock = begin(_blocks);
		auto lastNewlineStart = 0;

		const auto computeParagraphDirection = [&](int paragraphEnd) {
			const auto direction = (optionsDirection != Qt::LayoutDirectionAuto)
				? optionsDirection
				: text::Direction(_text, lastNewlineStart, paragraphEnd);

			if (paragraphEnd) {
				while (blockPosition(lastNewlineBlock) < lastNewlineStart) {
					++lastNewlineBlock;
				}
				assert(lastNewlineBlock != end(_blocks));
				const auto block = lastNewlineBlock->get();
				if (block->type() == TextBlockType::Newline) {
					assert(block->position() == lastNewlineStart);
					static_cast<NewlineBlock*>(block)->setParagraphDirection(
						direction);
				}
				else {
					assert(!lastNewlineStart);
					_startParagraphLTR = (direction == Qt::LeftToRight);
					_startParagraphRTL = (direction == Qt::RightToLeft);
				}
			}
		};

		auto qindex = quoteIndex(nullptr);
		auto quote = quoteByIndex(qindex);

		auto qpadding = quotePadding(quote);
		auto qminwidth = quoteMinWidth(quote);

		auto qlinesleft = quoteLinesLimit(quote);
		auto qmaxwidth = QFixed(qminwidth);
		auto qoldheight = 0;

		_maxWidth = 0;
		_minHeight = qpadding.top();

		const auto lineHeight = this->lineHeight();
		auto maxWidth = QFixed();

		auto width = QFixed(qminwidth);
		auto last_rBearing = QFixed();
		auto last_rPadding = QFixed();

		for (const auto& word : _words) {
			if (word.newline()) {
				const auto block = word.newlineBlockIndex();
				const auto index = quoteIndex(_blocks[block].get());

				const auto changed = (qindex != index);
				const auto hidden = !qlinesleft;

				core::utility::accumulateMax(maxWidth, width);
				core::utility::accumulateMax(qmaxwidth, width);

				if (changed) {
					_minHeight += qpadding.bottom();

					if (quote) {
						quote->maxWidth = qmaxwidth.ceil().toInt();
						quote->minHeight = _minHeight - qoldheight;
					}

					qoldheight = _minHeight;
					qindex = index;

					quote = quoteByIndex(qindex);
					qpadding = quotePadding(quote);

					qminwidth = quoteMinWidth(quote);
					qlinesleft = quoteLinesLimit(quote);

					qmaxwidth = qminwidth;
					_minHeight += qpadding.top();
					qpadding.setTop(0);
				}
				else if (qlinesleft > 0)
					--qlinesleft;
			
				if (initial)
					computeParagraphDirection(word.position());
				
				lastNewlineStart = word.position();

				if (!hidden)
					_minHeight += lineHeight;

				last_rBearing = 0;
				last_rPadding = word.f_rpadding();

				width = qminwidth;
				continue;
			}

			auto w__f_rbearing = word.f_rbearing();

			core::utility::accumulateMax(maxWidth, width);
			core::utility::accumulateMax(qmaxwidth, width);

			width += last_rBearing + (last_rPadding + word.f_width() - w__f_rbearing);

			last_rBearing = w__f_rbearing;
			last_rPadding = word.f_rpadding();
		}
		if (initial) {
			computeParagraphDirection(_text.size());
		}
		if (width > 0) {
			const auto useSkipHeight = (_blocks.back()->type() == TextBlockType::Skip)
				&& (_words.back().f_width() == width);
			_minHeight += qpadding.top() + qpadding.bottom();

			if (qlinesleft != 0)
				_minHeight += useSkipHeight
					? _blocks.back().unsafe<SkipBlock>().height()
					: lineHeight;
			
			core::utility::accumulateMax(maxWidth, width);
			core::utility::accumulateMax(qmaxwidth, width);
		}
		_maxWidth = maxWidth.ceil().toInt();
		if (quote) {
			quote->maxWidth = qmaxwidth.ceil().toInt();
			quote->minHeight = _minHeight - qoldheight;
			_endsWithQuoteOrOtherDirection = true;
		}
		else {
			const auto lastIsNewline = (lastNewlineBlock != end(_blocks))
				&& (lastNewlineBlock->get()->type() == TextBlockType::Newline);
			const auto lastNewline = lastIsNewline
				? static_cast<NewlineBlock*>(lastNewlineBlock->get())
				: nullptr;

			const auto lastLineDirection = lastNewline
				? lastNewline->paragraphDirection()
				: _startParagraphRTL
				? Qt::RightToLeft
				: Qt::LeftToRight;
			_endsWithQuoteOrOtherDirection
				= (lastLineDirection != Qt::LeftToRight);
		}
	}

	int String::lineHeight() const {
		return _st->_font->height;
	}

	const std::vector<Modification>& String::modifications() const {
		static const auto kEmpty = std::vector<Modification>();
		return _extended ? _extended->modifications : kEmpty;
	}


	TextState String::getState(QPoint point, int width, StateRequest request) const {
		if (isEmpty())
			return {};

		#pragma warning(push)
		#pragma warning(disable: 6262)

		return Renderer(*this).getState(
			point,
			SimpleGeometry(width, 0, 0, false),
			request);

		#pragma warning(pop)
	}

	TextState String::getStateLeft(QPoint point, int width, int outerw, StateRequest request) const {
		return getState(point, width, request);
	}

	TextState String::getStateElided(QPoint point, int width, StateRequestElided request) const {
		if (isEmpty())
			return {};

		#pragma warning(push)

		#pragma warning(disable: 6262)
		#pragma warning(disable: 26437)

		return Renderer(*this).getState(point, SimpleGeometry(
			width,
			request.lines,
			request.removeFromEnd,
			request.flags & StateRequest::StateFlag::BreakEverywhere
		), static_cast<StateRequest>(request));

		#pragma warning(pop)
	}

	TextState String::getStateElidedLeft(QPoint point, int width, int outerw, StateRequestElided request) const {
		return getStateElided(point, width, request);
	}

	TextSelection String::adjustSelection(
		TextSelection selection,
		TextSelection::Type selectType)
	{
		auto from = selection.from;
		auto to = selection.to;

		if (from < _text.size() && from <= to) {
			if (to > _text.size())
				to = _text.size();
			if (selectType == TextSelection::Type::Paragraphs) {
				for (const auto& block : _blocks) {
					if (block->position() < from) {
						continue;
					}

					const auto& entities = toTextWithEntities().entities;
					const auto eIt = std::ranges::find_if(entities, [&](
						const EntityInText& e) {
							return (e.type() == EntityType::Pre
								|| e.type() == EntityType::Code)
								&& (from >= e.offset())
								&& ((e.offset() + e.length()) >= to);
						});
					if (eIt != entities.end()) {
						from = eIt->offset();
						to = eIt->offset() + eIt->length();
						while (to > 0 && text::IsSpace(_text.at(to - 1))) {
							--to;
						}
						if (to >= from) {
							return { from, to };
						}
					}
					break;
				}

				if (!text::IsParagraphSeparator(_text.at(from))) {
					while (from > 0 && !text::IsParagraphSeparator(_text.at(from - 1))) {
						--from;
					}
				}
				if (to < _text.size()) {
					if (text::IsParagraphSeparator(_text.at(to))) {
						++to;
					}
					else {
						while (to < _text.size() && !text::IsParagraphSeparator(_text.at(to))) {
							++to;
						}
					}
				}
			}
			else if (selectType == TextSelection::Type::Words) {
				if (!text::IsWordSeparator(_text.at(from))) {
					while (from > 0 && !text::IsWordSeparator(_text.at(from - 1))) {
						--from;
					}
				}
				if (to < _text.size()) {
					if (text::IsWordSeparator(_text.at(to))) {
						++to;
					}
					else {
						while (to < _text.size() && !text::IsWordSeparator(_text.at(to))) {
							++to;
						}
					}
				}
			}
		}
		return { from, to };
	}

	void String::clear() {
		_text.clear();
		_blocks.clear();
		_extended = nullptr;
		_maxWidth = _minHeight = 0;
		_startQuoteIndex = 0;
		_startParagraphLTR = false;
		_startParagraphRTL = false;
	}

	String::ExtendedWrap::ExtendedWrap() noexcept = default;

	String::ExtendedWrap::ExtendedWrap(ExtendedWrap&& other) noexcept
		: unique_ptr(std::move(other))
	{
		adjustFrom(&other);
	}

	String::ExtendedWrap& String::ExtendedWrap::operator=(
		ExtendedWrap&& other) noexcept
	{
		*static_cast<unique_ptr*>(this) = std::move(other);
		adjustFrom(&other);
		return *this;
	}

	String::ExtendedWrap::ExtendedWrap(
		std::unique_ptr<ExtendedData>&& other) noexcept
		: unique_ptr(std::move(other)) {
		assert(!get());
	}

	String::ExtendedWrap& String::ExtendedWrap::operator=(
		std::unique_ptr<ExtendedData>&& other) noexcept 
	{
		*static_cast<unique_ptr*>(this) = std::move(other);
		assert(!get());
		return *this;
	}

	String::ExtendedWrap::~ExtendedWrap() = default;

	void String::ExtendedWrap::adjustFrom(const ExtendedWrap* other) {
		const auto data = get();
		if (!data) {
			return;
		}
		const auto raw = [](auto pointer) {
			return reinterpret_cast<quintptr>(pointer);
			};
		const auto adjust = [&](auto& link) {
			const auto otherText = raw(link->text().get());
			link->setText(
				reinterpret_cast<String*>(otherText + raw(this) - raw(other)));
			};
		if (const auto quotes = data->quotes.get()) {
			for (auto& quote : quotes->list) {
				if (quote.copy) {
					adjust(quote.copy);
				}
				if (quote.toggle) {
					adjust(quote.toggle);
				}
			}
		}
	}

	not_null<ExtendedData*> String::ensureExtended() {
		if (!_extended) {
			_extended = std::make_unique<ExtendedData>();
		}
		return _extended.get();
	}

	not_null<QuotesData*> String::ensureQuotes() {
		const auto extended = ensureExtended();
		if (!extended->quotes) {
			extended->quotes = std::make_unique<QuotesData>();
		}
		return extended->quotes.get();
	}

	uint16 String::blockPosition(
		std::vector<Block>::const_iterator i,
		int fullLengthOverride) const {
		return (i != end(_blocks))
			? CountPosition(i)
			: (fullLengthOverride >= 0)
			? uint16(fullLengthOverride)
			: uint16(_text.size());
	}

	uint16 String::blockEnd(
		std::vector<Block>::const_iterator i,
		int fullLengthOverride) const {
		return (i != end(_blocks) && i + 1 != end(_blocks))
			? CountPosition(i + 1)
			: (fullLengthOverride >= 0)
			? uint16(fullLengthOverride)
			: uint16(_text.size());
	}

	uint16 String::blockLength(
		std::vector<Block>::const_iterator i,
		int fullLengthOverride) const {
		return (i == end(_blocks))
			? 0
			: (i + 1 != end(_blocks))
			? (CountPosition(i + 1) - CountPosition(i))
			: (fullLengthOverride >= 0)
			? (fullLengthOverride - CountPosition(i))
			: (int(_text.size()) - CountPosition(i));
	}

	QuoteDetails* String::quoteByIndex(int index) const {
		Expects(!index
			|| (_extended
				&& _extended->quotes
				&& index <= _extended->quotes->list.size()));

		return index ? &_extended->quotes->list[index - 1] : nullptr;
	}

	int String::quoteIndex(const AbstractBlock* block) const {
		Expects(!block || block->type() == TextBlockType::Newline);

		return block
			? static_cast<const NewlineBlock*>(block)->quoteIndex()
			: _startQuoteIndex;
	}

	const style::QuoteStyle& String::quoteStyle(
		not_null<QuoteDetails*> quote) const {
		return _st->blockquote;
	}

	QMargins String::quotePadding(QuoteDetails* quote) const {
		if (!quote) {
			return {};
		}
		const auto& st = quoteStyle(quote);
		const auto skip = st.verticalSkip;
		return QMargins(0, 0, 0, skip);
	}

	int String::quoteMinWidth(QuoteDetails* quote) const {
		if (!quote) {
			return 0;
		}
		const auto qpadding = quotePadding(quote);
		const auto& qheader = quoteHeaderText(quote);
		const auto& qst = quoteStyle(quote);
		const auto radius = qst.radius;
		const auto corner = std::max({ radius });
		const auto top = qpadding.left()
			+ (qheader.isEmpty()
				? 0
				: (_st->_font->monospace()->width(qheader))
			+ std::max(
				qpadding.right(),
				0));
		return std::max(top, 2 * corner);
	}

	const QString& String::quoteHeaderText(QuoteDetails* quote) const {
		static const auto kEmptyHeader = QString();
		static const auto kDefaultHeader = QString("Copy");
		return (!quote || !quote->pre)
			? kEmptyHeader
			: quote->language.isEmpty()
			? kDefaultHeader
			: quote->language;
	}

	int String::quoteLinesLimit(QuoteDetails* quote) const {
		return (quote && quote->collapsed && !quote->expanded)
			? kQuoteCollapsedLines
			: -1;
	}

	template <
		typename AppendPartCallback,
		typename ClickHandlerStartCallback,
		typename ClickHandlerFinishCallback,
		typename FlagsChangeCallback>
	void String::enumerateText(
		TextSelection selection,
		AppendPartCallback appendPartCallback,
		ClickHandlerStartCallback clickHandlerStartCallback,
		ClickHandlerFinishCallback clickHandlerFinishCallback,
		FlagsChangeCallback flagsChangeCallback) const
	{
		if (isEmpty() || selection.empty()) {
			return;
		}

		int linkIndex = 0;
		uint16 linkPosition = 0;
		int quoteIndex = _startQuoteIndex;

		TextBlockFlags flags = {};
		for (auto i = _blocks.cbegin(), e = _blocks.cend(); true; ++i) {
			const auto blockPosition = (i == e)
				? uint16(_text.size())
				: (*i)->position();
			const auto blockFlags = (i == e) ? TextBlockFlags() : (*i)->flags();
			const auto blockQuoteIndex = (i == e)
				? 0
				: ((*i)->type() != TextBlockType::Newline)
				? quoteIndex
				: static_cast<const NewlineBlock*>(i->get())->quoteIndex();
			const auto blockLinkIndex = [&] {
				if (IsMono(blockFlags) || (i == e)) {
					return 0;
				}
				const auto result = (*i)->linkIndex();
				return (result && _extended && _extended->links[result - 1])
					? result
					: 0;
				}();
			if (blockLinkIndex != linkIndex) {
				if (linkIndex) {
					auto rangeFrom = qMax(selection.from, linkPosition);
					auto rangeTo = qMin(selection.to, blockPosition);
					if (rangeTo > rangeFrom) { // handle click handler
						const auto r = StringViewMid(
							_text,
							rangeFrom,
							rangeTo - rangeFrom);
						// Ignore links that are partially copied.
						const auto handler = (linkPosition != rangeFrom
							|| blockPosition != rangeTo
							|| !_extended)
							? nullptr
							: _extended->links[linkIndex - 1];
						const auto type = handler
							? handler->getTextEntity().type
							: EntityType::Invalid;
						clickHandlerFinishCallback(r, handler, type);
					}
				}
				linkIndex = blockLinkIndex;
				if (linkIndex) {
					linkPosition = blockPosition;
					const auto handler = _extended
						? _extended->links[linkIndex - 1]
						: nullptr;
					clickHandlerStartCallback(handler
						? handler->getTextEntity().type
						: EntityType::Invalid);
				}
			}

			const auto checkBlockFlags = (blockPosition >= selection.from)
				&& (blockPosition <= selection.to);
			if (checkBlockFlags
				&& (blockFlags != flags
					|| ((flags & TextBlockFlag::Pre)
						&& blockQuoteIndex != quoteIndex))) {
				flagsChangeCallback(
					flags,
					quoteIndex,
					blockFlags,
					blockQuoteIndex);
				flags = blockFlags;
			}
			quoteIndex = blockQuoteIndex;
			if (i == e
				|| (linkIndex ? linkPosition : blockPosition) >= selection.to) {
				break;
			}

			const auto blockType = (*i)->type();
			if (blockType == TextBlockType::Skip) {
				continue;
			}
		}
	}

	template <typename Callback>
	void String::enumerateLines(
		int w,
		bool breakEverywhere,
		Callback&& callback) const 
	{
		if (isEmpty())
			return;
	
		const auto width = std::max(w, _minResizeWidth);
		auto g = SimpleGeometry(width, 0, 0, false);
		g.breakEverywhere = breakEverywhere;
		enumerateLines(g, std::forward<Callback>(callback));
	}

	template <typename Callback>
	void String::enumerateLines(
		GeometryDescriptor geometry,
		Callback&& callback) const 
	{
		if (isEmpty())
			return;

		const auto withElided = [&](bool elided) {
			if (geometry.outElided) {
				*geometry.outElided = elided;
			}
			};

		auto qindex = 0;
		auto quote = (QuoteDetails*)nullptr;
		auto qlinesleft = -1;
		auto qpadding = QMargins();

		auto top = 0;
		auto lineLeft = 0;
		auto lineWidth = 0;
		auto lineElided = false;
		auto widthLeft = QFixed(0);
		auto lineIndex = 0;
		const auto initNextLine = [&] {
			const auto line = geometry.layout(lineIndex++);
			lineLeft = line.left;
			lineWidth = line.width;
			lineElided = line.elided;
			if (quote && quote->maxWidth < lineWidth) {
				lineWidth = quote->maxWidth;
			}
			widthLeft = lineWidth - qpadding.left() - qpadding.right();
			};
		const auto initNextParagraph = [&](int16 paragraphIndex) {
			if (qindex != paragraphIndex) {
				qindex = paragraphIndex;
				quote = quoteByIndex(qindex);
				qpadding = quotePadding(quote);
				qlinesleft = quoteLinesLimit(quote);
				top += qpadding.top();
				qpadding.setTop(0);
			}
			initNextLine();
			};

		if ((*_blocks.cbegin())->type() != TextBlockType::Newline) {
			initNextParagraph(_startQuoteIndex);
		}

		const auto lineHeight = this->lineHeight();
		auto last_rBearing = QFixed();
		auto last_rPadding = QFixed();
		auto longWordLine = true;
		auto lastWordStart = begin(_words);
		auto lastWordStart_wLeft = widthLeft;
		for (auto w = lastWordStart, e = end(_words); w != e; ++w) {
			if (w->newline()) {
				const auto block = w->newlineBlockIndex();
				const auto index = quoteIndex(_blocks[block].get());
				const auto hidden = !qlinesleft;
				const auto changed = (qindex != index);
				if (changed)
					top += qpadding.bottom();

				if (qlinesleft > 0)
					--qlinesleft;
		
				if (!hidden)
					callback(lineLeft + lineWidth - widthLeft.toInt(), top += lineHeight);

				if (lineElided)
					return withElided(true);

				last_rBearing = 0;
				last_rPadding = w->f_rpadding();

				initNextParagraph(index);
				longWordLine = true;
				lastWordStart = w;
				lastWordStart_wLeft = widthLeft;
				continue;
			}
			else if (!qlinesleft)
				continue;
			const auto wordEndsHere = !w->unfinished();

			auto w__f_width = w->f_width();
			const auto w__f_rbearing = w->f_rbearing();
			const auto newWidthLeft = widthLeft
				- last_rBearing
				- (last_rPadding + w__f_width - w__f_rbearing);
			if (newWidthLeft >= 0) {
				last_rBearing = w__f_rbearing;
				last_rPadding = w->f_rpadding();
				widthLeft = newWidthLeft;

				if (wordEndsHere) {
					longWordLine = false;
				}
				if (wordEndsHere || longWordLine) {
					lastWordStart_wLeft = widthLeft;
					lastWordStart = w + 1;
				}
				continue;
			}

			if (lineElided) {
			}
			else if (w != lastWordStart && !geometry.breakEverywhere) {
				w = lastWordStart;
				widthLeft = lastWordStart_wLeft;
				w__f_width = w->f_width();
			}

			if (qlinesleft > 0) {
				--qlinesleft;
			}
			callback(lineLeft + lineWidth - widthLeft.toInt(), top += lineHeight);
			if (lineElided) {
				return withElided(true);
			}

			initNextLine();

			last_rBearing = w->f_rbearing();
			last_rPadding = w->f_rpadding();
			widthLeft -= w__f_width - last_rBearing;

			longWordLine = !wordEndsHere;
			lastWordStart = w + 1;
			lastWordStart_wLeft = widthLeft;
		}
		if (widthLeft < lineWidth) {
			const auto useSkipHeight = (_blocks.back()->type() == TextBlockType::Skip)
				&& (widthLeft + _words.back().f_width() == lineWidth);
			const auto useLineHeight = useSkipHeight
				? _blocks.back().unsafe<SkipBlock>().height()
				: lineHeight;
			callback(
				lineLeft + lineWidth - widthLeft.toInt(),
				top + useLineHeight + qpadding.bottom());
		}
		return withElided(false);
	}

} // namespace base::qt::text