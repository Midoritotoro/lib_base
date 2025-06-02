__BASE_QT_STYLE_NAMESPACE_BEGIN



// Style for widgets containing text. For example, text areas and etc. 
class TextWidgetStyleParameters :
	public TitleWidgetStyleParameters
{
	virtual NODISCARD bool linkUnderLine() const noexcept {
		return _linkUnderLine;
	}

	virtual NODISCARD QuoteStyle quoteStyle() const noexcept {
		return _blockQuote;
	}

	virtual NODISCARD QPoint headerPosition() const noexcept {
		return _blockQuote.headerPosition;
	}

	virtual NODISCARD int outlineShift() const noexcept {
		return _blockQuote.outlineShift;
	}

	virtual NODISCARD int quoteRadius() const noexcept {
		return _blockQuote.radius;
	}

	virtual void setLinkUnderLine(bool value) noexcept {
		_linkUnderLine = value;
	}

	virtual void setQuoteStyle(const QuoteStyle& value) noexcept {
		_blockQuote = value;
	}

	virtual void setOutlineShift(int value) noexcept {
		_blockQuote.outlineShift = value;
	}

	virtual void setQuoteRadius(int value) noexcept {
		_blockQuote.radius = value;
	}

	virtual const QImage& corners() const {
		return _blockQuote.corners;
	}

	virtual void setCorners(const QImage& value) {
		_blockQuote.corners = value;
	}

	virtual const QImage& expand() const {
		return _blockQuote.expand;
	}

	virtual void setExpand(const QImage& value) {
		_blockQuote.expand = value;
	}

	virtual const QImage& collapse() const {
		return _blockQuote.collapse;
	}

	virtual void setCollapse(const QImage& value) {
		_blockQuote.collapse = value;
	}

	virtual const QImage& bottomCorner() const {
		return _blockQuote.bottomCorner;
	}

	virtual void setBottomCorner(const QImage& value) const {
		_blockQuote.bottomCorner = value;
	}

	virtual const QImage& bottomRounding() const {
		return _blockQuote.bottomRounding;
	}

	virtual void setBottomRounding(const QImage& value) const {
		_blockQuote.bottomRounding = value;
	}

	virtual const QImage& collapsedLine() const {
		return _blockQuote.collapsedLine;
	}

	virtual void setCollapsedLine(const QImage& value) const {
		_blockQuote.collapsedLine = value;
	}

	virtual int outlinesCached() const {
		return _blockQuote.outlinesCached;
	}

	virtual void setOutlinesCached(int value) {
		_blockQuote.outlinesCached = value;
	}

	virtual const QColor& headerCached() const {
		return _blockQuote.headerCached;
	}

	virtual void setHeaderCached(const QColor& value) {
		_blockQuote.headerCached = value;
	}

	virtual const QColor& backgroundCached() const {
		return _blockQuote.bgCached;
	}

	virtual void setBackgroundCached(const QColor& value) {
		_blockQuote.bgCached = value;
	}

	virtual int outline() const {
		return _blockQuote.outline;
	}

	virtual void setOutline(int value) {
		_blockQuote.outline = value;
	}

	virtual int header() const {
		return _blockQuote.header;
	}

	virtual void setHeader(int value) {
		_blockQuote.header = value;
	}

	virtual int verticalSkip() const {
		return _blockQuote.verticalSkip;
	}

	virtual void setVerticalSkip(int value) {
		_blockQuote.verticalSkip = value;
	}
private:
	bool _linkUnderLine = true;
	QuoteStyle _blockQuote = {};
};
