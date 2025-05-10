#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/string/StringIterator.h>
#include <src/core/string/CharTypeCompatibility.h>

#include <src/core/string/StringDebug.h>
#include <base/core/container/Vector.h>



WARNING_DISABLE_MSVC(4834)
WARNING_DISABLE_MSVC(4002)
WARNING_DISABLE_MSVC(4003)


__BASE_STRING_NAMESPACE_BEGIN


template <
	class _Char_,
	class _Traits_				= CharTraits<_Char_>,
	class _Allocator_			= std::allocator<_Char_>,
	class _SimdOptimization_	= StringSimd::_Optimization_Disable_,
	class _Storage_				= BasicStringStorage<_Char_, _SimdOptimization_>>
class BasicString 
{
public:
	using allocator_type	= _Allocator_;
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

	using reverse_iterator	= std::reverse_iterator<iterator>;
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
		const ValueType* string, 
		const size_type length);

	BasicString& assign(const ValueType* string);
	BasicString& assign(std::initializer_list<ValueType> initializerList);

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

	SizeType find(
		const BasicString&	str, 
		SizeType			position = 0) const;

	SizeType find(
		const ValueType*	needle,
		SizeType			position,
		SizeType			size) const;

	SizeType find(
		const ValueType*	string, 
		SizeType			position = 0) const;

	SizeType find(
		ValueType	ch, 
		SizeType	position = 0) const;

	SizeType rfind(
		const BasicString&	string, 
		SizeType			position = -1) const;

	SizeType rfind(
		const ValueType*	string,
		SizeType			position, 
		SizeType			length) const;

	SizeType rfind(
		const ValueType*	string,
		SizeType			pos = -1) const;

	SizeType rfind(
		ValueType	ch, 
		SizeType	position = -1) const;

	SizeType find_first_of(
		const BasicString&	string,
		SizeType			position = 0) const;

	SizeType find_first_of(
		const ValueType*	string,
		SizeType			position,
		SizeType			length) const;

	SizeType find_first_of(
		const ValueType*	string,
		SizeType			position = 0) const;

	SizeType find_first_of(
		ValueType	ch,
		SizeType	position = 0) const;

	SizeType find_last_of(
		const BasicString&	string,
		SizeType			position = -1) const;

	SizeType find_last_of(
		const ValueType*	string,
		SizeType			position,
		SizeType n) const;

	SizeType find_last_of(
		const ValueType*	string, 
		SizeType			position = -1) const;

	SizeType find_last_of(
		ValueType	ch,
		SizeType	position = -1) const;

	SizeType find_first_not_of(
		const BasicString&	string,
		SizeType			position = 0) const;

	SizeType find_first_not_of(
		const ValueType*	string,
		SizeType			position, 
		SizeType			length) const;

	SizeType find_first_not_of(
		const ValueType*	string,
		SizeType			position = 0) const;

	SizeType find_first_not_of(
		ValueType	ch,
		SizeType	position = 0) const;

	SizeType find_last_not_of(
		const BasicString&	string,
		SizeType			position = -1) const;

	SizeType find_last_not_of(
		const ValueType*	string,
		SizeType			position, 
		SizeType			length) const;

	SizeType find_last_not_of(
		const ValueType*	string, 
		SizeType			position = -1) const;

	SizeType find_last_not_of(
		ValueType	ch,
		SizeType	position = -1) const;

	BasicString substr(
		SizeType position = 0,
		SizeType length = -1) const&;

	BasicString substr(
		SizeType position = 0,
		SizeType length = -1) &&;

	int compare(const BasicString& str) const;

	int compare(
		SizeType			firstPosition,
		SizeType			firstLength,
		const BasicString&	string) const;

	int compare(
		SizeType			firstPosition, 
		SizeType			firstLength,
		const ValueType*	string) const;

	int compare(
		SizeType			firstPosition,
		SizeType			firstLength,
		const ValueType*	string,
		SizeType			secondLength) const;

	int compare(
		SizeType			firstPosition,
		SizeType			firstLength,
		const BasicString&	string,
		SizeType			secondPosition,
		SizeType			secondLength) const;

	int compare(const ValueType* string) const;

	NODISCARD container::Vector<SizeType> findAll(const BasicString& string) const noexcept;
	NODISCARD container::Vector<SizeType> findAll(ValueType ch) const noexcept;

	SizeType findFirstOf(
		const BasicString&	string,
		SizeType			position) const;
	SizeType findFirstOf(
		const ValueType*	string,
		SizeType			position,
		SizeType			length) const;

	SizeType findFirstOf(
		const ValueType*	string,
		SizeType			position) const;
	SizeType findFirstOf(
		ValueType	ch,
		SizeType	position) const;

	SizeType findLastOf(
		const BasicString&	string,
		SizeType			position) const;
	SizeType findLastOf(
		const ValueType*	string,
		SizeType			position,
		SizeType			length) const;

	SizeType findLastOf(
		const ValueType*	string,
		SizeType			position) const;
	SizeType findLastOf(
		ValueType	ch,
		SizeType	position) const;

	SizeType findFirstNotOf(
		const BasicString&	string,
		SizeType			position) const;
	SizeType findFirstNotOf(
		const ValueType*	string,
		SizeType			position,
		SizeType			length) const;

	SizeType findFirstNotOf(
		const ValueType*	string,
		SizeType			position) const;
	SizeType findFirstNotOf(
		ValueType	ch,
		SizeType	position) const;

	SizeType findLastNotOf(
		const BasicString&	string,
		SizeType			position) const;
	SizeType findLastNotOf(
		const ValueType*	string,
		SizeType			position,
		SizeType			length) const;

	SizeType findLastNotOf(
		const ValueType*	string,
		SizeType			position) const;
	SizeType findLastNotOf(
		ValueType	ch,
		SizeType	position) const;
private:
};


// =========================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::~BasicString() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	const ValueType* chs,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(
	Iterator first,
	Iterator last)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::u8string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::u8string& string) {

}

#endif

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(BasicString&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::wstring&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::wstring& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::u16string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::u16string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(std::u32string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::BasicString(const std::u32string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::operator[](const SizeType index) noexcept 
{

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::operator[](const SizeType index) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::at(const SizeType index) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD NativeString BasicString<_Char_, _Traits_, _Allocator_>::toNativeString() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::wstring BasicString<_Char_, _Traits_, _Allocator_>::toStdWString() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::string BasicString<_Char_, _Traits_, _Allocator_>::toStdString() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::u8string BasicString<_Char_, _Traits_, _Allocator_>::toStdUTF8String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::u16string BasicString<_Char_, _Traits_, _Allocator_>::toStdUTF16String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::u32string BasicString<_Char_, _Traits_, _Allocator_>::toStdUTF32String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::size() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::length() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::capacity() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Reference 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::at(const SizeType offset) noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::count() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::unusedCapacity() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::pop() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::popBack() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::pop_back() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::pop_front() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_>::popFront() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_>::SizeType>
	BasicString<_Char_, _Traits_, _Allocator_>::findAll(const BasicString& string) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_>::SizeType> 
	BasicString<_Char_, _Traits_, _Allocator_>::findAll(ValueType ch) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(
	const BasicString& string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(
	const ValueType* string,
	SizeType			position,
	SizeType			length) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(
	const ValueType* string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstOf(
	ValueType	ch,
	SizeType	position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(
	const BasicString& string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(
	const ValueType* string,
	SizeType			position,
	SizeType			length) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(
	const ValueType* string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastOf(
	ValueType	ch,
	SizeType	position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(
	const BasicString& string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(
	const ValueType* string,
	SizeType			position,
	SizeType			length) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(
	const ValueType* string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findFirstNotOf(
	ValueType	ch,
	SizeType	position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(
	const BasicString& string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(
	const ValueType* string,
	SizeType			position,
	SizeType			length) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(
	const ValueType* string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::findLastNotOf(
	ValueType	ch,
	SizeType	position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::isEmpty() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::isNull() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::data() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::constData() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_>::data() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::begin() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::begin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::cbegin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator
	BasicString<_Char_, _Traits_, _Allocator_>::constBegin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::end() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::end() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::cend() const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::constEnd() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rbegin() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rend() noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rbegin() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::rend() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::crbegin() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::crend() const noexcept 
{
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedBegin() noexcept
{
	return rbegin();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedEnd() noexcept 
{
	return rend();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedBegin() const noexcept
{
	return rbegin();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::reversedEnd() const noexcept 
{
	return rend();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_>::constReversedBegin() const noexcept 
{
	return crbegin();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ConstReverseIterator
	BasicString<_Char_, _Traits_, _Allocator_>::constReversedEnd() const noexcept 
{
	return crend();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_back(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_back(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_back(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::append(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::append(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::append(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::prepend(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::prepend(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::prepend(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_front(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_front(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::push_front(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushFront(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushFront(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushFront(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushBack(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushBack(BasicString&& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::pushBack(ValueType&& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
void BasicString<_Char_, _Traits_, _Allocator_>::clear() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::erase(
		ConstIterator first,
		ConstIterator last)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_>::erase(ConstIterator it) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_>::front() const 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType&
	BasicString<_Char_, _Traits_, _Allocator_>::front() 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_>::back() const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::ValueType&
	BasicString<_Char_, _Traits_, _Allocator_>::back() 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
void BasicString<_Char_, _Traits_, _Allocator_>::resize(SizeType size) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
void BasicString<_Char_, _Traits_, _Allocator_>::resize(
	SizeType size,
	ValueType fill)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		ValueType c,
		CaseSensitivity cs) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::lastIndexOf(
		const BasicString& string,
		CaseSensitivity cs) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::contains(
	ValueType ch,
	CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::contains(
	const BasicString& string,
	CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_>::count(
		ValueType ch,
		CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_>::count(
		const BasicString& string,
		CaseSensitivity caseSensitivity) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_>& 
	BasicString<_Char_, _Traits_, _Allocator_>::fill(
		ValueType ch,
		SizeType size)
{	

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	ValueType ch)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	const ValueType* ch,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType index,
	const BasicString& string)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(ValueType ch) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(
	const ValueType* ch,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::append(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(ValueType ch) {
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(
	const ValueType* ch,
	SizeType len)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::prepend(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	SizeType index,
	SizeType length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	ValueType ch,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::remove(
	const BasicString& string,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeAt(SizeType pos) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeFirst() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::removeLast() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	ValueType after)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	SizeType index,
	SizeType length,
	const BasicString& after)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	ValueType before,
	ValueType after,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	const BasicString& before,
	const BasicString& after,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::replace(
	ValueType ch,
	const BasicString& after,
	CaseSensitivity caseSensitivity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::reverse() noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>
	::reserve(BASE_GUARDOVERFLOW SizeType newCapacity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::shrinkToFit() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::shrink_to_fit() {
	 
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::contains(const ValueType& element) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>::
	contains(const BasicString& subString) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::startsWith(const ValueType& element) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::startsWith(const BasicString& subVector) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::endsWith(const ValueType& element) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_>
	::endsWith(const BasicString& subVector) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::swap(BasicString& other) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::swapAt(
	SizeType _First,
	SizeType _Last) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::swapAt(
	Iterator _First,
	Iterator _Last)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::first() 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::first() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::constFirst() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::last()
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::last() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_>::constLast() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_>::removeAll(const ValueType& element) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
constexpr inline NODISCARD sizetype BasicString<_Char_, _Traits_, _Allocator_>::max_size() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
constexpr inline NODISCARD sizetype BasicString<_Char_, _Traits_, _Allocator_>::maxSize() const noexcept {
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_>::ConstPointer 
	BasicString<_Char_, _Traits_, _Allocator_>::c_str() const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::allocator_type&
	BasicString<_Char_, _Traits_, _Allocator_>::get_allocator() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_>::get_allocator() const noexcept 
{

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_>::getAllocator() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD const BasicString<_Char_, _Traits_, _Allocator_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_>::getAllocator() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(BasicString&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(
	const BasicString&	string,
	const size_type		position,
	size_type			length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(
	const ValueType* string,
	const size_type length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::assign(const ValueType* string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>
	::assign(std::initializer_list<ValueType> initializerList)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
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
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>& BasicString<_Char_, _Traits_, _Allocator_>::insert(
	SizeType	position,
	SizeType	length,
	ValueType	ch)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::iterator BasicString<_Char_, _Traits_, _Allocator_>::insert(
	ConstIterator	where,
	const ValueType ch)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find(
	const BasicString&	string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find(
	const ValueType*	needle,
	SizeType			position,
	SizeType			size) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find(
	const ValueType*	string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find(
	ValueType	ch,
	SizeType	position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::rfind(
	const BasicString&	string,
	SizeType			position) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::rfind(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::rfind(
	const ValueType*	string,
	SizeType			pos) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::rfind(
	ValueType	ch,
	SizeType	position) const
{
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_of(
	const BasicString&	string,
	SizeType			position) const
{
	return findFirstOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_of(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{
	return findFirstOf(string, position, length);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_of(
	const ValueType*	string,
	SizeType			position) const
{
	return findFirstOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_of(
	ValueType	ch,
	SizeType	position) const
{
	return findFirstOf(ch, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_of(
	const BasicString&	string,
	SizeType			position) const
{
	return findLastOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_of(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{
	return findLastOf(string, position, length);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_of(
	const ValueType*	string,
	SizeType			position) const
{
	return findLastOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_of(
	ValueType	ch,
	SizeType	position) const
{
	return findLastOf(ch, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_not_of(
	const BasicString&	string,
	SizeType			position) const
{
	return findFirstNotOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_not_of(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{
	return findFirstNotOf(string, position, length);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_not_of(
	const ValueType*	string,
	SizeType			position) const
{
	return findFirstNotOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_first_not_of(
	ValueType	ch,
	SizeType	position) const
{
	return findFirstNotOf(ch, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_not_of(
	const BasicString&	string,
	SizeType			position) const
{
	return findLastNotOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_not_of(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{
	return findLastNotOf(string, position, length);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_not_of(
	const ValueType*	string,
	SizeType			position) const
{
	return findLastNotOf(string, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_>::SizeType BasicString<_Char_, _Traits_, _Allocator_>::find_last_not_of(
	ValueType	ch,
	SizeType	position) const
{
	return findLastNotOf(ch, position);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_> BasicString<_Char_, _Traits_, _Allocator_>::substr(
	SizeType position,
	SizeType length) const&
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_> BasicString<_Char_, _Traits_, _Allocator_>::substr(
	SizeType position,
	SizeType length)&&
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
int BasicString<_Char_, _Traits_, _Allocator_>::compare(const BasicString& strøòï) const {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
int BasicString<_Char_, _Traits_, _Allocator_>::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const BasicString&	string) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
int BasicString<_Char_, _Traits_, _Allocator_>::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const ValueType*	string) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
int BasicString<_Char_, _Traits_, _Allocator_>::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const ValueType*	string,
	SizeType			secondLength) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
int BasicString<_Char_, _Traits_, _Allocator_>::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const BasicString&	string,
	SizeType			secondPosition,
	SizeType			secondLength) const
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
int BasicString<_Char_, _Traits_, _Allocator_>::compare(const ValueType* string) const {

}

//template <typename E, class T, class A, class S>
//inline BasicString<E, T, A, S> operator+(
//	BasicString<E, T, A, S>&& lhs, E rhs) {
//  //
//  return std::move(lhs += rhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator==(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator==(std::nullptr_t, const basic_fbstring<E, T, A, S>&) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator==(
//    const typename basic_fbstring<E, T, A, S>::value_type* lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return rhs == lhs;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator==(const basic_fbstring<E, T, A, S>&, std::nullptr_t) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator==(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const typename basic_fbstring<E, T, A, S>::value_type* rhs) {
//  return lhs.compare(rhs) == 0;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator!=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(lhs == rhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator!=(std::nullptr_t, const basic_fbstring<E, T, A, S>&) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator!=(
//    const typename basic_fbstring<E, T, A, S>::value_type* lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(lhs == rhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator!=(const basic_fbstring<E, T, A, S>&, std::nullptr_t) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator!=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const typename basic_fbstring<E, T, A, S>::value_type* rhs) {
//  return !(lhs == rhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator<(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return lhs.compare(rhs) < 0;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator<(const basic_fbstring<E, T, A, S>&, std::nullptr_t) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator<(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const typename basic_fbstring<E, T, A, S>::value_type* rhs) {
//  return lhs.compare(rhs) < 0;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator<(std::nullptr_t, const basic_fbstring<E, T, A, S>&) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator<(
//    const typename basic_fbstring<E, T, A, S>::value_type* lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return rhs.compare(lhs) > 0;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator>(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return rhs < lhs;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator>(const basic_fbstring<E, T, A, S>&, std::nullptr_t) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator>(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const typename basic_fbstring<E, T, A, S>::value_type* rhs) {
//  return rhs < lhs;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator>(std::nullptr_t, const basic_fbstring<E, T, A, S>&) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator>(
//    const typename basic_fbstring<E, T, A, S>::value_type* lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return rhs < lhs;
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator<=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(rhs < lhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator<=(const basic_fbstring<E, T, A, S>&, std::nullptr_t) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator<=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const typename basic_fbstring<E, T, A, S>::value_type* rhs) {
//  return !(rhs < lhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator<=(std::nullptr_t, const basic_fbstring<E, T, A, S>&) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator<=(
//    const typename basic_fbstring<E, T, A, S>::value_type* lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(rhs < lhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator>=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(lhs < rhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator>=(const basic_fbstring<E, T, A, S>&, std::nullptr_t) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator>=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const typename basic_fbstring<E, T, A, S>::value_type* rhs) {
//  return !(lhs < rhs);
//}
//
//template <typename E, class T, class A, class S>
//inline bool operator>=(std::nullptr_t, const basic_fbstring<E, T, A, S>&) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator>=(
//    const typename basic_fbstring<E, T, A, S>::value_type* lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(lhs < rhs);
//}
//
//#if FOLLY_CPLUSPLUS >= 202002
//template <typename E, class T, class A, class S>
//inline bool operator<=>(std::nullptr_t, const basic_fbstring<E, T, A, S>&) =
//    delete;
//
//template <typename E, class T, class A, class S>
//inline bool operator<=>(const basic_fbstring<E, T, A, S>&, std::nullptr_t) =
//    delete;
//#endif
//
//// C++11 21.4.8.8
//template <typename E, class T, class A, class S>
//void swap(basic_fbstring<E, T, A, S>& lhs, basic_fbstring<E, T, A, S>& rhs) {
//  lhs.swap(rhs);
//}
//
//// TODO: make this faster.
//template <typename E, class T, class A, class S>
//inline std::basic_istream<
//    typename basic_fbstring<E, T, A, S>::value_type,
//    typename basic_fbstring<E, T, A, S>::traits_type>&
//operator>>(
//    std::basic_istream<
//        typename basic_fbstring<E, T, A, S>::value_type,
//        typename basic_fbstring<E, T, A, S>::traits_type>& is,
//    basic_fbstring<E, T, A, S>& str) {
//  typedef std::basic_istream<
//      typename basic_fbstring<E, T, A, S>::value_type,
//      typename basic_fbstring<E, T, A, S>::traits_type>
//      _istream_type;
//  typename _istream_type::sentry sentry(is);
//  size_t extracted = 0;
//  typename _istream_type::iostate err = _istream_type::goodbit;
//  if (sentry) {
//    auto n = is.width();
//    if (n <= 0) {
//      n = str.max_size();
//    }
//    str.erase();
//    for (auto got = is.rdbuf()->sgetc(); extracted != size_t(n); ++extracted) {
//      if (got == T::eof()) {
//        err |= _istream_type::eofbit;
//        is.width(0);
//        break;
//      }
//      if (isspace(got)) {
//        break;
//      }
//      str.push_back(got);
//      got = is.rdbuf()->snextc();
//    }
//  }
//  if (!extracted) {
//    err |= _istream_type::failbit;
//  }
//  if (err) {
//    is.setstate(err);
//  }
//  return is;
//}
//
//template <typename E, class T, class A, class S>
//inline std::basic_ostream<
//    typename basic_fbstring<E, T, A, S>::value_type,
//    typename basic_fbstring<E, T, A, S>::traits_type>&
//operator<<(
//    std::basic_ostream<
//        typename basic_fbstring<E, T, A, S>::value_type,
//        typename basic_fbstring<E, T, A, S>::traits_type>& os,
//    const basic_fbstring<E, T, A, S>& str) {
//#ifdef _LIBCPP_VERSION
//  typedef std::basic_ostream<
//      typename basic_fbstring<E, T, A, S>::value_type,
//      typename basic_fbstring<E, T, A, S>::traits_type>
//      _ostream_type;
//  typename _ostream_type::sentry _s(os);
//  if (_s) {
//    typedef std::ostreambuf_iterator<
//        typename basic_fbstring<E, T, A, S>::value_type,
//        typename basic_fbstring<E, T, A, S>::traits_type>
//        _Ip;
//    size_t __len = str.size();
//    bool __left =
//        (os.flags() & _ostream_type::adjustfield) == _ostream_type::left;
//    if (__pad_and_output(
//            _Ip(os),
//            str.data(),
//            __left ? str.data() + __len : str.data(),
//            str.data() + __len,
//            os,
//            os.fill())
//            .failed()) {
//      os.setstate(_ostream_type::badbit | _ostream_type::failbit);
//    }
//  }
//#elif defined(_MSC_VER)
//  typedef decltype(os.precision()) streamsize;
//  // MSVC doesn't define __ostream_insert
//  os.write(str.data(), static_cast<streamsize>(str.size()));
//#else
//  std::__ostream_insert(os, str.data(), str.size());
//#endif
//  return os;
//}
//
//// basic_string compatibility routines
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator==(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const std::basic_string<E, T, A2>& rhs) {
//  return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) == 0;
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator==(
//    const std::basic_string<E, T, A2>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return rhs == lhs;
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator!=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const std::basic_string<E, T, A2>& rhs) {
//  return !(lhs == rhs);
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator!=(
//    const std::basic_string<E, T, A2>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(lhs == rhs);
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator<(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const std::basic_string<E, T, A2>& rhs) {
//  return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) < 0;
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator>(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const std::basic_string<E, T, A2>& rhs) {
//  return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) > 0;
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator<(
//    const std::basic_string<E, T, A2>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return rhs > lhs;
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator>(
//    const std::basic_string<E, T, A2>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return rhs < lhs;
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator<=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const std::basic_string<E, T, A2>& rhs) {
//  return !(lhs > rhs);
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator>=(
//    const basic_fbstring<E, T, A, S>& lhs,
//    const std::basic_string<E, T, A2>& rhs) {
//  return !(lhs < rhs);
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator<=(
//    const std::basic_string<E, T, A2>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(lhs > rhs);
//}
//
//template <typename E, class T, class A, class S, class A2>
//inline bool operator>=(
//    const std::basic_string<E, T, A2>& lhs,
//    const basic_fbstring<E, T, A, S>& rhs) {
//  return !(lhs < rhs);
//}

__BASE_STRING_NAMESPACE_END
