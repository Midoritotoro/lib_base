#pragma once 


#include "Config.h"

#include <cstddef>
#include <type_traits>


namespace FFmpeg {
    struct log_t
    {
        uintptr_t   i_object_id; /**< Emitter (temporarily) unique object ID or 0 */
        const char* psz_object_type; /**< Emitter object type name */
        const char* psz_module; /**< Emitter module (source code) */
        const char* psz_header; /**< Additional header (used by VLM media) */
        const char* file; /**< Source code file name or NULL */
        int line; /**< Source code file line number or -1 */
        const char* func; /**< Source code calling function name or NULL */
        unsigned long tid; /**< Emitter thread ID */
    };

    typedef void (*log_cb) (void* data, int type, const log_t* item,
        const char* fmt, va_list args);

    struct logger_operations
    {
        log_cb log;
        void (*destroy)(void* data);
    };

    struct logger {
        const logger_operations* ops;
    };

    struct object_t;
    struct object_internals_t
    {
        object_t* parent; /**< Parent object (or NULL) */
        const char* _typename; /**< Object type human-readable name */

        /* Object variables */
        void* var_root;
        ::Threads::mutex_t     var_lock;

        /* Object resources */
        struct res* resources;
    };


    struct object_t
    {
        logger* logger;
        union {
            struct object_internals_t* priv;
            struct object_marker* obj;
        };

        bool no_interact;

        /** Module probe flag
         *
         * A boolean during module probing when the probe is "forced".
         * See \ref module_need().
         */
        bool force;
    };

    struct res
    {
        struct res* prev;
        void (*release)(void*);
        max_align_t payload[];
    };

    res** obj_res(object_t* obj);
    void* objres_new(size_t size, void (*release)(void*));

    void objres_push(object_t* obj, void* data);

    void* objres_pop(object_t* obj);
    void objres_clear(object_t* obj);

    void objres_remove(object_t* obj, void* data,
        bool (*match)(void*, void*));

    void dummy_release(void* data);

    bool ptrcmp(void* a, void* b);

    void* obj_malloc(object_t* obj, size_t size);
    void* obj_calloc(object_t* obj, size_t nmemb, size_t size);

    void* obj_memdup(object_t* obj, const void* base, size_t len);
    char* obj_strdup(object_t* obj, const char* str);

    void obj_free(object_t* obj, void* ptr);

    template <typename type>
    inline bool ckd_add(type* r, type a, type b)
    {
        *r = a + b;
        return ((type)(a + b)) < a;
    }

    template <typename type>
    inline bool ckd_sub(type* r, type a, type b)
    {
        *r = a - b;
        return a < b;
    }

    template <typename type>
    inline bool ckd_mul(type* r, type a, type b)
    {
        if (b == 0) return true;
        *r = a * b;
        return a > (INT_MAX / b);
    }

    inline object_t* OBJECT(object_t* o)
    {
        return o;
    }

    template<typename T>
    inline object_t* OBJECT(T* d)
    {
        return &d->obj;
    }

    inline object_internals_t* objectPrivate(object_t* object) {
        return (object->priv);
    }

    inline object_t* object_parent(object_t* object) {
        return objectPrivate(object)->parent;
    }

    void object_deinit(object_t* obj);


    void object_delete(object_t* obj);

    template<typename O>
    void object_delete(O* obj);

} // namespace FFmpeg
