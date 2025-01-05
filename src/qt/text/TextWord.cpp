#include <text/TextWord.h>


namespace base::qt::text {
	Word::Word(
		uint16 position,
		bool unfinished,
		QFixed width,
		QFixed rbearing
	): 
		_position(position)
		, _rbearing_modulus(std::min(std::abs(rbearing.value()), 0x7FFF))
		, _rbearing_positive(rbearing.value() > 0 ? 1 : 0)
		, _unfinished(unfinished ? 1 : 0)
		, _qfixedwidth(width.value()) 
	{}

	Word::Word(uint16 position, int newlineBlockIndex):
		_position(position)
		, _newline(1)
		, _newlineBlockIndex(newlineBlockIndex) 
	{}

	bool Word::newline() const {
		return _newline != 0;
	}

	int Word::newlineBlockIndex() const {
		return _newline ? _newlineBlockIndex : 0;
	}

	bool Word::unfinished() const {
		return _unfinished != 0;
	}

	uint16 Word::position() const {
		return _position;
	}

	QFixed Word::f_rbearing() const {
		return QFixed::fromFixed(
			int(_rbearing_modulus) * (_rbearing_positive ? 1 : -1));
	}

	QFixed Word::f_width() const {
		return _newline ? 0 : QFixed::fromFixed(_qfixedwidth);
	}

	QFixed Word::f_rpadding() const {
		return _rpadding;
	}

	void Word::add_rpadding(QFixed padding) {
		_rpadding += padding;
	}

	[[nodiscard]] inline uint16 CountPosition(Words::const_iterator i) {
		return i->position();
	}
} // namespace base::qt::text