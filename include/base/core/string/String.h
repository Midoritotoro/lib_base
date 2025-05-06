#pragma once 

#include <base/core/string/Char.h>

#include <base/core/string/StringIterator.h>
#include <base/core/utility/TypeTraits.h>

#include <vector>

__BASE_STRING_NAMESPACE_BEGIN

class StringView;

class String {
public:
    using iterator                  = StringIterator<String>;
    using const_iterator            = StringConstIterator<String>;

    using Iterator                  = iterator;
    using ConstIterator             = const_iterator;

    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

	using ReverseIterator			= reverse_iterator;
	using ConstReverseIterator		= const_reverse_iterator;
  
    using size_type                 = sizetype;
    using difference_type           = ptrdiff;

    using const_reference           = const Char&;
    using reference                 = Char&;

    using pointer                   = Char*;
    using const_pointer             = const Char*;

    using value_type                = Char;


	template <typename _Char>
	struct IsCompatibleCharTypeHelper
		: std::integral_constant<bool,
		std::is_same<_Char, _Char>::value ||
		std::is_same<_Char, ushort>::value ||
		std::is_same<_Char, char16_t>::value ||
		(std::is_same<_Char, wchar_t>::value && sizeof(wchar_t) == sizeof(_Char))> {};
	template <typename _Char>
	struct IsCompatibleCharType
		: IsCompatibleCharTypeHelper<remove_cvref_t<_Char>> {};

	template <typename Pointer>
	struct IsCompatiblePointerHelper : std::false_type {};
	template <typename _Char>
	struct IsCompatiblePointerHelper<_Char*>
		: IsCompatibleCharType<_Char> {};
	template <typename Pointer>
	struct IsCompatiblePointer
		: IsCompatiblePointerHelper<remove_cvref_t<Pointer>> {};

	template <typename T, typename Enable = void>
	struct IsContainerCompatibleWithStringView : std::false_type {};

	template <typename T>
	struct IsContainerCompatibleWithStringView<T, std::enable_if_t<std::conjunction_v<
		// lacking concepts and ranges, we accept any T whose std::data yields a suitable pointer ...
		IsCompatiblePointer<decltype(std::data(std::declval<const T&>()))>,
		// ... and that has a suitable size ...
		std::is_convertible<decltype(std::size(std::declval<const T&>())), sizetype>,
		// ... and it's a range as it defines an iterator-like API
		IsCompatibleCharType<typename std::iterator_traits<decltype(std::begin(std::declval<const T&>()))>::value_type>,
		std::is_convertible<
		decltype(std::begin(std::declval<const T&>()) != std::end(std::declval<const T&>())),
		bool>,

		// These need to be treated specially due to the empty vs null distinction
		std::negation<std::is_same<std::decay_t<T>, String>>,

		// Don't make an accidental copy constructor
		std::negation<std::is_same<std::decay_t<T>, StringView>>
		>>> : std::true_type{};

	template <typename _Char>
	using if_compatible_char = typename std::enable_if<IsCompatibleCharType<_Char>::value, bool>::type;

	template <typename Pointer>
	using if_compatible_pointer = typename std::enable_if<IsCompatiblePointer<Pointer>::value, bool>::type;

	template <typename T>
	using if_compatible_string_like = typename std::enable_if<std::is_same<T, String>::value, bool>::type;

	template <typename T>
	using if_compatible_container = typename std::enable_if<IsContainerCompatibleWithStringView<T>::value, bool>::type;


	enum _SplitBehavior : uchar {
		KeepEmptyParts = 0,
		SkipEmptyParts = 0x1,
	};

	DECLARE_FLAGS_ENUM(SplitBehavior, _SplitBehavior)

	CONSTEXPR_CXX20 String();
	CONSTEXPR_CXX20 ~String();

	CONSTEXPR_CXX20 String(const String& string);
	CONSTEXPR_CXX20 String(
		const Char* chs, 
		size_type length);

	CONSTEXPR_CXX20 String(
		Iterator first,
		Iterator last);

	template <typename _Char, if_compatible_char<_Char> = true>
	CONSTEXPR_CXX20 String(const _Char* str, size_type len)
	{
		for (size_type i = 0; i < len; ++i)
			_data.push_back(str[i]);
	}


	template <typename _Char, if_compatible_char<_Char> = true>
	CONSTEXPR_CXX20 String(
		const _Char* first,
		const _Char* last) :
		String(first, last - first)
	{}

	NODISCARD Char& operator[](const sizetype index) noexcept;
	NODISCARD const Char& operator[](const sizetype index) const noexcept;

	NODISCARD Char at(const sizetype index) const noexcept;

	NODISCARD NativeString toNativeString() const noexcept;

	NODISCARD std::wstring toStdWString() const noexcept;
	NODISCARD std::string toStdString() const noexcept;

	NODISCARD sizetype size() const noexcept;
	NODISCARD sizetype length() const noexcept;

	NODISCARD sizetype capacity() const noexcept;

	NODISCARD Char pop() noexcept;

	NODISCARD sizetype find(const String& string) const noexcept;
	NODISCARD sizetype find(Char ch) const noexcept;

	NODISCARD std::vector<sizetype> findAll(const String& string) const noexcept;
	NODISCARD std::vector<sizetype> findAll(Char ch) const noexcept;

	NODISCARD sizetype findLastOf(const String& string) const noexcept;
	NODISCARD sizetype findLastOf(Char ch) const noexcept;

	NODISCARD sizetype findFirstOf(const String& string) const noexcept;
	NODISCARD sizetype findFirstOf(Char ch) const noexcept;

	NODISCARD sizetype findLastNotOf(const String& string) const noexcept;
	NODISCARD sizetype findLastNotOf(Char ch) const noexcept;

	NODISCARD sizetype findFirstNotOf(const String& string) const noexcept;
	NODISCARD sizetype findFirstNotOf(Char ch) const noexcept;

	NODISCARD bool isEmpty() const noexcept;
	NODISCARD bool isNull() const noexcept;

	NODISCARD Char* data() const noexcept;
	NODISCARD const Char* constData() const noexcept;

	CONSTEXPR_CXX20 Iterator begin() noexcept;
	CONSTEXPR_CXX20 ConstIterator begin() const noexcept;

	CONSTEXPR_CXX20 ConstIterator cbegin() const noexcept;
	CONSTEXPR_CXX20 ConstIterator constBegin() const noexcept;

	CONSTEXPR_CXX20 Iterator end() noexcept;
	CONSTEXPR_CXX20 ConstIterator end() const noexcept;

	CONSTEXPR_CXX20 ConstIterator cend() const;
	CONSTEXPR_CXX20 ConstIterator constEnd() const noexcept;

	CONSTEXPR_CXX20 ReverseIterator rbegin() noexcept;
	CONSTEXPR_CXX20 ReverseIterator rend() noexcept;

	CONSTEXPR_CXX20 ConstReverseIterator rbegin() const noexcept;
	CONSTEXPR_CXX20 ConstReverseIterator rend() const noexcept;

	CONSTEXPR_CXX20 ConstReverseIterator crbegin() const noexcept;
	CONSTEXPR_CXX20 ConstReverseIterator crend() const noexcept;

	void push_back(Char c);
	void push_back(const String& s);

	void push_front(Char c);
	void push_front(const String& s);

	void clear();

	NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last);
	NODISCARD Iterator erase(ConstIterator it);

	NODISCARD Char front() const;
	NODISCARD Char& front();

	NODISCARD Char back() const;
	NODISCARD Char& back();

	void resize(sizetype size);
	void resize(
		sizetype size,
		Char fill);

	NODISCARD sizetype indexOf(
		Char ch,
		sizetype from = 0, 
		CaseSensitivity cs = CaseSensitive) const;
	NODISCARD sizetype indexOf(
		const String& string,
		sizetype from = 0, 
		CaseSensitivity cs = CaseSensitive) const;

	NODISCARD sizetype lastIndexOf(
		Char c,
		CaseSensitivity cs = CaseSensitive) const noexcept;
	NODISCARD sizetype lastIndexOf(
		Char c, 
		sizetype from, 
		CaseSensitivity cs = CaseSensitive) const;

	NODISCARD sizetype lastIndexOf(
		const String& string,
		CaseSensitivity cs = CaseSensitive) const;
	NODISCARD sizetype lastIndexOf(
		const String& string,
		sizetype from,
		CaseSensitivity cs = CaseSensitive) const;


	NODISCARD bool contains(
		Char ch, 
		CaseSensitivity caseSensitivity = CaseSensitive) const;
	NODISCARD bool contains(
		const String& string,
		CaseSensitivity caseSensitivity = CaseSensitive) const;

	NODISCARD sizetype count(
		Char ch, 
		CaseSensitivity caseSensitivity = CaseSensitive) const;
	NODISCARD sizetype count(
		const String& string,
		CaseSensitivity caseSensitivity = CaseSensitive) const;

	NODISCARD String& fill(
		Char ch, 
		sizetype size = -1);

	String& insert(
		sizetype index,
		Char ch);
	String& insert(
		sizetype index,
		const Char* ch,
		sizetype length);

	String& insert(
		sizetype index,
		const String& string);

	String& append(Char ch);
	String& append(
		const Char* ch,
		sizetype length);

	String& append(const String& string);

	String& prepend(Char ch);
	String& prepend(
		const Char* ch, 
		sizetype len);

	String& prepend(const String& string);

	String& remove(
		sizetype index,
		sizetype length);

	String& remove(
		Char ch, 
		CaseSensitivity caseSensitivity = CaseSensitive);
	String& remove(
		const String& string, 
		CaseSensitivity caseSensitivity = CaseSensitive);

	String& removeAt(sizetype pos);

	String& removeFirst();
	String& removeLast();

	String& replace(
		sizetype index, 
		sizetype length, 
		Char after);

	String& replace(
		sizetype index,
		sizetype length, 
		const Char* ch, 
		sizetype slen);
	String& replace(
		sizetype index,
		sizetype length, 
		const String& after);

	String& replace(
		Char before,
		Char after,
		CaseSensitivity caseSensitivity = CaseSensitive);
	String& replace(
		const Char* before,
		sizetype beforeLength, 
		const Char* after, 
		sizetype afterLength, 
		CaseSensitivity caseSensitivity = CaseSensitive);

	String& replace(
		const String& before,
		const String& after,
		CaseSensitivity caseSensitivity = CaseSensitive);
	String& replace(
		Char ch, 
		const String& after, 
		CaseSensitivity caseSensitivity = CaseSensitive);

	using StringList = std::vector<String>;

	NODISCARD StringList split(
		const String& sep,
		SplitBehavior behavior = KeepEmptyParts,
		CaseSensitivity cs = CaseSensitive) const;

	NODISCARD StringList split(
		Char sep, 
		SplitBehavior behavior = KeepEmptyParts,
		CaseSensitivity caseSensibity = CaseSensitive) const;

private:
	
	
#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	NODISCARD int ConvertWCharToUnicode(
		char* buffer,
		size_t bufferlen,
		const wchar_t* input);

	NODISCARD int ConvertUnicodeToWChar(
		wchar_t* buffer,
		size_t bufferlen,
		const char* input);
#endif

	std::vector<Char> _data;
};

NODISCARD CONSTEXPR_CXX20 auto Char::fromUcs4(char32_t c) noexcept
{
	struct R {
		char16_t chars[2];
		NODISCARD CONSTEXPR_CXX20 operator String() const noexcept { return { begin(), end() }; }
		NODISCARD CONSTEXPR_CXX20 sizetype size() const noexcept { return chars[1] ? 2 : 1; }
		NODISCARD CONSTEXPR_CXX20 const char16_t* begin() const noexcept { return chars; }
		NODISCARD CONSTEXPR_CXX20 const char16_t* end() const noexcept { return begin() + size(); }
	};
	return requiresSurrogates(c) ? R{ {Char::highSurrogate(c),
									  Char::lowSurrogate(c)} } :
		R{ {char16_t(c), u'\0'} };
}

__BASE_STRING_NAMESPACE_END

