#pragma once


#include <base/core/BaseNamespace.h>
#include <base/core/memory/MemoryUtility.h>


__BASE_CONTAINER_NAMESPACE_BEGIN

#ifndef _DECLARE_VECTOR_ALGORITHM_TYPES_
#define _DECLARE_VECTOR_ALGORITHM_TYPES_(_Vector_)                                      \
	using pointer					= typename _Vector_::pointer;                       \
	using const_pointer				= typename _Vector_::const_pointer;                 \
	using size_type					= typename _Vector_::size_type;						\
	using difference_type			= typename _Vector_::difference_type;				\
	using value_type				= typename _Vector_::value_type;                    \
	using allocator_type			= typename _Vector_::allocator_type;                \
	using reference					= typename _Vector_::reference;                     \
	using const_reference			= typename _Vector_::const_reference;               \
	using iterator					= typename _Vector_::iterator;                      \
	using const_iterator			= typename _Vector_::const_iterator;                \
	using reverse_iterator			= typename _Vector_::reverse_iterator;				\
	using const_reverse_iterator	= typename _Vector_::const_reverse_iterator;		\
	using Iterator					= iterator;                                         \
	using ConstIterator				= const_iterator;                                   \
	using ValueType					= value_type;                                       \
	using SizeType					= size_type;                                        \
	using Reference					= reference;                                        \
	using ConstReference			= const_reference;                                  \
	using Pointer					= pointer;                                          \
	using ConstPointer				= const_pointer;                                    \
	using ReverseIterator			= reverse_iterator;                                 \
	using ConstReverseIterator		= const_reverse_iterator;
#endif

struct _Vector_Scalar_Algorithm_Tag_ { };
struct _Vector_SIMD_Algorithm_Tag_   { };

enum class _Vector_SIMD_Algorithm_Alignment : sizetype { };

static constexpr _Vector_SIMD_Algorithm_Alignment _VectorSIMDAlignment =
	_Vector_SIMD_Algorithm_Alignment
#if LIB_BASE_SIMD_ENABLE
	{ MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT };
#else
	{ 0 }
#endif
	;

template <
	typename _SpecializationTag_,
	typename _Vector_>
class VectorAlgorithm 
{};

template <typename _Vector_>
class VectorAlgorithm<_Vector_Scalar_Algorithm_Tag_, _Vector_> {
public:
	_DECLARE_VECTOR_ALGORITHM_TYPES_(_Vector_)

	static inline NODISCARD size_type indexOf(
		const_reference element,
		size_type from) const noexcept
	{
		
	}

	static inline NODISCARD size_type lastIndexOf(
		const_reference element,
		size_type from = 0) const noexcept
	{
		
	}

	static inline NODISCARD bool contains(
		const_reference element,
		size_type from = 0) const noexcept
	{
		
	}

	static inline NODISCARD bool contains(
		const Vector<value_type> subVector,
		size_type from = 0) const noexcept
	{
		
	}

	static inline NODISCARD size_type count(
		const_reference element) const noexcept
	{
		
	}

	static inline NODISCARD size_type count(
		const Vector<value_type>& subVector) const noexcept
	{
		for ()
	}

	static inline void fill(
		const_reference fill) noexcept 
	{
		for (Iterator i = first; i < last; ++i)
			*i = fill;
	}
};

template <typename _Vector_>
class VectorAlgorithm<_Vector_SIMD_Algorithm_Tag_, _Vector_> {
public:
	_DECLARE_VECTOR_ALGORITHM_TYPES_(_Vector_)
};


__BASE_CONTAINER_NAMESPACE_END