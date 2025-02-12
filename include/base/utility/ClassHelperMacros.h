#pragma once 

#include <base/system/CompilerDetection.h>


#ifdef LIB_BASE_ENABLE_QT
    #include <qtclasshelpermacros.h>

    #define DISABLE_COPY Q_DISABLE_COPY
    #define DISABLE_COPY_MOVE Q_DISABLE_COPY_MOVE

    #define MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP
    #define MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP

    #define DECLARE_PRIVATE Q_DECLARE_PRIVATE
    #define DECLARE_PRIVATE_D Q_DECLARE_PRIVATE_D

    #define _D Q_D
    #define _Q Q_Q
#else
	#define DISABLE_COPY(Class) \
		Class(const Class&) = delete;\
		Class& operator=(const Class&) = delete;

	#define DISABLE_COPY_MOVE(Class) \
		DISABLE_COPY(Class) \
		Class(Class&&) = delete; \
		Class &operator=(Class &&) = delete;

    #define MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(Class) \
        Class &operator=(Class &&other) noexcept { \
            Class moved(std::move(other)); \
            swap(moved); \
            return *this; \
        }

    #define MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(Class) \
        Class &operator=(Class &&other) noexcept { \
            swap(other); \
            return *this; \
        }


    template <typename T> inline T* GetPtrHelper(T* ptr) noexcept { return ptr; }
    template <typename Ptr> inline auto GetPtrHelper(Ptr& ptr) noexcept -> decltype(ptr.get())
    {
        static_assert(noexcept(ptr.get()), "Smart d pointers for DECLARE_PRIVATE must have noexcept get()"); return ptr.get();
    }

    #define DECLARE_PRIVATE(Class) \
        inline Class##Private* d_func() noexcept \
        { CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private *>(GetPtrHelper(d_ptr));) } \
        inline const Class##Private* d_func() const noexcept \
        { CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private *>(GetPtrHelper(d_ptr));) } \
        friend class Class##Private;

    #define DECLARE_PRIVATE_D(Dptr, Class) \
        inline Class##Private* d_func() noexcept \
        { CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private *>(GetPtrHelper(Dptr));) } \
        inline const Class##Private* d_func() const noexcept \
        { CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private *>(GetPtrHelper(Dptr));) } \
        friend class Class##Private;

    #define _D(Class) Class##Private * const d = d_func()
    #define _U(Class) Class * const u = u_func()
#endif