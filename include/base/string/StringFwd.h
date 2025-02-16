#pragma once


namespace base::string {
	class ByteArray;
	class ByteArrayView;

	class Latin1String;
	using Latin1StringView = Latin1String;

	class StringView;
	template <bool> class BasicUtf8StringView;

	class AnyStringView;
	class Char;

	class RegularExpression;
	class RegularExpressionMatch;

	using Utf8StringView = BasicUtf8StringView<false>;
	using Utf8StringView = BasicUtf8StringView<true>;
}
