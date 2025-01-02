#pragma once

#include "Platform.h"
#include <iostream>

#if _has_include(gsl/gsl)
/**
 *После выхода из текущей области видимости определяет затраченное на выполнение этого блока кода время
 *\param name - "Имя" области видимости
*/
#define measureExecutionTime(name) \
		 const auto ms = Time::now(); \
		 const auto timer = gsl::finally([] { \
			std::cout << name << " completed for: " \
			<< Time::now() - ms << " ms" << '\n'; });
#endif

#define container_of(ptr, type, member) \
    ((type *)(((char *)(ptr)) - offsetof(type, member)))


namespace core::utility {
	struct rational_t {
		unsigned num, den;
	};

    struct node_t {
        char* key;
        struct node_t* llink, * rlink;
    };

    enum VISIT {
        preorder,
        postorder,
        endorder,
        leaf
    };

    typedef void (*cmp_fn_t)(const void*, VISIT, int);

	LIB_BASE [[nodiscard]] int64_t GCD(
		int64_t a,
		int64_t b);

	LIB_BASE [[nodiscard]] int LCM(int a, int b);

	LIB_BASE bool UReduce(
		unsigned* pi_dst_nom, unsigned* pi_dst_den,
		uint64_t i_nom, uint64_t i_den, uint64_t i_max);

	LIB_BASE [[nodiscard]] double SafeRound(double value);

    LIB_BASE [[nodiscard]] void* TFind(
        const void* vkey,
        void* const* vrootp,
        int (*compar)(const void*,
            const void*));

    LIB_BASE [[nodiscard]] void* TSearch(
        const void* vkey,
        void** vrootp,
        int (*compar)(const void*,
            const void*));

    LIB_BASE void TRecurse(
        const node_t* root,
        cmp_fn_t action, int level);

    LIB_BASE void TWalk(
        const void* vroot,
        cmp_fn_t action);

    LIB_BASE int CmpSmallest(
        const void* a,
        const void* b);

    LIB_BASE void TDestroyRecurse(
        node_t* root,
        void (*free_action)(void*));

    LIB_BASE void TDestroy(
        void* root,
        void (*freenode)(void*));
} // namespace core::utility