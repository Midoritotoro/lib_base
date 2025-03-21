#pragma once

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryAllocator.h>

#include <base/core/container/VectorIterator.h> 
#include <base/core/memory/MemoryUtility.h>

#include <base/core/container/STLCompatibleContainerBase.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

struct _Vector_Scalar_Algorithm_Tag_	{ };
struct _Vector_SIMD_Algorithm_Tag_		{ };

enum class _Vector_SIMD_Algorithm_Alignment : sizetype { };

static constexpr _Vector_SIMD_Algorithm_Alignment _VectorSIMDAlignment =
	_Vector_SIMD_Algorithm_Alignment
#if LIB_BASE_HAS_SIMD_SUPPORT
	{ MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT };
#else
	{ 0 }
#endif
	;

template <
	typename _Element_,
	class _Allocator_ = memory::RawMemoryAllocator<_Element_>>
class VectorBase : 
	public STLCompatibleContainerBase<
		_Element_,
		VectorIterator<
			VectorBase<_Element_, _Allocator_>>,
		VectorConstIterator<
			VectorBase<_Element_, _Allocator_>>>
{
public:
	using _MyVectorBase_ = STLCompatibleContainerBase<
		_Element_,
		VectorIterator<
			VectorBase<_Element_, _Allocator_>>,
		VectorConstIterator<
			VectorBase<_Element_, _Allocator_>>>;
	
	using pointer					= typename _MyVectorBase_::pointer;									
	using const_pointer				= typename _MyVectorBase_::const_pointer;	

	using size_type					= typename _MyVectorBase_::size_type;								
	using difference_type			= typename _MyVectorBase_::difference_type;		

	using value_type				= typename _MyVectorBase_::value_type;								
	using allocator_type			= _Allocator_;			

	using reference					= value_type&;														
	using const_reference			= const value_type&;		

	using iterator					= typename _MyVectorBase_::iterator;									
	using const_iterator			= typename _MyVectorBase_::const_iterator;		

	using reverse_iterator			= typename _MyVectorBase_::reverse_iterator;					
	using const_reverse_iterator	= typename _MyVectorBase_::const_reverse_iterator;	

	using Iterator					= iterator;															
	using ConstIterator				= const_iterator;	

	using ValueType					= value_type;														
	using SizeType					= size_type;	

	using Reference					= reference;														
	using ConstReference			= const_reference;			

	using Pointer					= pointer;															
	using ConstPointer				= const_pointer;

	using ReverseIterator			= reverse_iterator;											
	using ConstReverseIterator		= const_reverse_iterator;

	static constexpr sizetype _Buffer_For_Resizing_Length = 16; // ... 
};

template <
	class		_AlgorithmTag_,
	typename	_Element_,
	class		_Allocator_>
class Vector :
	public VectorBase<_Element_, _Allocator_>
{
	
};

template <
	typename	_Element_,
	class		_Allocator_>
class Vector<_Vector_Scalar_Algorithm_Tag_, _Element_, _Allocator_>:
	public VectorBase<_Element_, _Allocator_>
{
public:
	using _MyBase_	= VectorBase<_Element_, _Allocator_>;

	using pointer					= typename _MyBase_::pointer;
	using const_pointer				= typename _MyBase_::const_pointer;

	using size_type					= typename _MyBase_::size_type;
	using difference_type			= typename _MyBase_::difference_type;

	using value_type				= typename _MyBase_::value_type;
	using allocator_type			= typename _MyBase_::_Allocator_;

	using reference					= typename _MyBase_::value_type&;
	using const_reference			= const typename _MyBase_::value_type&;

	using iterator					= typename _MyBase_::iterator;
	using const_iterator			= typename _MyBase_::const_iterator;

	using reverse_iterator			= typename _MyBase_::reverse_iterator;
	using const_reverse_iterator	= typename _MyBase_::const_reverse_iterator;

	using Iterator					= iterator;															
	using ConstIterator				= const_iterator;	

	using ValueType					= value_type;														
	using SizeType					= size_type;	

	using Reference					= reference;														
	using ConstReference			= const_reference;			

	using Pointer					= pointer;															
	using ConstPointer				= const_pointer;

	using ReverseIterator			= reverse_iterator;											
	using ConstReverseIterator		= const_reverse_iterator;

    constexpr Vector() noexcept {};
    constexpr ~Vector() noexcept = default;

    constexpr inline Vector(std::initializer_list<ValueType> elements) noexcept {
		const auto _Capacity		= capacity();
		const auto _UnusedCapacity	= unusedCapacity();

		const auto elementsSize		= elements.size();

		if (_UnusedCapacity < elementsSize)
			TryResize(_Capacity + elementsSize);

		for (SizeType i = 0; i < elementsSize; ++i) {
			const auto adress = (_start + i);
			*adress = (elements[i]);
		}
    }

    inline Vector(const Vector<ValueType>& other) {
		const auto _Capacity = other.capacity();
		TryResize(_Capacity);

		if (memory::MemoryCopy(_start, other._start, _Capacity) == false)
			AssertReturn(false, "base::container::Vector::Vector: Ошибка при копировании элементов");
	}

    inline Vector(const std::vector<ValueType>& vector) {
		const auto _Capacity = vector.capacity();
		TryResize(_Capacity);

		if (memory::MemoryCopy(_start, vector.data(), _Capacity) == false)
			AssertReturn(false, "base::container::Vector::Vector: Ошибка при копировании элементов");
    }

    inline Vector(const SizeType capacity) {
		TryResize(capacity);
    }

    inline Vector(
        const SizeType _Capacity,
        const ValueType& _Fill)
    {
		TryResize(_Capacity);
		fill(_Fill);
    }
    
    inline Vector(Vector<ValueType>&& rOther) noexcept {

    }

    inline void push_back(const ValueType& element) {

		
		++_current;

		memory::MemoryMove(_current, )
    }

    inline void push_front(const ValueType& element) {

    }

    inline void append(const ValueType& element) {

    }
   	
	inline NODISCARD ValueType pop() noexcept {
		const auto value = back();
		remveAt(size() - 1);

		return value;
	}

	inline NODISCARD SizeType find(ConstReference element) const noexcept {
		return 0;
	}

	inline NODISCARD Vector<size_type> 
		findAll(ConstReference element) const noexcept
	{
		return {}
	}
	
	inline NODISCARD size_type
		findLastOf(ConstReference element) const noexcept
	{
		
	}

	inline NODISCARD size_type 
		findFirstOf(const_reference element) const noexcept
	{
		;
	}

	inline NODISCARD size_type
		findLastNotOf(const_reference element) const noexcept
	{
		return 0;
	}
	
	inline NODISCARD size_type 
		findFirstNotOf(const_reference element) const noexcept
	{
		return 0;
	}

	inline void clear() {
		UNUSED(erase(constBegin(), constEnd()));
	}

	inline NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last)
	{
		return {};
	}

	inline NODISCARD Iterator erase(ConstIterator it) {
		return {};
	}

	inline NODISCARD bool resize(
		size_type size,
		const_reference _Fill)
	{
		const auto resizeSuccess = TryResize(size);
		const auto fillSuccess   = fill(_Fill);

		const auto success		 = (resizeSuccess == true && fillSuccess == true);
		return success;
	}

    inline NODISCARD size_type indexOf(
		const_reference element,
		size_type from = 0) const noexcept
	{
		for (size_type i = from; i < size(); ++i) {
			const auto adress = (_start + i);

			if ((*adress) == element)
				return i;
		}

		return -1;
	}

	inline NODISCARD size_type lastIndexOf(
		const_reference element,
		size_type from = 0) const noexcept
	{
		
	}

	inline NODISCARD bool contains(
		const_reference element,
		size_type from = 0) const noexcept
	{
		for (size_type i = from; i < size(); ++i) {
			const auto adress = (_start + i);

			if ((*adress) == element)
				return true;
		}

		return false;
	}

	inline NODISCARD bool contains(
		const Vector<value_type> subVector,
		size_type from = 0) const noexcept
	{
		size_type overlaps = 0;
		const auto subVectorSize = subVector.size();

		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);

			if ((*adress) == subVector[i % subVectorSize] && (++overlaps == subVectorSize))
				return true;
		}

		return false;
	}
	
	inline NODISCARD size_type count(
		const_reference element) const noexcept
	{
		size_type _Count = 0;

		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);
			_Count += ((*adress) == element);
		}
	}
	
	inline NODISCARD size_type count(
		const Vector<value_type>& subVector) const noexcept
	{
		size_type _Count = 0;
		size_type overlaps = 0;

		const auto subVectorSize = subVector.size();

		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);

			((*adress) == subVector[i % subVectorSize] 
				&& (++overlaps == subVectorSize))
					? ++_Count;
					: overlaps = 0;
		}
		
		return _Count;
	}
	
	inline void fill(const_reference _Fill) noexcept {
		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);
			*adress = _Fill;
		}
	}

	inline NODISCARD bool resize(const SizeType _Capacity) {
		const auto bytesRequired = _Capacity * sizeof(ValueType);
		if (UNLIKELY(bytesRequired <= 0))
			return false;

		_Allocator_ allocator = {};

		const auto memory = allocator.Allocate(bytesRequired);

		if (UNLIKELY(memory == nullptr))
			return false;

		const auto blockStart	= memory;
		const auto blockEnd		= memory + _Capacity;

		UNUSED(memory::UnInitializedCopy(
			begin(), end(),
			blockStart, blockEnd));

		_start		= blockStart;
		_end		= blockEnd;
			
		_current	= nullptr;
	}

	inline void insert(
		size_type index,
		const_reference element) noexcept
	{

	}

	inline void prepend(const_reference element) noexcept {

	}
private:
	inline void TryResize(sizetype newCapacity) {
		const auto isEnoughMemory = resize(newCapacity);

		if (UNLIKELY(isEnoughMemory == false))
			AssertReturn(false, "base::container::Vector: Not enough memory to expand the Vector\n", UNUSED(0));
	}

	inline void InsertToAdress(
		sizetype offset, 
		ValueType* value)
	{
		const auto adressForOffset = (_start + offset);
		
	}
};

template <
	typename _Element_,
	class _Allocator_>
class Vector<_Vector_SIMD_Algorithm_Tag_, _Element_, _Allocator_> :
	public VectorBase<_Element_, _Allocator_>
{
public:
	using _MyBase_ = VectorBase<_Element_, _Allocator_>;

	using pointer					= typename _MyBase_::pointer;
	using const_pointer				= typename _MyBase_::const_pointer;

	using size_type					= typename _MyBase_::size_type;
	using difference_type			= typename _MyBase_::difference_type;

	using value_type				= typename _MyBase_::value_type;
	using allocator_type			= typename _MyBase_::_Allocator_;

	using reference					= typename _MyBase_::value_type&;
	using const_reference			= const typename _MyBase_::value_type&;

	using iterator					= typename _MyBase_::iterator;
	using const_iterator			= typename _MyBase_::const_iterator;

	using reverse_iterator			= typename _MyBase_::reverse_iterator;
	using const_reverse_iterator	= typename _MyBase_::const_reverse_iterator;

	using Iterator					= iterator;															
	using ConstIterator				= const_iterator;	

	using ValueType					= value_type;														
	using SizeType					= size_type;	

	using Reference					= reference;														
	using ConstReference			= const_reference;			

	using Pointer					= pointer;															
	using ConstPointer				= const_pointer;

	using ReverseIterator			= reverse_iterator;											
	using ConstReverseIterator		= const_reverse_iterator;


	inline NODISCARD bool resize(const SizeType _Capacity) {
		const auto bytesRequired = _Capacity * sizeof(ValueType);
		if (UNLIKELY(bytesRequired <= 0))
			return false;

		_Allocator_ allocator = {};

		const auto memory = allocator.AllocateAligned(bytesRequired, _VectorSIMDAlignment);

		if (UNLIKELY(memory == nullptr))
			return false;

		const auto blockStart = memory;
		const auto blockEnd = memory + _Capacity;

		UNUSED(memory::UnInitializedCopy(
			_MyBase_::begin(), _MyBase_::end(),
			blockStart, blockEnd));

		_start = blockStart;
		_end = blockEnd;

		_current = nullptr;
	}

	inline NODISCARD bool resize(
		size_type size,
		const_reference _Fill)
	{
		const auto resizeSuccess = resize(size);
		const auto fillSuccess = fill(_Fill);

		const auto success = (resizeSuccess == true && fillSuccess == true);
		return success;
	}
};


__BASE_CONTAINER_NAMESPACE_END