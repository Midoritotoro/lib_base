#pragma once

#include <base/core/arch/Platform.h>

#include <base/core/string/StringIterator.h>
#include <src/core/string/CharTypeCompatibility.h>

#include <src/core/string/StringDebug.h>
#include <base/core/container/Vector.h>

#include <src/core/string/BasicStringStorage.h>
#include <src/core/string/CharTraits.h>

#include <src/core/utility/simd/SimdAlgorithm.h>
#include <base/core/string/StringConverter.h>

#include <src/core/utility/simd/SimdVectorType.h>

WARNING_DISABLE_MSVC(4834)
WARNING_DISABLE_MSVC(4002)
WARNING_DISABLE_MSVC(4003)

#if !defined(__BASE_BS_TEMPLATE)
#  define __BASE_BS_TEMPLATE template <class _Char_, class _Traits_, class _Allocator_, class _SimdOptimization_, class _Storage_>
#endif 

#if !defined(__BASE_BS)
#  define __BASE_BS BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
#endif 

__BASE_STRING_NAMESPACE_BEGIN

template <
	class _Char_,
	class _Traits_				= std::char_traits<_Char_>,
	class _Allocator_			= std::allocator<_Char_>,
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _Storage_				= BasicStringStorage<_Char_, _SimdOptimization_, stringStorage::OptimizationSmallAndLarge>>
class BasicString 
{
	using BasicStdString = std::basic_string<
		_Char_, std::char_traits<_Char_>, std::allocator<_Char_>>;
public:
	inline static constexpr std::size_t npos = -1;

	using allocator_type	= _Allocator_;
	using value_type		= _Char_;

	using reference			= _Char_&;
	using const_reference	= const _Char_&;

	using pointer			= _Char_*;
	using const_pointer		= const _Char_*;

	using size_type			= sizetype;
	using difference_type	= ptrdiff;

	using iterator			= StringIterator<BasicString>;
	using const_iterator	= StringConstIterator<BasicString>;

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

	using traits_type = CharTraits<ValueType>;

	using traits = CharTraits<ValueType>;
	using Traits = traits;

	using Allocator = allocator_type;

	template <typename __Char>
	using if_compatible_char		= typename std::enable_if<
		IsCompatibleCharType<__Char>::value, bool>::type;

	template <typename _Pointer_>
	using if_compatible_pointer		= typename std::enable_if<
		IsCompatiblePointer<_Pointer_>::value, bool>::type;

	template <typename _Type_>
	using if_compatible_string_like	= typename std::enable_if<
		std::is_same<_Type_, BasicString>::value, bool>::type;

	template <class _Vectorization_>
	static inline constexpr bool IsVectorizationEnabled = std::is_same_v<
		_Vectorization_, stringSimd::OptimizationEnable>;
	/*template <typename _Type_>
	using if_compatible_container	= typename std::enable_if<
		IsContainerCompatibleWithStringView<_Type_>::value, bool>::type;*/

	template <typename __Char>
	inline static constexpr bool is_compatible_char_v			= IsCompatibleCharType<__Char>::value;

	template <typename _Pointer_>
	inline static constexpr bool is_compatible_pointer_v		= IsCompatiblePointer<_Pointer_>::value;

	template <typename _Type_>
	inline static constexpr bool is_compatible_string_like_v	= std::is_same<_Type_, BasicString>::value;

	//template <typename _Type_>
	//inline static constexpr bool is_compatible_container_v		= IsContainerCompatibleWithStringView<_Type_>::value;

	enum SplitBehaviorEnum : uchar {
		KeepEmptyParts = 0,
		SkipEmptyParts = 0x1
	};

	DECLARE_FLAGS_ENUM(SplitBehavior, SplitBehaviorEnum);

	// =======================================================================================
	//									 Constructors
	// =======================================================================================

	CONSTEXPR_CXX20 BasicString();
	CONSTEXPR_CXX20 ~BasicString();

	CONSTEXPR_CXX20 BasicString(const ValueType* chars);

	CONSTEXPR_CXX20 BasicString(
		const ValueType* chs, 
		SizeType length);

	CONSTEXPR_CXX20 BasicString(
		const ValueType* chs,
		SizeType length,
		Allocator allocator = Allocator());

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
		const __Char* last,
		typename std::enable_if<
			!std::is_same<__Char, value_type*>::value,
			const Allocator>::type& = Allocator());

	CONSTEXPR_CXX20 inline BasicString(std::initializer_list<ValueType> initializerList);

	CONSTEXPR_CXX20 BasicString(BasicString&& string);
	CONSTEXPR_CXX20 BasicString(const BasicString& string);
	
	template <
		typename _CharType_,
		class _AllocatorType_>
	CONSTEXPR_CXX20 BasicString(std::basic_string<_CharType_, std::char_traits<_CharType_>, _AllocatorType_>&& string);

	//template <
	//	typename _CharType_,
	//	class _AllocatorType_>
	//CONSTEXPR_CXX20 BasicString(const std::basic_string<_CharType_, std::char_traits<_CharType_>, _AllocatorType_>& stringg);

	// =======================================================================================
	//								 Assignment Operators
	// =======================================================================================

	NODISCARD ValueType& operator[](const SizeType index) noexcept;
	NODISCARD const ValueType& operator[](const SizeType index) const noexcept;

	CONSTEXPR_CXX20 BasicString& operator=(std::initializer_list<ValueType> initializerList);
	CONSTEXPR_CXX20 BasicString& operator+=(std::initializer_list<ValueType> initializerList);


	// =======================================================================================
	//							Accessors and Element Access
	// =======================================================================================

	NODISCARD ValueType at(const SizeType index) const noexcept;
	NODISCARD inline Reference at(const SizeType offset) noexcept;

	CONSTEXPR_CXX20 NODISCARD inline ValueType front() const noexcept;
	CONSTEXPR_CXX20 NODISCARD inline Reference front() noexcept;

	CONSTEXPR_CXX20 NODISCARD inline ValueType back() const noexcept;
	CONSTEXPR_CXX20 NODISCARD inline Reference back() noexcept;

	CONSTEXPR_CXX20 NODISCARD ValueType* data() noexcept;
	CONSTEXPR_CXX20 NODISCARD inline ConstPointer data() const noexcept;
	CONSTEXPR_CXX20 NODISCARD const ValueType* constData() const noexcept;

	CONSTEXPR_CXX20 NODISCARD inline ValueType& first();
	CONSTEXPR_CXX20 NODISCARD inline const ValueType& first() const noexcept;
	CONSTEXPR_CXX20 NODISCARD inline const ValueType& constFirst() const noexcept;

	CONSTEXPR_CXX20 NODISCARD inline ValueType& last();
	CONSTEXPR_CXX20 NODISCARD inline const ValueType& last() const noexcept;
	CONSTEXPR_CXX20 NODISCARD inline const ValueType& constLast() const noexcept;

	CONSTEXPR_CXX20 inline ConstPointer c_str() const noexcept;
	CONSTEXPR_CXX20 inline Pointer c_str() noexcept;


	// =======================================================================================
	//										Iterators
	// =======================================================================================

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


	// =======================================================================================
	//									Capacity and Size
	// =======================================================================================

	CONSTEXPR_CXX20 inline NODISCARD SizeType count() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType size() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType length() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD SizeType capacity() const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType unusedCapacity() const noexcept;
	
	CONSTEXPR_CXX20 NODISCARD bool isEmpty() const noexcept;
	CONSTEXPR_CXX20 NODISCARD bool isNull() const noexcept;

	// =======================================================================================
	//										Allocator
	// =======================================================================================


	CONSTEXPR_CXX20 inline NODISCARD allocator_type& getAllocator() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD const allocator_type& getAllocator() const noexcept;

	
	constexpr inline NODISCARD sizetype maxSize() const noexcept;

	// =======================================================================================
	//										Modifiers
	// =======================================================================================

	// increase capacity to newCapacity (without geometric growth)
	CONSTEXPR_CXX20 inline void reserve(BASE_GUARDOVERFLOW SizeType newCapacity);

	CONSTEXPR_CXX20 inline void shrinkToFit();

	void resize(SizeType size);
	void resize(
		SizeType size,
		ValueType fill);

	void clear();

	CONSTEXPR_CXX20 NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last);
	CONSTEXPR_CXX20 NODISCARD Iterator erase(ConstIterator it);

	// =======================================================================================
	//							Push/Pop and Appending/Prepending
	// =======================================================================================

	CONSTEXPR_CXX20 inline BasicString& append(std::initializer_list<ValueType> initializerList);
	CONSTEXPR_CXX20 inline BasicString& append(ValueType element);
	CONSTEXPR_CXX20 inline BasicString& append(BasicString&& other);

	CONSTEXPR_CXX20 inline BasicString& append(const BasicString& string);
	CONSTEXPR_CXX20 inline BasicString& append(
		const ValueType*	chars,
		SizeType			length);

	CONSTEXPR_CXX20 inline BasicString& prepend(std::initializer_list<ValueType> initializerList);
	CONSTEXPR_CXX20 inline BasicString& prepend(ValueType element);
	CONSTEXPR_CXX20 inline BasicString& prepend(BasicString&& other);

	CONSTEXPR_CXX20 inline BasicString& prepend(
		const ValueType*	chars,
		SizeType			length);
	CONSTEXPR_CXX20 inline BasicString& prepend(const BasicString& string);

	CONSTEXPR_CXX20 inline BasicString& pushFront(std::initializer_list<ValueType> initializerList);
	CONSTEXPR_CXX20 inline BasicString& pushFront(ValueType element);
	CONSTEXPR_CXX20 inline BasicString& pushFront(BasicString&& other);

	CONSTEXPR_CXX20 inline BasicString& pushFront(
		const ValueType*	chars,
		SizeType			length);
	CONSTEXPR_CXX20 inline BasicString& pushFront(const BasicString& string);

	CONSTEXPR_CXX20 inline BasicString& pushBack(std::initializer_list<ValueType> initializerList);
	CONSTEXPR_CXX20 inline BasicString& pushBack(ValueType element);
	CONSTEXPR_CXX20 inline BasicString& pushBack(BasicString&& other);

	CONSTEXPR_CXX20 inline BasicString& pushBack(
		const ValueType*	chars,
		SizeType			length);
	CONSTEXPR_CXX20 inline BasicString& pushBack(const BasicString& string);

	CONSTEXPR_CXX20 NODISCARD ValueType pop() noexcept;

	CONSTEXPR_CXX20 NODISCARD ValueType popBack() noexcept;
	CONSTEXPR_CXX20 NODISCARD ValueType popFront() noexcept;

	// =======================================================================================
	//										Swap
	// =======================================================================================

	CONSTEXPR_CXX20 inline void swap(BasicString& other);

	CONSTEXPR_CXX20 inline void swapAt(
		SizeType _First,
		SizeType _Last);
	CONSTEXPR_CXX20 inline void swapAt(
		Iterator _First,
		Iterator _Last);

	// =======================================================================================
	//									Type Conversion
	// =======================================================================================

	NODISCARD NativeString toNativeString() const noexcept;

	NODISCARD std::wstring toStdWString() const noexcept;
	NODISCARD std::string toStdString() const noexcept;

#if __cpp_lib_char8_t
	NODISCARD std::u8string toStdUTF8String() const noexcept;
#endif

	NODISCARD std::u16string toStdUTF16String() const noexcept;
	NODISCARD std::u32string toStdUTF32String() const noexcept;

	// =======================================================================================
	//				String Operations (Search, Find, Replace, Remove, etc.)
	// =======================================================================================

	CONSTEXPR_CXX20 inline void reverse() noexcept;

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


	CONSTEXPR_CXX20 inline NODISCARD bool contains(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool contains(const BasicString& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const BasicString& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const BasicString& subVector) const noexcept;

	CONSTEXPR_CXX20 inline void removeAll(const ValueType& element);

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

	// =======================================================================================
	//									STL compatibility
	// =======================================================================================

	BASE_FUNCTION_ALIAS_SPEC(CONSTEXPR_CXX20 inline, shrink_to_fit, shrinkToFit)

	BASE_FUNCTION_ALIAS_SPEC_QUAL(CONSTEXPR_CXX20 inline NODISCARD, get_allocator, noexcept, getAllocator)
	BASE_FUNCTION_ALIAS_SPEC_QUAL(CONSTEXPR_CXX20 inline NODISCARD, get_allocator, const noexcept, getAllocator)

	BASE_FUNCTION_ALIAS_SPEC_QUAL(constexpr inline NODISCARD, max_size, const noexcept, maxSize)

	BASE_FUNCTION_ALIAS_SPEC_QUAL(inline, find_first_of, const, findFirstOf)
	BASE_FUNCTION_ALIAS_SPEC_QUAL(inline, find_last_of, const, findLastOf)

	BASE_FUNCTION_ALIAS_SPEC_QUAL(inline, find_first_not_of, const, findFirstNotOf)
	BASE_FUNCTION_ALIAS_SPEC_QUAL(inline, find_last_not_of, const, findLastNotOf)

	BASE_FUNCTION_ALIAS_SPEC(CONSTEXPR_CXX20 inline, push_front, pushFront)
	BASE_FUNCTION_ALIAS_SPEC(CONSTEXPR_CXX20 inline, push_back, pushBack)

	BASE_FUNCTION_ALIAS_SPEC_QUAL(CONSTEXPR_CXX20 inline NODISCARD, pop_back, noexcept, popBack)
	BASE_FUNCTION_ALIAS_SPEC_QUAL(CONSTEXPR_CXX20 inline NODISCARD, pop_front, noexcept, popFront)
private:
	_Storage_ _storage;
};

// ================================================================================================================================================
//																Implementation
// ================================================================================================================================================


// =======================================================================================
//									 Constructors
// =======================================================================================

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::BasicString() {
	 
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::~BasicString() {

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::BasicString(const ValueType* chars) {

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::BasicString(
	const ValueType* chs,
	SizeType length,
	Allocator allocator)
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::BasicString(
	Iterator first,
	Iterator last)
{

}

__BASE_BS_TEMPLATE
template <typename __Char>
CONSTEXPR_CXX20 inline __BASE_BS::BasicString(
	const __Char*	string,
	size_type		length)
{
	static_assert(is_compatible_char_v<__Char>, "Not supported __Char type");
}

__BASE_BS_TEMPLATE
template <typename __Char>
CONSTEXPR_CXX20 inline __BASE_BS::BasicString(
	const __Char* first,
	const __Char* last,
	typename std::enable_if<
		!std::is_same<__Char, value_type*>::value,
		const Allocator>::type&
) :
	BasicString(first, last - first)
{
	static_assert(is_compatible_char_v<__Char>, "Not supported __Char type");
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS::BasicString(const BasicString& string):
	_storage(string._storage)
{}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS::BasicString(BasicString&& string) :
	_storage(std::move(string._storage))
{}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS::BasicString(std::initializer_list<ValueType> initializerList)
{
	assign(initializerList);
}

__BASE_BS_TEMPLATE
template <
	typename _CharType_,
	class _AllocatorType_>
CONSTEXPR_CXX20 __BASE_BS::BasicString(std::basic_string<_CharType_, std::char_traits<_CharType_>, _AllocatorType_>&& string) {
	// TODO - optimize move
	if constexpr (std::is_same_v<_CharType_, ValueType>) {
		_storage.initAny(string.data(), string.size());
		return;	
	}

	//return _storage.initAny(StringConverter<DefaultReplacementConversionMode>::convertString<
	//	std::basic_string<
	//		_CharType_,
	//		std::char_traits<_CharType_>,
	//		std::allocator<_CharType_>>,
	//		>(std::move(string));
		
}

//__BASE_BS_TEMPLATE
//template <
//	typename _CharType_,
//	class _AllocatorType_>
//CONSTEXPR_CXX20 __BASE_BS::BasicString(const std::basic_string<_CharType_, std::char_traits<_CharType_>, _AllocatorType_>& string) {
//
//}

// =======================================================================================
//								 Assignment Operators
// =======================================================================================


__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::ValueType& __BASE_BS::operator[](const SizeType index) noexcept 
{
	return at(index);
}

__BASE_BS_TEMPLATE
NODISCARD const __BASE_BS::ValueType&  __BASE_BS::operator[](const SizeType index) const noexcept
{
	return at(index);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS& __BASE_BS::operator=(std::initializer_list<ValueType> initializerList) 
{
	return assign(initializerList.begin(), initializerList.size());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS& __BASE_BS::operator+=(std::initializer_list<ValueType> initializerList) 
{
	return append(initializerList);
}


// =======================================================================================
//							Accessors and Element Access
// =======================================================================================

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::ValueType __BASE_BS::at(const SizeType index) const noexcept
{
	return at(index);
}

__BASE_BS_TEMPLATE
NODISCARD inline __BASE_BS::Reference __BASE_BS::at(const SizeType index) noexcept 
{
	DebugAssertLog(_storage.data() != nullptr, "base:string::BasicString::at: Data is nullptr");
	DebugAssertLog(index >= 0 && index < size(), "base::string::BasicString::at: Index out of range");

	return *(_storage.data() + index);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType* __BASE_BS::data() noexcept
{
	return _storage.data();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD const __BASE_BS::ValueType* __BASE_BS::constData() const noexcept 
{
	return _storage.data();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD const __BASE_BS::ValueType* __BASE_BS::data() const noexcept 
{
	return _storage.data();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS::ValueType& __BASE_BS::first() 
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(0);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline const __BASE_BS::ValueType& __BASE_BS::first() const noexcept
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(0);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline const __BASE_BS::ValueType& __BASE_BS::constFirst() const noexcept
{
	return first();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS::ValueType& __BASE_BS::last()
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(size() - 1);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline const __BASE_BS::ValueType& __BASE_BS::last() const noexcept
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(size() - 1);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline const __BASE_BS::ValueType& __BASE_BS::constLast() const noexcept 
{
	return last();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS::ConstPointer __BASE_BS::c_str() const noexcept
{
	return _storage.c_str();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS::Pointer __BASE_BS::c_str() noexcept
{
	return _storage.c_str();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType __BASE_BS::front() const noexcept
{
	return at(0);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType& __BASE_BS::front() noexcept
{
	return at(0);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType __BASE_BS::back() const noexcept
{
	return at(size() - 1);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType& __BASE_BS::back() noexcept
{
	return at(size() - 1);
}


// =======================================================================================
//										Iterators
// =======================================================================================


__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::Iterator __BASE_BS::begin() noexcept 
{
	return (StringIterator(this));
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstIterator __BASE_BS::begin() const noexcept
{
	return (StringConstIterator(this));
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstIterator __BASE_BS::cbegin() const noexcept
{
	return (StringConstIterator(this));
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstIterator __BASE_BS::constBegin() const noexcept
{
	return (StringConstIterator(this));
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::Iterator __BASE_BS::end() noexcept
{
	return (StringIterator(this) + size());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstIterator __BASE_BS::end() const noexcept
{
	return (StringConstIterator(this) + size());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstIterator __BASE_BS::cend() const
{
	return (StringConstIterator(this) + size());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstIterator 
	__BASE_BS::constEnd() const noexcept
{
	return (StringConstIterator(this) + size());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ReverseIterator __BASE_BS::rbegin() noexcept
{
	return ReverseIterator(begin());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ReverseIterator __BASE_BS::rend() noexcept 
{
	return ReverseIterator(end());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstReverseIterator __BASE_BS::rbegin() const noexcept 
{
	return ConstReverseIterator(begin());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstReverseIterator __BASE_BS::rend() const noexcept
{
	return ConstReverseIterator(end());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstReverseIterator __BASE_BS::crbegin() const noexcept
{
	return ConstReverseIterator(begin());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 __BASE_BS::ConstReverseIterator __BASE_BS::crend() const noexcept 
{
	return ConstReverseIterator(end());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::ReverseIterator __BASE_BS::reversedBegin() noexcept
{
	return ReverseIterator(begin());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::ReverseIterator __BASE_BS::reversedEnd() noexcept 
{
	return ReverseIterator(end());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::ConstReverseIterator __BASE_BS::reversedBegin() const noexcept
{
	return ConstReverseIterator(begin());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::ConstReverseIterator __BASE_BS::reversedEnd() const noexcept 
{
	return ConstReverseIterator(end());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::ConstReverseIterator __BASE_BS::constReversedBegin() const noexcept 
{
	return crbegin();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::ConstReverseIterator __BASE_BS::constReversedEnd() const noexcept 
{
	return crend();
}


// =======================================================================================
//									Capacity and Size
// =======================================================================================

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::SizeType __BASE_BS::size() const noexcept
{
	return _storage.size();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::SizeType __BASE_BS::length() const noexcept
{
	return _storage.size();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::SizeType __BASE_BS::capacity() const noexcept
{
	return _storage.capacity();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::SizeType __BASE_BS::count() const noexcept 
{
	return size();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::SizeType __BASE_BS::unusedCapacity() const noexcept
{
	return (capacity() - size());
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD bool __BASE_BS::isEmpty() const noexcept {
	return (size() == 0);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD bool __BASE_BS::isNull() const noexcept {
	return (capacity() == 0);
}


// =======================================================================================
//										Allocator
// =======================================================================================


__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD __BASE_BS::allocator_type& __BASE_BS::getAllocator() noexcept
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD const __BASE_BS::allocator_type& __BASE_BS::getAllocator() const noexcept 
{

}

__BASE_BS_TEMPLATE
constexpr inline NODISCARD sizetype __BASE_BS::maxSize() const noexcept {
	return static_cast<std::size_t>(-1) / sizeof(ValueType);
}


// =======================================================================================
//									Type Conversion
// =======================================================================================


__BASE_BS_TEMPLATE
// It is mainly used in Windows to avoid unnecessary code with a defined UNICODE 
NODISCARD NativeString __BASE_BS::toNativeString() const noexcept {
	if constexpr (std::is_same_v<NativeString, std::string>)
		return toStdString();
	else if constexpr (std::is_same_v<NativeString, std::wstring>)
		return toStdWString();

	AssertUnreachable();
}

__BASE_BS_TEMPLATE
NODISCARD std::wstring __BASE_BS::toStdWString() const noexcept {
	return StringConverter::convertString<BasicStdString, std::wstring>(data());
}

__BASE_BS_TEMPLATE
NODISCARD std::string __BASE_BS::toStdString() const noexcept {
	return StringConverter::convertString<BasicStdString, std::string>(data());
}

#if __cpp_lib_char8_t

__BASE_BS_TEMPLATE
NODISCARD std::u8string __BASE_BS::toStdUTF8String() const noexcept {
	return StringConverter::convertString<BasicStdString, std::u8string>(data());
}

#endif

__BASE_BS_TEMPLATE
NODISCARD std::u16string __BASE_BS::toStdUTF16String() const noexcept {
	return StringConverter::convertString<BasicStdString, std::u16string>(data());
}

__BASE_BS_TEMPLATE
NODISCARD std::u32string __BASE_BS::toStdUTF32String() const noexcept {
	return StringConverter::convertString<BasicStdString, std::u32string>(data());
}


// =======================================================================================
//										Modifiers
// =======================================================================================


__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline void __BASE_BS::reserve(SizeType newCapacity)
{
	_storage.reserve(newCapacity);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline void __BASE_BS::shrinkToFit() {
	if (capacity() < size() * 1.5)
		return;

	BasicString(cbegin(), cend()).swap(*this);
}


__BASE_BS_TEMPLATE
void __BASE_BS::resize(SizeType size) {
	_storage.expandNoInit(size, true);
}

__BASE_BS_TEMPLATE
void __BASE_BS::resize(
	SizeType size,
	ValueType fillChar)
{
	_storage.expandNoInit(size, true);
	fill(fillChar);
}

__BASE_BS_TEMPLATE
void __BASE_BS::clear() {
	if (_storage.category() == _Storage_::Category::isSmall)
		return;

	_storage.destroyMediumLarge();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::Iterator __BASE_BS::erase(
	ConstIterator first,
	ConstIterator last)
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::Iterator
	__BASE_BS::erase(ConstIterator it) 
{

}


// =======================================================================================
//							Push/Pop and Appending/Prepending
// =======================================================================================

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS& __BASE_BS::pushFront(ValueType element)
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS& __BASE_BS::pushFront(std::initializer_list<ValueType> initializerList)
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS& __BASE_BS::pushFront(BasicString&& other)
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS& __BASE_BS::pushBack(ValueType element) 
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS& __BASE_BS::pushBack(std::initializer_list<ValueType> initializerList)
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline __BASE_BS& __BASE_BS::pushBack(BasicString&& other) 
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType __BASE_BS::pop() noexcept
{
	return popBack();
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType __BASE_BS::popBack() noexcept 
{
	DebugAssertLog(isEmpty() == false, "base::string::BasicString::popBack: Data is empty");

	const auto lastPosition = size() - 1;

	const auto temp = at(lastPosition);
	erase(lastPosition, lastPosition);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 NODISCARD __BASE_BS::ValueType __BASE_BS::popFront() noexcept
{
	DebugAssertLog(isEmpty() == false, "base::string::BasicString::popFront: Data is empty");

	const auto temp = at(0);
	erase(0, 0);

	return temp;
}


// =======================================================================================
//										Swap
// =======================================================================================


__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline void __BASE_BS::swap(BasicString& other) {
	_storage.swap(other._storage);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline void __BASE_BS::swapAt(
	SizeType firstPosition,
	SizeType secondPosition)
{
	DebugAssertLog(firstPosition >= 0 && firstPosition < size(), "base::string::BasicString::swap: Index out of range");
	DebugAssertLog(secondPosition >= 0 && secondPosition < size(), "base::string::BasicString::swap: Index out of range");

	const auto temp			= at(firstPosition);

	(*this)[firstPosition]	= at(secondPosition);
	(*this)[secondPosition] = temp;
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline void __BASE_BS::swapAt(
	Iterator firstIterator,
	Iterator secondIterator)
{
#ifdef _DEBUG
	const auto myDataStart = data();

	const auto firstPointer = firstIterator._currentChar;
	const auto secondPointer = secondIterator._currentChar;

	DebugAssertLog(firstPointer >= myDataStart && firstPointer < myDataStart + size(), "base::string::BasicString::swap: Index out of range");
	DebugAssertLog(secondPointer >= myDataStart && secondPointer < myDataStart + size(), "base::string::BasicString::swap: Index out of range");
#endif

	const auto temp = *firstIterator;

	*firstIterator	= *secondIterator;
	*secondIterator = temp;
}


// =======================================================================================
//				String Operations (Search, Find, Replace, Remove, etc.)
// =======================================================================================

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::indexOf(
	ValueType ch,
	SizeType from,
	CaseSensitivity cs) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::indexOf(
	const BasicString& string,
	SizeType from,
	CaseSensitivity cs) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::lastIndexOf(
	ValueType c,
	CaseSensitivity cs) const noexcept
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::lastIndexOf(
	ValueType c,
	SizeType from,
	CaseSensitivity cs) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::lastIndexOf(
	const BasicString& string,
	CaseSensitivity cs) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::lastIndexOf(
	const BasicString& string,
	SizeType from,
	CaseSensitivity cs) const
{

}

__BASE_BS_TEMPLATE
NODISCARD bool __BASE_BS::contains(
	ValueType ch,
	CaseSensitivity caseSensitivity) const
{

}

__BASE_BS_TEMPLATE
NODISCARD bool __BASE_BS::contains(
	const BasicString& string,
	CaseSensitivity caseSensitivity) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::count(
	ValueType ch,
	CaseSensitivity caseSensitivity) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::SizeType __BASE_BS::count(
	const BasicString& string,
	CaseSensitivity caseSensitivity) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS& __BASE_BS::fill(
	ValueType ch,
	SizeType size)
{	

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::insert(
	SizeType index,
	ValueType ch)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::insert(
	SizeType			index,
	const ValueType*	ch,
	SizeType			length)
{

}

__BASE_BS_TEMPLATE
__BASE_BS&  __BASE_BS::insert(
	SizeType			index,
	const BasicString&	string)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::remove(
	SizeType index,
	SizeType length)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::remove(
	ValueType ch,
	CaseSensitivity caseSensitivity)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::remove(
	const BasicString& string,
	CaseSensitivity caseSensitivity)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::removeAt(SizeType pos)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::removeFirst() 
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::removeLast()
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::replace(
	SizeType index,
	SizeType length,
	ValueType after)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::replace(
	SizeType index,
	SizeType length,
	const ValueType* ch,
	SizeType slen)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::replace(
	SizeType index,
	SizeType length,
	const BasicString& after)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::replace(
	ValueType before,
	ValueType after,
	CaseSensitivity caseSensitivity)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::replace(
	const ValueType* before,
	SizeType beforeLength,
	const ValueType* after,
	SizeType afterLength,
	CaseSensitivity caseSensitivity)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::replace(
	const BasicString& before,
	const BasicString& after,
	CaseSensitivity caseSensitivity)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::replace(
	ValueType ch,
	const BasicString& after,
	CaseSensitivity caseSensitivity)
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::StringList __BASE_BS::split(
	const BasicString& sep,
	SplitBehavior behavior,
	CaseSensitivity cs) const
{

}

__BASE_BS_TEMPLATE
NODISCARD __BASE_BS::StringList __BASE_BS::split(
	ValueType sep,
	SplitBehavior behavior,
	CaseSensitivity caseSensibity) const
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline void __BASE_BS::reverse() noexcept {

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD bool __BASE_BS::contains(const ValueType& element) const noexcept
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD bool __BASE_BS::
	contains(const BasicString& subString) const noexcept
{

}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD bool __BASE_BS::startsWith(const ValueType& element) const noexcept
{
	return (at(0) == element);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD bool __BASE_BS::startsWith(const BasicString& subString) const noexcept
{
	const auto subStringSize = subString.size();

	if (size() < subStringSize)
		return false;

	for (SizeType current = 0; current < subStringSize; ++current)
		if (at(current) != subString.at(current))
			return false;

	return true;
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD bool __BASE_BS::endsWith(const ValueType& element) const noexcept 
{
	return (at(size() - 1) == element);
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline NODISCARD bool __BASE_BS::endsWith(const BasicString& subString) const noexcept
{
	const auto subStringSize	= subString.size();
	const auto mySize			= size();

	if (mySize < subStringSize)
		return false;

	for (SizeType current = 0; current < subStringSize; ++current)
		if (at(mySize - subStringSize + current) != subString.at(current))
			return false;

	return true;
}

__BASE_BS_TEMPLATE
CONSTEXPR_CXX20 inline void __BASE_BS::removeAll(const ValueType& element) {

}


__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::assign(const BasicString& string) 
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::assign(BasicString&& string)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::assign(
	const BasicString&	string,
	const size_type		position,
	size_type			length)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::assign(
	const ValueType* string,
	const size_type length)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::assign(const ValueType* string) 
{
	
}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::assign(std::initializer_list<ValueType> initializerList)
{
	return assign(initializerList.data(), initializerList.size());
}

__BASE_BS_TEMPLATE
template <
	class _IteratorOrLength_,
	class _IteratorOrChar_>
__BASE_BS& __BASE_BS::assign(
	_IteratorOrLength_	firstOrLength,
	_IteratorOrChar_	lastOrChar)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::insert(
	SizeType				firstPosition,
	const BasicString&		string,
	SizeType				secondPosition,
	SizeType				length)
{

}

__BASE_BS_TEMPLATE
__BASE_BS& __BASE_BS::insert(
	SizeType	position,
	SizeType	length,
	ValueType	ch)
{

}

__BASE_BS_TEMPLATE
__BASE_BS::iterator __BASE_BS::insert(
	ConstIterator	where,
	const ValueType ch)
{

}

__BASE_BS_TEMPLATE
NODISCARD container::Vector<typename __BASE_BS::SizeType>
	__BASE_BS::findAll(const BasicString& string) const noexcept 
{

}

__BASE_BS_TEMPLATE
NODISCARD container::Vector<typename __BASE_BS::SizeType> 
	__BASE_BS::findAll(ValueType ch) const noexcept
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstOf(
	const BasicString&	string,
	SizeType			position) const
{
	//if constexpr (is_vectorization_enabled) {
	//	if (string.size() == 1) {
	//		return FindVectorized(
	//			data() + position, data() + size(),
	//			string.at(0));
	//	}

	//	return FindSubRangeVectorized(
	//		data() + position, data() + size(),
	//		string.data(), string.size());
	//}

	//wcslen()
}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstOf(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstOf(
	const ValueType*	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstOf(
	ValueType	ch,
	SizeType	position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastOf(
	const BasicString&	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastOf(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastOf(
	const ValueType*	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastOf(
	ValueType	ch,
	SizeType	position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstNotOf(
	const BasicString&	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstNotOf(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstNotOf(
	const ValueType*	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findFirstNotOf(
	ValueType	ch,
	SizeType	position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastNotOf(
	const BasicString&	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastNotOf(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastNotOf(
	const ValueType*	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::findLastNotOf(
	ValueType	ch,
	SizeType	position) const
{	

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::find(
	const BasicString&	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::find(
	const ValueType*	needle,
	SizeType			position,
	SizeType			size) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::find(
	const ValueType*	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::find(
	ValueType	ch,
	SizeType	position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::rfind(
	const BasicString&	string,
	SizeType			position) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::rfind(
	const ValueType*	string,
	SizeType			position,
	SizeType			length) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::rfind(
	const ValueType*	string,
	SizeType			pos) const
{

}

__BASE_BS_TEMPLATE
__BASE_BS::SizeType __BASE_BS::rfind(
	ValueType	ch,
	SizeType	position) const
{
	
}


// =======================================================================================
//									STL compatibility
// =======================================================================================


__BASE_BS_TEMPLATE
__BASE_BS 
	__BASE_BS::substr(
		SizeType position,
		SizeType length) const&
{

}

__BASE_BS_TEMPLATE
__BASE_BS 
	__BASE_BS::substr(
		SizeType position,
		SizeType length)&&
{

}

__BASE_BS_TEMPLATE
int __BASE_BS::compare(const BasicString& string) const {

}

__BASE_BS_TEMPLATE
int __BASE_BS::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const BasicString&	string) const
{

}

__BASE_BS_TEMPLATE
int __BASE_BS::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const ValueType*	string) const
{

}

__BASE_BS_TEMPLATE
int __BASE_BS::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const ValueType*	string,
	SizeType			secondLength) const
{

}

__BASE_BS_TEMPLATE
int __BASE_BS::compare(
	SizeType			firstPosition,
	SizeType			firstLength,
	const BasicString&	string,
	SizeType			secondPosition,
	SizeType			secondLength) const
{

}

__BASE_BS_TEMPLATE
int __BASE_BS::compare(const ValueType* string) const {

}

// =======================================================================================
//										Operators
// =======================================================================================

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	const __BASE_BS& lhs,
	const __BASE_BS& rhs)
{
	__BASE_BS result;

	result.reserve(lhs.size() + rhs.size());
	result.append(lhs).append(rhs);

	return result;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	__BASE_BS&&			lhs,
	const __BASE_BS&	rhs) 
{
	return std::move(lhs.append(rhs));
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	const __BASE_BS&	lhs,
	__BASE_BS&&			rhs) 
{
	if (rhs.capacity() >= lhs.size() + rhs.size()) {
		// Good, at least we don't need to reallocate
		return std::move(rhs.insert(0, lhs));
	}

	// Meh, no go. Forward to operator+(const&, const&).
	auto const& rhsC = rhs;
	return lhs + rhsC;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	__BASE_BS&& lhs,
	__BASE_BS&& rhs)
{
	return std::move(lhs.append(rhs));
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	const _Char_*																		lhs, 
	const __BASE_BS&	rhs)
{
	__BASE_BS result;

	const auto len = __BASE_BS::traits_type::length(lhs);

	result.reserve(len + rhs.size());
	result.append(lhs, len).append(rhs);

	return result;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	const _Char_*																lhs,
	__BASE_BS&& rhs) 
{
	const auto len = __BASE_BS::traits_type::length(lhs);

	if (rhs.capacity() >= len + rhs.size()) {
		rhs.insert(rhs.begin(), lhs, lhs + len);
		return std::move(rhs);
	}

	__BASE_BS result;

	result.reserve(len + rhs.size());
	result.append(lhs, len).append(rhs);

	return result;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	_Char_																				lhs,
	const __BASE_BS&	rhs) 
{
	__BASE_BS result;

	result.reserve(1 + rhs.size());
	result.push_back(lhs);

	result.append(rhs);

	return result;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	_Char_																		lhs,
	__BASE_BS&& rhs) 
{
	if (rhs.capacity() > rhs.size()) {
		rhs.insert(rhs.begin(), lhs);
		return std::move(rhs);
	}

	// Meh, no go. Forward to operator+(E, const&).
	auto const& rhsC = rhs;
	return lhs + rhsC;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	const __BASE_BS&	lhs,
	const _Char_*																		rhs) 
{
	using size_type = typename __BASE_BS::size_type;
	using traits_type = typename __BASE_BS::traits_type;

	__BASE_BS result;

	const size_type len = traits_type::length(rhs);

	result.reserve(lhs.size() + len);
	result.append(lhs).append(rhs, len);

	return result;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	__BASE_BS&& lhs,
	const _Char_*																rhs)
{
	return std::move(lhs += rhs);
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	const __BASE_BS&	lhs,
	_Char_																				rhs) 
{
	__BASE_BS result;

	result.reserve(lhs.size() + 1);
	result.append(lhs);

	result.push_back(rhs);

	return result;
}

__BASE_BS_TEMPLATE
inline __BASE_BS operator+(
	__BASE_BS&& lhs,
	_Char_ rhs)
{
  return std::move(lhs += rhs);
}

__BASE_BS_TEMPLATE
inline bool operator==(
    const __BASE_BS& lhs,
    const __BASE_BS& rhs) 
{
	return lhs.size() == rhs.size() 
		&& lhs.compare(rhs) == 0;
}

__BASE_BS_TEMPLATE
inline bool operator==(
	std::nullptr_t, 
	const __BASE_BS&) = delete;

__BASE_BS_TEMPLATE
inline bool operator==(
    const typename __BASE_BS::value_type*	lhs,
    const __BASE_BS&						rhs) 
{
	return rhs == lhs;
}

__BASE_BS_TEMPLATE
inline bool operator==(
	const __BASE_BS&,
	std::nullptr_t) = delete;

__BASE_BS_TEMPLATE
inline bool operator==(
    const __BASE_BS&						lhs,
    const typename __BASE_BS::value_type*	rhs) 
{
	return lhs.compare(rhs) == 0;
}

__BASE_BS_TEMPLATE
inline bool operator!=(
    const __BASE_BS& lhs,
    const __BASE_BS& rhs) 
{
	return !(lhs == rhs);
}

__BASE_BS_TEMPLATE
inline bool operator!=(
	std::nullptr_t, 
	__BASE_BS&) = delete;

__BASE_BS_TEMPLATE
inline bool operator!=(
    const typename __BASE_BS::value_type*	lhs,
    const __BASE_BS&						rhs) 
{
	return !(lhs == rhs);
}

__BASE_BS_TEMPLATE
inline bool operator!=(
	const __BASE_BS&, 
	std::nullptr_t) = delete;

__BASE_BS_TEMPLATE
inline bool operator!=(
    const __BASE_BS&						lhs,
    const typename __BASE_BS::value_type*	rhs) 
{
	return !(lhs == rhs);
}

__BASE_BS_TEMPLATE
inline bool operator<(
    const __BASE_BS& lhs,
    const __BASE_BS& rhs)
{
	return lhs.compare(rhs) < 0;
}

__BASE_BS_TEMPLATE
inline bool operator<(
	const __BASE_BS&,
	std::nullptr_t) = delete;

__BASE_BS_TEMPLATE
inline bool operator<(
    const __BASE_BS&						lhs,
    const typename __BASE_BS::value_type*	rhs) 
{
	return lhs.compare(rhs) < 0;
}

__BASE_BS_TEMPLATE
inline bool operator<(
	std::nullptr_t, 
	const __BASE_BS&) = delete;

__BASE_BS_TEMPLATE
inline bool operator<(
    const typename __BASE_BS::value_type*	lhs,
    const __BASE_BS&						rhs) 
{
	return rhs.compare(lhs) > 0;
}

__BASE_BS_TEMPLATE
inline bool operator>(
    const __BASE_BS& lhs,
    const __BASE_BS& rhs) {
  return rhs < lhs;
}

__BASE_BS_TEMPLATE
inline bool operator>(
	const __BASE_BS&,
	std::nullptr_t) = delete;

__BASE_BS_TEMPLATE
inline bool operator>(
    const __BASE_BS&						lhs,
    const typename __BASE_BS::value_type*	rhs)
{
	return rhs < lhs;
}

__BASE_BS_TEMPLATE
inline bool operator>(
	std::nullptr_t, 
	const __BASE_BS&) = delete;

__BASE_BS_TEMPLATE
inline bool operator>(
    const typename __BASE_BS::value_type*	lhs,
    const __BASE_BS&						rhs)
{
	return rhs < lhs;
}

__BASE_BS_TEMPLATE
inline bool operator<=(
    const __BASE_BS& lhs,
    const __BASE_BS& rhs)
{
	return !(rhs < lhs);
}

__BASE_BS_TEMPLATE
inline bool operator<=(
	const __BASE_BS&,
	std::nullptr_t) = delete;

__BASE_BS_TEMPLATE
inline bool operator<=(
    const __BASE_BS&						lhs,
    const typename __BASE_BS::value_type*	rhs) {
  return !(rhs < lhs);
}

__BASE_BS_TEMPLATE
inline bool operator<=(
	std::nullptr_t, 
	__BASE_BS&) = delete;

__BASE_BS_TEMPLATE
inline bool operator<=(
    const typename __BASE_BS::value_type*	lhs,
    const __BASE_BS&						rhs)
{
	return !(rhs < lhs);
}

__BASE_BS_TEMPLATE
inline bool operator>=(
    const __BASE_BS& lhs,
    const __BASE_BS& rhs)
{
	return !(lhs < rhs);
}

__BASE_BS_TEMPLATE
inline bool operator>=(
	const __BASE_BS&, 
	std::nullptr_t) = delete;

__BASE_BS_TEMPLATE
inline bool operator>=(
    const __BASE_BS&						lhs,
    const typename __BASE_BS::value_type*	rhs) 
{
	return !(lhs < rhs);
}

__BASE_BS_TEMPLATE
inline bool operator>=(
	std::nullptr_t,
	const __BASE_BS&) = delete;

__BASE_BS_TEMPLATE
inline bool operator>=(
    const typename __BASE_BS::value_type*	lhs,
    const __BASE_BS&						rhs) 
{
	return !(lhs < rhs);
}

#if BASE_HAS_CXX20
__BASE_BS_TEMPLATE
inline bool operator<=>(
	std::nullptr_t,
	const __BASE_BS&) = delete;

__BASE_BS_TEMPLATE
inline bool operator<=>(
	const __BASE_BS&,
	std::nullptr_t) = delete;
#endif

__BASE_BS_TEMPLATE
void swap(
	__BASE_BS& lhs,
	__BASE_BS& rhs) 
{
	lhs.swap(rhs);
}

__BASE_BS_TEMPLATE
inline std::basic_istream<
    typename __BASE_BS::value_type,
    typename __BASE_BS::traits_type>&
operator>>(
    std::basic_istream<
        typename __BASE_BS::value_type,
        typename __BASE_BS::traits_type>& is,
	__BASE_BS& str) 
{
	using _InputStream_ = std::basic_istream<
      typename __BASE_BS::value_type,
      typename __BASE_BS::traits_type>;

	using _SEntry_ = typename _InputStream_::sentry;
	using _IOState_ = typename _InputStream_::iostate;

	_SEntry_ sentry(is);
	size_t extracted = 0;

	_IOState_ err = _InputStream_::goodbit;

	if (sentry) {
		auto n = is.width();

		if (n <= 0)
			n = str.max_size();

		str.erase();

		for (auto got = is.rdbuf()->sgetc(); extracted != size_t(n); ++extracted) {
			if (got == _Traits_::eof()) {
				err |= _InputStream_::eofbit;
				is.width(0);

				break;
			}

			if (isspace(got))
				break;
			
			str.push_back(got);
			got = is.rdbuf()->snextc();
		}
	}

	if (!extracted)
		err |= _InputStream_::failbit;

	if (err)
		is.setstate(err);

	return is;
}

__BASE_BS_TEMPLATE

inline std::basic_ostream<
    typename __BASE_BS::value_type,
    typename __BASE_BS::traits_type>&
operator<<(
    std::basic_ostream<
        typename __BASE_BS::value_type,
        typename __BASE_BS::traits_type>& os,
    const __BASE_BS& str) 
{
#if defined(CPP_GNU)
	using _OutputStream_ = std::basic_ostream<
      typename __BASE_BS::value_type,
      typename __BASE_BS::traits_type>;

	using _SEntry_ = _OutputStream_::sentry;
	using _Ip_ = _OutputStream_::std::ostreambuf_iterator<
		typename __BASE_BS::value_type,
		typename __BASE_BS::traits_type>

	_SEntry_ _s(os);
	if (_s) {
		size_t __len = str.size();
		bool __left =
			(os.flags() & _OutputStream_::adjustfield) == _OutputStream_::left;

	if (__pad_and_output(
			_Ip(os), str.data(),
			__left ? str.data() + __len : str.data(),
			str.data() + __len, os, os.fill()) .failed()) 
	{
		os.setstate(_ostream_type::badbit | _ostream_type::failbit);
	}
}
#elif defined(CPP_MSVC)
	using streamsize = decltype(os.precision());
	// MSVC doesn't define __ostream_insert
	os.write(str.data(), static_cast<streamsize>(str.size()));
#else
	std::__ostream_insert(os, str.data(), str.size());
#endif

	return os;
}

// std::basic_string compatibility

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator==(
    const __BASE_BS&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs) 
{
	return (lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) == 0);
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator==(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const __BASE_BS&	rhs)
{
	return rhs == lhs;
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator!=(
    const __BASE_BS&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return !(lhs == rhs);
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator!=(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>& lhs,
    const __BASE_BS& rhs)
{
	return !(lhs == rhs);
}

template <
	class _Char_, class _Traits_, class _Allocator_, 
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator<(
    const __BASE_BS&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return (lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) < 0);
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator>(
    const __BASE_BS&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return (lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) > 0);
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator<(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const __BASE_BS&	rhs) 
{
	return rhs > lhs;
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator>(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const __BASE_BS&	rhs) 
{
	return rhs < lhs;
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator<=(
    const __BASE_BS&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return !(lhs > rhs);
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator>=(
    const __BASE_BS&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs) 
{	
	return !(lhs < rhs);
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator<=(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const __BASE_BS&	rhs) 
{
	return !(lhs > rhs);
}

template <
	class _Char_, class _Traits_, class _Allocator_,
	class _SimdOptimization_, class _Storage_, class _SecondAllocator_>
inline bool operator>=(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const __BASE_BS&	rhs)
{
	return !(lhs < rhs);
}

__BASE_STRING_NAMESPACE_END
