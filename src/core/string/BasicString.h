#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/string/StringIterator.h>
#include <src/core/string/CharTypeCompatibility.h>

#include <src/core/string/StringDebug.h>
#include <base/core/container/Vector.h>


__BASE_STRING_NAMESPACE_BEGIN

template <
	class _Char_,
	class _Traits_		= CharTraits<_Char_>,
	class _Allocator_	= std::allocator<_Char_>>
class BasicString 
{
public:
	using allocator_type	= _Allocator;
	using value_type		= _Char_;

	using reference			= _Char_&;
	using const_reference	= const _Char_&;

	using pointer			= _Char_*;
	using const_pointer		= const _Char_*;

	using size_type			= sizetype;
	using difference_type	= ptrdiff;

	using iterator			= StringIterator<
		BasicString<_Char_, _Traits_, _Allocator_>>;
	using const_iterator	= StringConstIterator<
		BasicString<_Char_, _Traits_, _Allocator_>>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using AllocatoTtype		= allocator_type;
	using ValueType			= value_type;

	using Reference			= reference;
	using ConstReference	= const_reference;

	using Pointer			= pointer;
	using ConstPointer		= const_pointer;

	using SizeType			= size_type;
	using DifferenceType	= ptrdiff;

	using Iterator			= iterator;
	using ConstIterator		= const_iterator;

	using ReverseIterator		= reverse_iterator;
	using ConstReverseIterator	= const_reverse_iterator;

	using StringList = container::Vector<BasicString>;

	using traits = CharTraits<ValueType>;
	using Traits = traits;

	template <typename __Char>
	using if_compatible_char		= typename std::enable_if<
		IsCompatibleCharType<__Char>::value, bool>::type;

	template <typename _Pointer_>
	using if_compatible_pointer		= typename std::enable_if<
		IsCompatiblePointer<_Pointer_>::value, bool>::type;

	template <typename _Type_>
	using if_compatible_string_like	= typename std::enable_if<
		std::is_same<_Type_, BasicString>::value, bool>::type;

	template <typename _Type_>
	using if_compatible_container	= typename std::enable_if<
		IsContainerCompatibleWithStringView<_Type_>::value, bool>::type;

	template <typename __Char>
	inline static constexpr bool is_compatible_char_v			= IsCompatibleCharType<__Char>::value;

	template <typename _Pointer_>
	inline static constexpr bool is_compatible_pointer_v		= IsCompatiblePointer<_Pointer_>::value;

	template <typename _Type_>
	inline static constexpr bool is_compatible_string_like_v	= std::is_same<_Type_, BasicString>::value;

	template <typename _Type_>
	inline static constexpr bool is_compatible_container_v		= IsContainerCompatibleWithStringView<_Type_>::value;

	enum SplitBehaviorEnum : uchar {
		KeepEmptyParts = 0,
		SkipEmptyParts = 0x1
	};

	DECLARE_FLAGS_ENUM(SplitBehavior, SplitBehaviorEnum);

	CONSTEXPR_CXX20 BasicString();
	CONSTEXPR_CXX20 ~BasicString();

	CONSTEXPR_CXX20 BasicString(
		const ValueType* chs, 
		SizeType length);

	CONSTEXPR_CXX20 BasicString(
		Iterator first,
		Iterator last);

	template <typename __Char>
	CONSTEXPR_CXX20 inline BasicString(
		const __Char* str,
		size_type len);

	template <typename __Char>
	CONSTEXPR_CXX20 inline BasicString(
		const __Char* first,
		const __Char* last);

#if __cpp_lib_char8_t
	CONSTEXPR_CXX20 BasicString(std::u8string&& string);
	CONSTEXPR_CXX20 BasicString(const std::u8string& string);
#endif

	CONSTEXPR_CXX20 BasicString(BasicString&& string);
	CONSTEXPR_CXX20 BasicString(const BasicString& string);

	CONSTEXPR_CXX20 BasicString(std::string&& string);
	CONSTEXPR_CXX20 BasicString(const std::string& string);

	CONSTEXPR_CXX20 BasicString(std::wstring&& string);
	CONSTEXPR_CXX20 BasicString(const std::wstring& string);

	CONSTEXPR_CXX20 BasicString(std::u16string&& string);
	CONSTEXPR_CXX20 BasicString(const std::u16string& string);

	CONSTEXPR_CXX20 BasicString(std::u32string&& string);
	CONSTEXPR_CXX20 BasicString(const std::u32string& string);


	NODISCARD ValueType& operator[](const SizeType index) noexcept;
	NODISCARD const ValueType& operator[](const SizeType index) const noexcept;

	NODISCARD ValueType at(const SizeType index) const noexcept;
	NODISCARD inline Reference at(const SizeType offset) noexcept;

	CONSTEXPR_CXX20 inline NODISCARD SizeType count() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType size() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType length() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD SizeType capacity() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType unusedCapacity() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD Iterator begin() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ConstIterator begin() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ConstIterator cbegin() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ConstIterator constBegin() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD Iterator end() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ConstIterator end() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ConstIterator cend() const;
	CONSTEXPR_CXX20 inline NODISCARD ConstIterator constEnd() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ReverseIterator rbegin() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ReverseIterator rend() noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator rbegin() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator rend() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator crbegin() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator crend() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ReverseIterator reversedBegin() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ReverseIterator reversedEnd() noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator reversedBegin() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator reversedEnd() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator constReversedBegin() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ConstReverseIterator constReversedEnd() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ValueType front() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD Reference front() noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ValueType back() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD Reference back() noexcept;

	CONSTEXPR_CXX20 inline void push_back(const ValueType& element);
	CONSTEXPR_CXX20 inline void push_back(BasicString&& other);
	CONSTEXPR_CXX20 inline void push_back(ValueType&& element);

	CONSTEXPR_CXX20 inline void append(const ValueType& element);
	CONSTEXPR_CXX20 inline void append(BasicString&& other);
	CONSTEXPR_CXX20 inline void append(ValueType&& element);

	CONSTEXPR_CXX20 inline void prepend(const ValueType& element);
	CONSTEXPR_CXX20 inline void prepend(BasicString&& other);
	CONSTEXPR_CXX20 inline void prepend(ValueType&& element);

	CONSTEXPR_CXX20 inline void push_front(const ValueType& element);
	CONSTEXPR_CXX20 inline void push_front(BasicString&& other);
	CONSTEXPR_CXX20 inline void push_front(ValueType&& element);

	CONSTEXPR_CXX20 inline void pushFront(const ValueType& element);
	CONSTEXPR_CXX20 inline void pushFront(BasicString&& other);
	CONSTEXPR_CXX20 inline void pushFront(ValueType&& element);

	CONSTEXPR_CXX20 inline void pushBack(const ValueType& element);
	CONSTEXPR_CXX20 inline void pushBack(BasicString&& other);
	CONSTEXPR_CXX20 inline void pushBack(ValueType&& element);

	NODISCARD NativeString toNativeString() const noexcept;

	NODISCARD std::wstring toStdWString() const noexcept;
	NODISCARD std::string toStdString() const noexcept;

	NODISCARD std::u8string toStdUTF8String() const noexcept;
	NODISCARD std::u16string toStdUTF16String() const noexcept;
	NODISCARD std::u32string toStdUTF32String() const noexcept;

	NODISCARD SizeType size() const noexcept;
	NODISCARD SizeType length() const noexcept;

	NODISCARD SizeType capacity() const noexcept;

	NODISCARD ValueType pop() noexcept;

	NODISCARD ValueType popBack() noexcept;
	NODISCARD ValueType pop_back() noexcept;

	NODISCARD ValueType pop_front() noexcept;
	NODISCARD ValueType popFront() noexcept;

	NODISCARD SizeType find(const BasicString& string) const noexcept;
	NODISCARD SizeType find(ValueType ch) const noexcept;

	NODISCARD container::Vector<SizeType> findAll(const BasicString& string) const noexcept;
	NODISCARD container::Vector<SizeType> findAll(ValueType ch) const noexcept;

	NODISCARD SizeType findLastOf(const BasicString& string) const noexcept;
	NODISCARD SizeType findLastOf(ValueType ch) const noexcept;

	NODISCARD SizeType findFirstOf(const BasicString& string) const noexcept;
	NODISCARD SizeType findFirstOf(ValueType ch) const noexcept;

	NODISCARD SizeType findLastNotOf(const BasicString& string) const noexcept;
	NODISCARD SizeType findLastNotOf(ValueType ch) const noexcept;

	NODISCARD SizeType findFirstNotOf(const BasicString& string) const noexcept;
	NODISCARD SizeType findFirstNotOf(ValueType ch) const noexcept;

	NODISCARD bool isEmpty() const noexcept;
	NODISCARD bool isNull() const noexcept;

	NODISCARD ValueType* data() noexcept;
	NODISCARD inline ConstPointer data() const noexcept;
	NODISCARD const ValueType* constData() const noexcept;

	void clear();

	NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last);
	NODISCARD Iterator erase(ConstIterator it);

	NODISCARD ValueType front() const;
	NODISCARD ValueType& front();

	NODISCARD ValueType back() const;
	NODISCARD ValueType& back();

	void resize(SizeType size);
	void resize(
		SizeType size,
		ValueType fill);

	NODISCARD SizeType indexOf(
		ValueType ch,
		SizeType from = 0, 
		CaseSensitivity cs = CaseSensitive) const;
	NODISCARD SizeType indexOf(
		const BasicString& string,
		SizeType from = 0, 
		CaseSensitivity cs = CaseSensitive) const;

	NODISCARD SizeType lastIndexOf(
		ValueType c,
		CaseSensitivity cs = CaseSensitive) const noexcept;
	NODISCARD SizeType lastIndexOf(
		ValueType c,
		SizeType from, 
		CaseSensitivity cs = CaseSensitive) const;

	NODISCARD SizeType lastIndexOf(
		const BasicString& string,
		CaseSensitivity cs = CaseSensitive) const;
	NODISCARD SizeType lastIndexOf(
		const BasicString& string,
		SizeType from,
		CaseSensitivity cs = CaseSensitive) const;


	NODISCARD bool contains(
		ValueType ch,
		CaseSensitivity caseSensitivity = CaseSensitive) const;
	NODISCARD bool contains(
		const BasicString& string,
		CaseSensitivity caseSensitivity = CaseSensitive) const;

	NODISCARD SizeType count(
		ValueType ch,
		CaseSensitivity caseSensitivity = CaseSensitive) const;
	NODISCARD SizeType count(
		const BasicString& string,
		CaseSensitivity caseSensitivity = CaseSensitive) const;

	NODISCARD BasicString& fill(
		ValueType ch,
		SizeType size = -1);

	BasicString& append(ValueType ch);
	BasicString& append(
		const ValueType* ch,
		SizeType length);

	BasicString& append(const BasicString& string);

	BasicString& prepend(ValueType ch);
	BasicString& prepend(
		const ValueType* ch,
		SizeType len);

	BasicString& prepend(const BasicString& string);

	BasicString& remove(
		SizeType index,
		SizeType length);

	BasicString& remove(
		ValueType ch,
		CaseSensitivity caseSensitivity = CaseSensitive);
	BasicString& remove(
		const BasicString& string, 
		CaseSensitivity caseSensitivity = CaseSensitive);

	BasicString& removeAt(SizeType pos);

	BasicString& removeFirst();
	BasicString& removeLast();

	BasicString& replace(
		SizeType index, 
		SizeType length, 
		ValueType after);

	BasicString& replace(
		SizeType index,
		SizeType length, 
		const ValueType* ch,
		SizeType slen);
	BasicString& replace(
		SizeType index,
		SizeType length, 
		const BasicString& after);

	BasicString& replace(
		ValueType before,
		ValueType after,
		CaseSensitivity caseSensitivity = CaseSensitive);
	BasicString& replace(
		const ValueType* before,
		SizeType beforeLength, 
		const ValueType* after,
		SizeType afterLength, 
		CaseSensitivity caseSensitivity = CaseSensitive);

	BasicString& replace(
		const BasicString& before,
		const BasicString& after,
		CaseSensitivity caseSensitivity = CaseSensitive);
	BasicString& replace(
		ValueType ch,
		const BasicString& after,
		CaseSensitivity caseSensitivity = CaseSensitive);

	NODISCARD StringList split(
		const BasicString& sep,
		SplitBehavior behavior = KeepEmptyParts,
		CaseSensitivity cs = CaseSensitive) const;

	NODISCARD StringList split(
		ValueType sep,
		SplitBehavior behavior = KeepEmptyParts,
		CaseSensitivity caseSensibity = CaseSensitive) const;

	CONSTEXPR_CXX20 inline void reverse() noexcept;

	//increase capacity to newCapacity (without geometric growth)
	CONSTEXPR_CXX20 inline void reserve(BASE_GUARDOVERFLOW SizeType newCapacity);

	CONSTEXPR_CXX20 inline void shrinkToFit();
	CONSTEXPR_CXX20 inline void shrink_to_fit();

	CONSTEXPR_CXX20 inline NODISCARD bool contains(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool contains(const BasicString& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const BasicString& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const BasicString& subVector) const noexcept;
	CONSTEXPR_CXX20 inline void swap(BasicString& other);

	CONSTEXPR_CXX20 inline void swapAt(
		SizeType _First,
		SizeType _Last);
	CONSTEXPR_CXX20 inline void swapAt(
		Iterator _First,
		Iterator _Last);

	CONSTEXPR_CXX20 inline ValueType& first();
	CONSTEXPR_CXX20 inline const ValueType& first() const noexcept;
	CONSTEXPR_CXX20 inline const ValueType& constFirst() const noexcept;

	CONSTEXPR_CXX20 inline ValueType& last();
	CONSTEXPR_CXX20 inline const ValueType& last() const noexcept;
	CONSTEXPR_CXX20 inline const ValueType& constLast() const noexcept;

	CONSTEXPR_CXX20 inline void removeAll(const ValueType& element);

	constexpr inline NODISCARD sizetype max_size() const noexcept;
	constexpr inline NODISCARD sizetype maxSize() const noexcept;

	CONSTEXPR_CXX20 inline ConstPointer c_str() const;

	CONSTEXPR_CXX20 inline NODISCARD allocator_type& get_allocator() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD const allocator_type& get_allocator() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD allocator_type& getAllocator() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD const allocator_type& getAllocator() const noexcept;

	BasicString& assign(const BasicString& string);
	BasicString& assign(BasicString&& string);

	BasicString& assign(
		const BasicString&	string,
		const size_type		position,
		size_type			length);

	BasicString& assign(
		const value_type* string, 
		const size_type length);

	BasicString& assign(const value_type* string);
	BasicString& assign(std::initializer_list<value_type> initializerList);

	template <
		class _IteratorOrLength_,
		class _IteratorOrChar_>
	BasicString& assign(
		_IteratorOrLength_	firstOrLength,
		_IteratorOrChar_	lastOrChar);

	BasicString& insert(
		SizeType				firstPosition,
		const BasicString&		string,
		SizeType				secondPosition,
		SizeType				length);

	BasicString& insert(
		SizeType	position,
		SizeType	length,
		ValueType	ch);

	iterator insert(
		ConstIterator	where,
		const ValueType ch);

	BasicString& insert(
		SizeType index,
		ValueType ch);

	BasicString& insert(
		SizeType			firstPosition, 
		const BasicString&	string);

	BasicString& insert(
		SizeType			index,
		const ValueType*	chars,
		SizeType			length);

	BasicString& insert(
		SizeType index,
		const BasicString& string);
private:
};


// =========================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::~BasicString() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	const ValueType* chs,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	Iterator first,
	Iterator last)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
template <typename __Char>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	const __Char* str,
	size_type len)
{
	static_assert(is_compatible_char_v<__Char>, "Not supported __Char type");
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
template <typename __Char>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	const __Char* first,
	const __Char* last
) :
	BasicString(first, last - first)
{
	static_assert(is_compatible_char_v<__Char>, "Not supported __Char type");
}


#if __cpp_lib_char8_t

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::u8string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::u8string& string) {

}

#endif

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(BasicString&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::wstring&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::wstring& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::u16string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::u16string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::u32string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::u32string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::operator[](const SizeType index) noexcept 
{

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::operator[](const SizeType index) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::at(const SizeType index) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD NativeString BasicString<_Char_, _Traits_, _Allocator_>::toNativeString() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD std::wstring BasicString<_Char_, _Traits_, _Allocator_>::toStdWString() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD std::string BasicString<_Char_, _Traits_, _Allocator_>::toStdString() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD std::u8string BasicString<_Char_, _Traits_, _Allocator_>::toStdUTF8String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD std::u16string BasicString<_Char_, _Traits_, _Allocator_>::toStdUTF16String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD std::u32string BasicString<_Char_, _Traits_, _Allocator_>::toStdUTF32String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::size() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::length() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::capacity() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD inline BasicString<_Char_, _Traits_, _Allocator_>::Reference 
	BasicString<_Char_, _Traits_, _Allocator_>::at(const SizeType offset) noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::count() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::unusedCapacity() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::pop() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::popBack() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::pop_back() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::pop_front() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::popFront() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::find(const BasicString& string) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::find(ValueType ch) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_>::SizeType>
	BasicString<_Char_, _Traits_, _Allocator_>::findAll(const BasicString& string) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_>::SizeType> 
	BasicString<_Char_, _Traits_, _Allocator_>::findAll(ValueType ch) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(const BasicString& string) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(ValueType ch) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(const BasicString& string) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(ValueType ch) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(const BasicString& string) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(ValueType ch) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(const BasicString& string) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(ValueType ch) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::isEmpty() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::isNull() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::data() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::constData() const noexcept 
{

}
template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::data() const noexcept 
{

}



template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::begin() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::begin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::cbegin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator
	BasicString<_Char_, _Traits_, _Allocator_>::constBegin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::end() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::end() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::cend() const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::constEnd() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rbegin() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rend() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rbegin() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rend() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::crbegin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::crend() const noexcept 
{
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedBegin() noexcept
{
	return rbegin();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedEnd() noexcept 
{
	return rend();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedBegin() const noexcept
{
	return rbegin();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedEnd() const noexcept 
{
	return rend();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::constReversedBegin() const noexcept 
{
	return crbegin();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator
	BasicString<_Char_, _Traits_, _Allocator_>::constReversedEnd() const noexcept 
{
	return crend();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_back(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_back(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_back(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::append(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::append(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::append(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::prepend(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::prepend(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::prepend(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_front(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_front(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_front(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushFront(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushFront(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushFront(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushBack(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushBack(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushBack(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
void BasicString<_Char_, _Traits_, _Allocator_>::clear() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::erase(
		ConstIterator first,
		ConstIterator last)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::erase(ConstIterator it) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_>::front() const 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType&
	BasicString<_Char_, _Traits_, _Allocator_>::front() 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_>::back() const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType&
	BasicString<_Char_, _Traits_, _Allocator_>::back() 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
void BasicString<_Char_, _Traits_, _Allocator_>::resize(SizeType size) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
void BasicString<_Char_, _Traits_, _Allocator_>::resize(
	SizeType size,
	ValueType fill)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::indexOf(
		ValueType ch,
		SizeType from,
		CaseSensitivity cs) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::indexOf(
		const BasicString& string,
		SizeType from,
		CaseSensitivity cs) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		ValueType c,
		CaseSensitivity cs) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		ValueType c,
		SizeType from,
		CaseSensitivity cs) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		const BasicString& string,
		CaseSensitivity cs) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		const BasicString& string,
		SizeType from,
		CaseSensitivity cs) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::contains(
	ValueType ch,
	CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::contains(
	const BasicString& string,
	CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::count(
		ValueType ch,
		CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::count(
		const BasicString& string,
		CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>& 
	BasicString<_Char_, _Traits_, _Allocator_>::fill(
		ValueType ch,
		SizeType size)
{	

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	ValueType ch)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	const ValueType* ch,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	const BasicString& string)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(ValueType ch) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(
	const ValueType* ch,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(ValueType ch) {
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(
	const ValueType* ch,
	SizeType len)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	SizeType index,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	ValueType ch,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	const BasicString& string,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeAt(SizeType pos) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeFirst() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeLast() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	ValueType after)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	const ValueType* ch,
	SizeType slen)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	const BasicString& after)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	ValueType before,
	ValueType after,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	const ValueType* before,
	SizeType beforeLength,
	const ValueType* after,
	SizeType afterLength,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	const BasicString& before,
	const BasicString& after,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	ValueType ch,
	const BasicString& after,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::StringList 
	BasicString<_Char_, _Traits_, _Allocator_>::split(
		const BasicString& sep,
		SplitBehavior behavior,
		CaseSensitivity cs) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::StringList 
	BasicString<_Char_, _Traits_, _Allocator_>::split(
		ValueType sep,
		SplitBehavior behavior,
		CaseSensitivity caseSensibity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::reverse() noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>
	::reserve(BASE_GUARDOVERFLOW SizeType newCapacity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::shrinkToFit() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::shrink_to_fit() {
	 
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::contains(const ValueType& element) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::
	contains(const BasicString& subString) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::startsWith(const ValueType& element) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::startsWith(const BasicString& subVector) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::endsWith(const ValueType& element) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::endsWith(const BasicString& subVector) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::swap(BasicString& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::swapAt(
	SizeType _First,
	SizeType _Last) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::swapAt(
	Iterator _First,
	Iterator _Last)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::first() 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::first() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::constFirst() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::last()
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::last() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::constLast() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::removeAll(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
constexpr inline NODISCARD sizetype BasicString<_Char_, _Traits_, _Allocator_>::max_size() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
constexpr inline NODISCARD sizetype BasicString<_Char_, _Traits_, _Allocator_>::maxSize() const noexcept {
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::ConstPointer 
	BasicString<_Char_, _Traits_, _Allocator_>::c_str() const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::allocator_type&
	BasicString<_Char_, _Traits_, _Allocator_>::get_allocator() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_>::get_allocator() const noexcept 
{

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_>::getAllocator() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_>::getAllocator() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(BasicString&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(
	const BasicString&	string,
	const size_type		position,
	size_type			length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(
	const value_type* string,
	const size_type length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(const value_type* string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>
	::assign(std::initializer_list<value_type> initializerList)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
template <
	class _IteratorOrLength_,
	class _IteratorOrChar_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(
	_IteratorOrLength_	firstOrLength,
	_IteratorOrChar_	lastOrChar)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType				firstPosition,
	const BasicString&		string,
	SizeType				secondPosition,
	SizeType				length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType	position,
	SizeType	length,
	ValueType	ch)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_>
BasicString<_Char_, _Traits_, _Allocator_>::iterator BasicString<_Char_, _Traits_, _Allocator_>::insert(
	ConstIterator	where,
	const ValueType ch)
{

}

__BASE_STRING_NAMESPACE_END
