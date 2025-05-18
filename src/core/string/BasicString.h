#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/string/StringIterator.h>
#include <src/core/string/CharTypeCompatibility.h>

#include <src/core/string/StringDebug.h>
#include <base/core/container/Vector.h>

#include <src/core/string/BasicStringStorage.h>
#include <src/core/string/CharTraits.h>

#include <src/core/utility/simd/SimdAlgorithm.h>


WARNING_DISABLE_MSVC(4834)
WARNING_DISABLE_MSVC(4002)
WARNING_DISABLE_MSVC(4003)


__BASE_STRING_NAMESPACE_BEGIN

template <
	class _Char_,
	class _Traits_				= std::char_traits<_Char_>,
	class _Allocator_			= std::allocator<_Char_>,
	class _SimdOptimization_	= stringSimd::OptimizationDisable,
	class _Storage_				= BasicStringStorage<_Char_, _SimdOptimization_, stringStorage::OptimizationSmallAndLarge>>
class BasicString 
{
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

	NODISCARD std::u8string toStdUTF8String() const noexcept;
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

	CONSTEXPR_CXX20 inline void shrink_to_fit();

	CONSTEXPR_CXX20 inline NODISCARD allocator_type& get_allocator() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD const allocator_type& get_allocator() const noexcept;

	constexpr inline NODISCARD sizetype max_size() const noexcept;

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

	CONSTEXPR_CXX20 inline BasicString& push_front(std::initializer_list<ValueType> initializerList);
	CONSTEXPR_CXX20 inline BasicString& push_front(ValueType element);
	CONSTEXPR_CXX20 inline BasicString& push_front(BasicString&& other);

	CONSTEXPR_CXX20 inline BasicString& push_back(std::initializer_list<ValueType> initializerList);
	CONSTEXPR_CXX20 inline BasicString& push_back(ValueType element);
	CONSTEXPR_CXX20 inline BasicString& push_back(BasicString&& other);

	CONSTEXPR_CXX20 NODISCARD ValueType pop_back() noexcept;
	CONSTEXPR_CXX20 NODISCARD ValueType pop_front() noexcept;
private:
	static constexpr NODISCARD SizeType calculateGrowth(SizeType newSize) noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool resizeReallocate(SizeType newCapacity) noexcept;
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceBack(ValueType value);

	CONSTEXPR_CXX20 inline void emplaceBackWithUnusedCapacity(ValueType value);
	CONSTEXPR_CXX20 inline void emplaceBackReallocate(ValueType value);
	
	
	CONSTEXPR_CXX20 inline void emplaceAt(
		pointer&	location,
		ValueType	value);

	_Storage_ _storage;
};

// ================================================================================================================================================
//																Implementation
// ================================================================================================================================================


// =======================================================================================
//									 Constructors
// =======================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString() {
	 
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::~BasicString() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(const BasicString& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(const ValueType* chars) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(
	const ValueType* chs,
	SizeType length,
	Allocator allocator)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(
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
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(
	const __Char*	string,
	size_type		length)
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
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(
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

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::BasicString(std::initializer_list<ValueType> initializerList)
{
	assign(initializerList);
}


#if __cpp_lib_char8_t

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(std::u8string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(const std::u8string& string) {

}

#endif

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(BasicString&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(std::string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(const std::string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(std::wstring&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(const std::wstring& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(std::u16string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(const std::u16string& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(std::u32string&& string) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::BasicString(const std::u32string& string) {

}


// =======================================================================================
//								 Assignment Operators
// =======================================================================================


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::operator[](const SizeType index) noexcept 
{
	return at(index);
}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::operator[](const SizeType index) const noexcept
{
	return at(index);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::operator=(std::initializer_list<ValueType> initializerList) 
{
	return assign(initializerList.begin(), initializerList.size());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::operator+=(std::initializer_list<ValueType> initializerList) 
{
	return append(initializerList);
}


// =======================================================================================
//							Accessors and Element Access
// =======================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::at(const SizeType index) const noexcept
{
	return at(index);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Reference 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::at(const SizeType index) noexcept 
{
	DebugAssertLog(_storage.data() != nullptr, "base:string::BasicString::at: Data is nullptr");
	DebugAssertLog(index >= 0 && index < size(), "base::string::BasicString::at: Index out of range");

	return *(_storage.data() + index);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::data() noexcept
{
	return _storage.data();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::constData() const noexcept 
{
	return _storage.data();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType*
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::data() const noexcept 
{
	return _storage.data();
}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::first() 
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(0);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::first() const noexcept
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(0);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::constFirst() const noexcept
{
	return first();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::last()
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(size() - 1);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::last() const noexcept
{
	DebugAssertLog(size() > 0, "base::string::BasicString::first: String is empty");
	return at(size() - 1);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::constLast() const noexcept 
{
	return last();
}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstPointer 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::c_str() const noexcept
{
	return _storage.c_str();
}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Pointer
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::c_str() noexcept
{
	return _storage.c_str();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::front() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::front() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::back() const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::back() noexcept
{

}


// =======================================================================================
//										Iterators
// =======================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::begin() noexcept 
{
	return (StringIterator(this));
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::begin() const noexcept
{
	return (StringConstIterator(this));
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::cbegin() const noexcept
{
	return (StringConstIterator(this));
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstIterator
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::constBegin() const noexcept
{
	return (StringConstIterator(this));
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Iterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::end() noexcept
{
	return (StringIterator(this) + size());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::end() const noexcept
{
	return (StringConstIterator(this) + size());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::cend() const
{
	return (StringConstIterator(this) + size());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::constEnd() const noexcept
{
	return (StringConstIterator(this) + size());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rbegin() noexcept
{
	return ReverseIterator(begin());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rend() noexcept 
{
	return ReverseIterator(end());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rbegin() const noexcept 
{
	return ConstReverseIterator(begin());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rend() const noexcept
{
	return ConstReverseIterator(end());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::crbegin() const noexcept
{
	return ConstReverseIterator(begin());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::crend() const noexcept 
{
	return ConstReverseIterator(end());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::reversedBegin() noexcept
{
	return ReverseIterator(begin());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::reversedEnd() noexcept 
{
	return ReverseIterator(end());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::reversedBegin() const noexcept
{
	return ConstReverseIterator(begin());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::reversedEnd() const noexcept 
{
	return ConstReverseIterator(end());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::constReversedBegin() const noexcept 
{
	return crbegin();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ConstReverseIterator
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::constReversedEnd() const noexcept 
{
	return crend();
}


// =======================================================================================
//									Capacity and Size
// =======================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::size() const noexcept
{
	return _storage.size();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::length() const noexcept
{
	return size();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::capacity() const noexcept
{
	return _storage.capacity();
}
template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::count() const noexcept 
{
	return size();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::unusedCapacity() const noexcept
{
	return (capacity() - size());
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::isEmpty() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::isNull() const noexcept {

}


// =======================================================================================
//										Allocator
// =======================================================================================


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::getAllocator() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::getAllocator() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
constexpr inline NODISCARD sizetype BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::maxSize() const noexcept {
	return static_cast<std::size_t>(-1) / sizeof(ValueType);
}


// =======================================================================================
//									Type Conversion
// =======================================================================================


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD NativeString BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::toNativeString() const noexcept {
	if constexpr (std::is_same_v<NativeString, std::string>)
		return toStdString();
	else if constexpr (std::is_same_v<NativeString, std::wstring>)
		return toStdWString();

	AssertUnreachable();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::wstring BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::toStdWString() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::string BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::toStdString() const noexcept {
	return "";
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::u8string BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::toStdUTF8String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::u16string BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::toStdUTF16String() const noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD std::u32string BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::toStdUTF32String() const noexcept {

}


// =======================================================================================
//										Modifiers
// =======================================================================================


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::reserve(BASE_GUARDOVERFLOW SizeType newCapacity)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::shrinkToFit() {

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::resize(SizeType size) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::resize(
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
void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::clear() {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Iterator
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::erase(
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
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Iterator
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::erase(ConstIterator it) 
{

}


// =======================================================================================
//							Push/Pop and Appending/Prepending
// =======================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::push_back(ValueType element)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
		::push_back(std::initializer_list<ValueType> initializerList)
{
	return append(initializerList);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::push_back(BasicString&& other) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::push_front(ValueType element) 
{

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
		::push_front(std::initializer_list<ValueType> initializerList)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::push_front(BasicString&& other) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::pushFront(ValueType element)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
		::pushFront(std::initializer_list<ValueType> initializerList)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::pushFront(BasicString&& other)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::pushBack(ValueType element) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
		::pushBack(std::initializer_list<ValueType> initializerList)
{

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::pushBack(BasicString&& other) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::pop() noexcept
{
	return popBack();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::popBack() noexcept 
{
	DebugAssertLog(isEmpty() == false, "base::string::BasicString::popBack: Data is empty");

	const auto lastPosition = size() - 1;

	const auto temp = at(lastPosition);
	erase(lastPosition, lastPosition);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::pop_back() noexcept 
{
	return popBack();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::pop_front() noexcept 
{
	return popFront();
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::ValueType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::popFront() noexcept
{
	DebugAssertLog(isEmpty() == false, "base::string::BasicString::popFront: Data is empty");

	const auto temp = at(0);
	erase(0, 0);

	return temp;
}


// =======================================================================================
//										Swap
// =======================================================================================


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::swap(BasicString& other) {
	_storage.swap(other._storage);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::swapAt(
	SizeType firstPosition,
	SizeType secondPosition)
{
	DebugAssertLog(firstPosition >= 0 && firstPosition < size(), "base::string::BasicString::swap: Index out of range");
	DebugAssertLog(secondPosition >= 0 && secondPosition < size(), "base::string::BasicString::swap: Index out of range");

	const auto temp			= at(firstPosition);

	(*this)[firstPosition]	= at(secondPosition);
	(*this)[secondPosition] = temp;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::swapAt(
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

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::indexOf(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::indexOf(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::lastIndexOf(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::lastIndexOf(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::lastIndexOf(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::lastIndexOf(
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
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::contains(
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
NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::contains(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::count(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::count(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::fill(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::insert(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::insert(
		SizeType			index,
		const ValueType*	ch,
		SizeType			length)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::insert(
		SizeType			index,
		const BasicString&	string)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::remove(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::remove(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::remove(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::removeAt(SizeType pos)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::removeFirst() 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::removeLast()
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::replace(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::replace(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::replace(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::replace(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::replace(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::replace(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::replace(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::StringList 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::split(
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
NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::StringList 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::split(
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
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::reverse() noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::contains(const ValueType& element) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::
	contains(const BasicString& subString) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::startsWith(const ValueType& element) const noexcept
{
	return (at(0) == element);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::startsWith(const BasicString& subString) const noexcept
{
	const auto subStringSize = subString.size();

	if (size() < subStringSize)
		return false;

	for (SizeType current = 0; current < subStringSize; ++current)
		if (at(current) != subString.at(current))
			return false;

	return true;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::endsWith(const ValueType& element) const noexcept 
{
	return (at(size() - 1) == element);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::endsWith(const BasicString& subString) const noexcept
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

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::removeAll(const ValueType& element) {

}


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::assign(const BasicString& string) 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::assign(BasicString&& string)
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::assign(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::assign(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::assign(const ValueType* string) 
{
	
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>
	::assign(std::initializer_list<ValueType> initializerList)
{
	return assign(initializerList.data(), initializerList.size());
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::assign(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::insert(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::insert(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::iterator
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::insert(
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
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType>
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findAll(const BasicString& string) const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
NODISCARD container::Vector<typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType> 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findAll(ValueType ch) const noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstOf(
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

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findFirstNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::findLastNotOf(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rfind(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rfind(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rfind(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::rfind(
		ValueType	ch,
		SizeType	position) const
{
	
}


// =======================================================================================
//									STL compatibility
// =======================================================================================


template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
constexpr inline NODISCARD sizetype BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::max_size() const noexcept {
	return static_cast<std::size_t>(-1) / sizeof(ValueType);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::allocator_type&
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::get_allocator() noexcept
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::allocator_type& 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::get_allocator() const noexcept 
{

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::shrink_to_fit() {
	 
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_first_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::find_last_not_of(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::substr(
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
BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::substr(
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
int BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::compare(const BasicString& string) const {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
int BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::compare(
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
int BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::compare(
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
int BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::compare(
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
int BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::compare(
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
int BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::compare(const ValueType* string) const {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
constexpr NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::SizeType 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::calculateGrowth(SizeType newSize) noexcept 
{
	const auto oldCapacity	= capacity();
	const auto _MaxSize		= maxSize();

	if (oldCapacity > _MaxSize - oldCapacity / 1.5)
		return _MaxSize; // Overflow

	const SizeType geometricGrowth = oldCapacity + oldCapacity / 1.5;

	if (geometricGrowth < newSize)
		return newSize;

	return geometricGrowth;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD bool BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::resizeReallocate(SizeType newCapacity) noexcept {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline NODISCARD BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::Reference
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::emplaceBack(ValueType value) 
{


}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::emplaceBackWithUnusedCapacity(ValueType value) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::emplaceBackReallocate(ValueType value) {

}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
CONSTEXPR_CXX20 inline void BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::emplaceAt(
	pointer&		location,
	ValueType		value)
{
	memory::ConstructInPlace(*location, value);
	++location;
}

// =======================================================================================
//										Operators
// =======================================================================================

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs)
{
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> result;

	result.reserve(lhs.size() + rhs.size());
	result.append(lhs).append(rhs);

	return result;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&&			lhs,
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs) 
{
	return std::move(lhs.append(rhs));
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&&			rhs) 
{
	if (rhs.capacity() >= lhs.size() + rhs.size()) {
		// Good, at least we don't need to reallocate
		return std::move(rhs.insert(0, lhs));
	}

	// Meh, no go. Forward to operator+(const&, const&).
	auto const& rhsC = rhs;
	return lhs + rhsC;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&& lhs,
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&& rhs)
{
	return std::move(lhs.append(rhs));
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	const _Char_*																		lhs, 
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs)
{
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> result;

	const auto len = BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type::length(lhs);

	result.reserve(len + rhs.size());
	result.append(lhs, len).append(rhs);

	return result;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	const _Char_*																lhs,
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&& rhs) 
{
	const auto len = BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type::length(lhs);

	if (rhs.capacity() >= len + rhs.size()) {
		rhs.insert(rhs.begin(), lhs, lhs + len);
		return std::move(rhs);
	}

	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> result;

	result.reserve(len + rhs.size());
	result.append(lhs, len).append(rhs);

	return result;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	_Char_																				lhs,
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs) 
{
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> result;

	result.reserve(1 + rhs.size());
	result.push_back(lhs);

	result.append(rhs);

	return result;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	_Char_																		lhs,
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&& rhs) 
{
	if (rhs.capacity() > rhs.size()) {
		rhs.insert(rhs.begin(), lhs);
		return std::move(rhs);
	}

	// Meh, no go. Forward to operator+(E, const&).
	auto const& rhsC = rhs;
	return lhs + rhsC;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
	const _Char_*																		rhs) 
{
	using size_type = typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::size_type;
	using traits_type = typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type;

	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> result;

	const size_type len = traits_type::length(rhs);

	result.reserve(lhs.size() + len);
	result.append(lhs).append(rhs, len);

	return result;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&& lhs,
	const _Char_*																rhs)
{
	return std::move(lhs += rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
	_Char_																				rhs) 
{
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> result;

	result.reserve(lhs.size() + 1);
	result.append(lhs);

	result.push_back(rhs);

	return result;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_> operator+(
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&& lhs,
	_Char_ rhs)
{
  return std::move(lhs += rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator==(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs) 
{
	return lhs.size() == rhs.size() 
		&& lhs.compare(rhs) == 0;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator==(
	std::nullptr_t, 
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator==(
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						rhs) 
{
	return rhs == lhs;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator==(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&,
	std::nullptr_t) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator==(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						lhs,
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	rhs) 
{
	return lhs.compare(rhs) == 0;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator!=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs) 
{
	return !(lhs == rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator!=(
	std::nullptr_t, 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator!=(
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						rhs) 
{
	return !(lhs == rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator!=(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&, 
	std::nullptr_t) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator!=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						lhs,
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	rhs) 
{
	return !(lhs == rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs)
{
	return lhs.compare(rhs) < 0;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&,
	std::nullptr_t) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						lhs,
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	rhs) 
{
	return lhs.compare(rhs) < 0;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<(
	std::nullptr_t, 
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<(
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						rhs) 
{
	return rhs.compare(lhs) > 0;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs) {
  return rhs < lhs;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&,
	std::nullptr_t) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						lhs,
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	rhs)
{
	return rhs < lhs;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>(
	std::nullptr_t, 
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>(
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						rhs)
{
	return rhs < lhs;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs)
{
	return !(rhs < lhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<=(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&,
	std::nullptr_t) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						lhs,
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	rhs) {
  return !(rhs < lhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<=(
	std::nullptr_t, 
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<=(
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						rhs)
{
	return !(rhs < lhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs)
{
	return !(lhs < rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>=(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&, 
	std::nullptr_t) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						lhs,
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	rhs) 
{
	return !(lhs < rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>=(
	std::nullptr_t,
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator>=(
    const typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type*	lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&						rhs) 
{
	return !(lhs < rhs);
}

#if BASE_HAS_CXX20
template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<=>(
	std::nullptr_t,
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&) = delete;

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline bool operator<=>(
	const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&,
	std::nullptr_t) = delete;
#endif

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
void swap(
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& lhs,
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& rhs) 
{
	lhs.swap(rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline std::basic_istream<
    typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type,
    typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type>&
operator>>(
    std::basic_istream<
        typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type,
        typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type>& is,
	BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& str) 
{
	using _InputStream_ = std::basic_istream<
      typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type,
      typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type>;

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

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_>
inline std::basic_ostream<
    typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type,
    typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type>&
operator<<(
    std::basic_ostream<
        typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type,
        typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type>& os,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>& str) 
{
#if defined(CPP_GNU)
	using _OutputStream_ = std::basic_ostream<
      typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type,
      typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type>;

	using _SEntry_ = _OutputStream_::sentry;
	using _Ip_ = _OutputStream_::std::ostreambuf_iterator<
		typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::value_type,
		typename BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>::traits_type>

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

// basic_string compatibility routines

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator==(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs) 
{
	return (lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) == 0);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator==(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs)
{
	return rhs == lhs;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator!=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return !(lhs == rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator!=(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs)
{
	return !(lhs == rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator<(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return (lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) < 0);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator>(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return (lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) > 0);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator<(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs) 
{
	return rhs > lhs;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator>(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs) 
{
	return rhs < lhs;
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator<=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs)
{
	return !(lhs > rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator>=(
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	lhs,
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						rhs) 
{	
	return !(lhs < rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator<=(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs) 
{
	return !(lhs > rhs);
}

template <
	class _Char_,
	class _Traits_,
	class _Allocator_,
	class _SimdOptimization_,
	class _Storage_,
	class _SecondAllocator_>
inline bool operator>=(
    const std::basic_string<_Char_, _Traits_, _SecondAllocator_>&						lhs,
    const BasicString<_Char_, _Traits_, _Allocator_, _SimdOptimization_, _Storage_>&	rhs)
{
	return !(lhs < rhs);
}

__BASE_STRING_NAMESPACE_END
