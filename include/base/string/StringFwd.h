#pragma once


namespace base::string {
	# define BEGIN_HAS_CHAR8_T_NAMESPACE namespace has_char8_t {
	# define BEGIN_NO_CHAR8_T_NAMESPACE inline namespace no_char8_t {

	#define END_HAS_CHAR8_T_NAMESPACE }
	#define END_NO_CHAR8_T_NAMESPACE }

	BEGIN_HAS_CHAR8_T_NAMESPACE
	END_HAS_CHAR8_T_NAMESPACE

	BEGIN_NO_CHAR8_T_NAMESPACE
	END_NO_CHAR8_T_NAMESPACE

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


	BEGIN_NO_CHAR8_T_NAMESPACE
		using Utf8StringView = BasicUtf8StringView<false>;
	END_NO_CHAR8_T_NAMESPACE

	BEGIN_HAS_CHAR8_T_NAMESPACE
		using Utf8StringView = BasicUtf8StringView<true>;
	END_HAS_CHAR8_T_NAMESPACE
}
