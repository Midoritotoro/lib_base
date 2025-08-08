#pragma once

#include <src/qt/text/Types.h>
#include <cmath>


__BASE_QT_TEXT_NAMESPACE_BEGIN

using Words = std::vector<Word>;

class Word final {
public:
	Word() = default;
	Word( // !newline
		uint16 position,
		bool unfinished,
		QFixed width,
		QFixed rbearing);

	Word(uint16 position, int newlineBlockIndex);

	base_nodiscard bool newline() const;
	base_nodiscard int newlineBlockIndex() const;
	base_nodiscard bool unfinished() const;

	base_nodiscard uint16 position() const;
	base_nodiscard QFixed f_rbearing() const;
	base_nodiscard QFixed f_width() const;

	base_nodiscard QFixed f_rpadding() const;
	void add_rpadding(QFixed padding);
private:
	uint16 _position = 0;

	uint16 _rbearing_modulus : 13 = 0;
	uint16 _rbearing_positive : 1 = 0;

	uint16 _unfinished : 1 = 0;
	uint16 _newline : 1 = 0;

	QFixed _rpadding;

	union {
		int _qfixedwidth;
		int _newlineBlockIndex;
	};
};

base_nodiscard uint16 CountPosition(Words::const_iterator iterator);

__BASE_QT_TEXT_NAMESPACE_END