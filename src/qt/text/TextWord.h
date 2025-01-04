#pragma once

#include <src/qt/text/Types.h>
#include <cmath>


namespace base::qt::text {
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

		[[nodiscard]] bool newline() const;
		[[nodiscard]] int newlineBlockIndex() const;
		[[nodiscard]] bool unfinished() const;

		[[nodiscard]] uint16 position() const;
		[[nodiscard]] QFixed f_rbearing() const;
		[[nodiscard]] QFixed f_width() const;

		[[nodiscard]] QFixed f_rpadding() const;
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

	[[nodiscard]] uint16 CountPosition(Words::const_iterator iterator);
} // namespace base::qt::text