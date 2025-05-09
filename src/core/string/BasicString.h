#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/string/StringIterator.h>
#include <src/core/string/CharTypeCompatibility.h>

#include <src/core/string/StringDebug.h>
#include <base/core/container/Vector.h>

__BASE_STRING_NAMESPACE_BEGIN

#ifndef _BASIC_STRING_OUTSIDE_TEMPLATE_
#  define _BASIC_STRING_OUTSIDE_TEMPLATE_ template <class _Char_, class _Traits_, class _Allocator_>
#endif

template <
	class _Char_,
	class _Traits_,
	class _Allocator_ = std::allocator<_Char_>>
class BasicString 
{
public:
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

	using ValueType			= value_type;

	using Reference			= reference;
	using ConstReference	= const_reference;

	using Pointer			= pointer;
	using ConstPointer		= const_pointer;

	using SizeType			= size_type;
	using DifferenceType	= ptrdiff;

	using Iterator			= iterator;
	using ConstIterator		= const_iterator;

	template <typename _Char>
	using if_compatible_char = typename std::enable_if<
		IsCompatibleCharType<_Char>::value, bool>::type;

	template <typename Pointer>
	using if_compatible_pointer = typename std::enable_if<
		IsCompatiblePointer<Pointer>::value, bool>::type;

	template <typename T>
	using if_compatible_string_like = typename std::enable_if<
		std::is_same<T, BasicString>::value, bool>::type;

	template <typename T>
	using if_compatible_container = typename std::enable_if<
		IsContainerCompatibleWithStringView<T>::value, bool>::type;

	enum _SplitBehavior : uchar {
		KeepEmptyParts = 0,
		SkipEmptyParts = 0x1,
	};

	DECLARE_FLAGS_ENUM(SplitBehavior, _SplitBehavior);

	CONSTEXPR_CXX20 BasicString();
	CONSTEXPR_CXX20 ~BasicString();

	CONSTEXPR_CXX20 BasicString(const BasicString& string);
	CONSTEXPR_CXX20 BasicString(
		const ValueType* chs, 
		SizeType length);

	CONSTEXPR_CXX20 BasicString(
		Iterator first,
		Iterator last);

	template <
		typename __Char, 
		if_compatible_char<__Char> = true>
	CONSTEXPR_CXX20 inline BasicString(const __Char* str, size_type len);

	template <
		typename __Char,
		if_compatible_char<__Char> = true>
	CONSTEXPR_CXX20 inline BasicString(
		const __Char* first,
		const __Char* last);

	NODISCARD ValueType& operator[](const SizeType index) noexcept;
	NODISCARD const ValueType& operator[](const SizeType index) const noexcept;

	NODISCARD ValueType at(const SizeType index) const noexcept;

	NODISCARD NativeString toNativeString() const noexcept;

	NODISCARD std::wstring toStdWString() const noexcept;
	NODISCARD std::string toStdString() const noexcept;

	NODISCARD SizeType size() const noexcept;
	NODISCARD SizeType length() const noexcept;

	NODISCARD SizeType capacity() const noexcept;

	NODISCARD ValueType pop() noexcept;

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

	NODISCARD ValueType* data() const noexcept;
	NODISCARD const ValueType* constData() const noexcept;

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

	void push_back(ValueType c);
	void push_back(const BasicString& s);

	void push_front(ValueType c);
	void push_front(const BasicString& s);

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

	BasicString& insert(
		SizeType index,
		ValueType ch);
	BasicString& insert(
		SizeType index,
		const ValueType* ch,
		SizeType length);

	BasicString& insert(
		SizeType index,
		const BasicString& string);

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

	using StringList = std::vector<String>;

	NODISCARD StringList split(
		const BasicString& sep,
		SplitBehavior behavior = KeepEmptyParts,
		CaseSensitivity cs = CaseSensitive) const;

	NODISCARD StringList split(
		ValueType sep,
		SplitBehavior behavior = KeepEmptyParts,
		CaseSensitivity caseSensibity = CaseSensitive) const;

private:
};


// =========================================================================================

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString() {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::~BasicString() {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const BasicString& string) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	const ValueType* chs,
	SizeType length)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	Iterator first,
	Iterator last)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
template <typename __Char>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	const __Char* str,
	size_type len)
{
	for (size_type i = 0; i < len; ++i)
		_data.push_back(str[i]);
}

_BASIC_STRING_OUTSIDE_TEMPLATE_
template <typename __Char>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	const __Char* first,
	const __Char* last
) :
	BasicString(first, last - first)
{}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::operator[](const SizeType index) noexcept 
{

}


_BASIC_STRING_OUTSIDE_TEMPLATE_ 
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::operator[](const SizeType index) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::at(const SizeType index) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD NativeString BasicString<_Char_, _Traits_, _Allocator_>::toNativeString() const noexcept {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD std::wstring BasicString<_Char_, _Traits_, _Allocator_>::toStdWString() const noexcept {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD std::string BasicString<_Char_, _Traits_, _Allocator_>::toStdString() const noexcept {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::size() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::length() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::capacity() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::pop() noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::find(const BasicString& string) const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::find(ValueType ch) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_>::SizeType>
	BasicString<_Char_, _Traits_, _Allocator_>::findAll(const BasicString& string) const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_>::SizeType> 
	BasicString<_Char_, _Traits_, _Allocator_>::findAll(ValueType ch) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(const BasicString& string) const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(ValueType ch) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(const BasicString& string) const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(ValueType ch) const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(const BasicString& string) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(ValueType ch) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(const BasicString& string) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(ValueType ch) const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::isEmpty() const noexcept {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::isNull() const noexcept {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::data() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::constData() const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::begin() noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::begin() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::cbegin() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator
	BasicString<_Char_, _Traits_, _Allocator_>::constBegin() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::end() noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::end() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::cend() const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::constEnd() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rbegin() noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rend() noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rbegin() const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rend() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::crbegin() const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::crend() const noexcept 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
void BasicString<_Char_, _Traits_, _Allocator_>::push_back(ValueType c) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
void BasicString<_Char_, _Traits_, _Allocator_>::push_back(const BasicString& s) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
void BasicString<_Char_, _Traits_, _Allocator_>::push_front(ValueType c) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
void BasicString<_Char_, _Traits_, _Allocator_>::push_front(const BasicString& s) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
void BasicString<_Char_, _Traits_, _Allocator_>::clear() {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::erase(
		ConstIterator first,
		ConstIterator last)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::erase(ConstIterator it) 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::front() const 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::front() 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::back() const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::back() 
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
void BasicString<_Char_, _Traits_, _Allocator_>::resize(SizeType size) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
void BasicString<_Char_, _Traits_, _Allocator_>::resize(
	SizeType size,
	ValueType fill)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::indexOf(
		ValueType ch,
		SizeType from,
		CaseSensitivity cs) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::indexOf(
		const BasicString& string,
		SizeType from,
		CaseSensitivity cs) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		ValueType c,
		CaseSensitivity cs) const noexcept
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		ValueType c,
		SizeType from,
		CaseSensitivity cs) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		const BasicString& string,
		CaseSensitivity cs) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		const BasicString& string,
		SizeType from,
		CaseSensitivity cs) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::contains(
	ValueType ch,
	CaseSensitivity caseSensitivity) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::contains(
	const BasicString& string,
	CaseSensitivity caseSensitivity) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::count(
		ValueType ch,
		CaseSensitivity caseSensitivity) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::count(
		const BasicString& string,
		CaseSensitivity caseSensitivity) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>& 
	BasicString<_Char_, _Traits_, _Allocator_>::fill(
		ValueType ch,
		SizeType size)
{	

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	ValueType ch)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	const ValueType* ch,
	SizeType length)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	const BasicString& string)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(ValueType ch) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(
	const ValueType* ch,
	SizeType length)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(const BasicString& string) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(ValueType ch) {
	
}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(
	const ValueType* ch,
	SizeType len)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(const BasicString& string) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	SizeType index,
	SizeType length)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	ValueType ch,
	CaseSensitivity caseSensitivity)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	const BasicString& string,
	CaseSensitivity caseSensitivity)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeAt(SizeType pos) {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeFirst() {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeLast() {

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	ValueType after)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	const ValueType* ch,
	SizeType slen)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	const BasicString& after)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	ValueType before,
	ValueType after,
	CaseSensitivity caseSensitivity)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	const ValueType* before,
	SizeType beforeLength,
	const ValueType* after,
	SizeType afterLength,
	CaseSensitivity caseSensitivity)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	const BasicString& before,
	const BasicString& after,
	CaseSensitivity caseSensitivity)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	ValueType ch,
	const BasicString& after,
	CaseSensitivity caseSensitivity = CaseSensitive)
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::StringList 
	BasicString<_Char_, _Traits_, _Allocator_>::split(
		const BasicString& sep,
		SplitBehavior behavior,
		CaseSensitivity cs) const
{

}

_BASIC_STRING_OUTSIDE_TEMPLATE_
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::StringList 
	BasicString<_Char_, _Traits_, _Allocator_>::split(
		ValueType sep,
		SplitBehavior behavior,
		CaseSensitivity caseSensibity) const
{

}

__BASE_STRING_NAMESPACE_END
