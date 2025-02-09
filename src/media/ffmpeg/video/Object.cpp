#include <base/media/ffmpeg/video/Object.h>

#include <base/Utility.h>
#include <base/media/ffmpeg/video/Variables.h>


namespace base::media::ffmpeg::video {
    res** obj_res(object_t* obj)
    {
        return &objectPrivate(obj)->resources;
    }

    void* objres_new(size_t size, void (*release)(void*))
    {
        if (unlikely(ckd_add(&size, sizeof(struct res), size)))
        {
            errno = ENOMEM;
            return NULL;
        }

        struct res* _res = (struct res*)malloc(size);
        if (unlikely(_res == NULL))
            return NULL;

        _res->release = release;
        return _res->payload;
    }

    void objres_push(object_t* obj, void* data)
    {
        struct res** pp = obj_res(obj);
        struct res* _res = container_of(data, struct res, payload);

        _res->prev = *pp;
        *pp = _res;
    }

    void* objres_pop(object_t* obj)
    {
        struct res** pp = obj_res(obj);
        struct res* _res = *pp;

        if (_res == NULL)
            return NULL;
        *pp = _res->prev;
        return _res->payload;
    }

    void objres_clear(object_t* obj)
    {
        void* data;

        while ((data = objres_pop(obj)) != NULL)
        {
            struct res* _res = container_of(data, struct res, payload);

            _res->release(_res->payload);
            free(_res);
        }
    }

    void objres_remove(object_t* obj, void* data,
        bool (*match)(void*, void*))
    {
        struct res** pp = obj_res(OBJECT(obj));

        /* With a doubly-linked list, this function could have constant complexity.
         * But that would require one more pointer per resource.
         *
         * Any given list should contain a fairly small number of resources,
         * and in most cases, the resources are destroyed implicitly by
         * objres_clear().
         */
        for (;;)
        {
            struct res* _res = *pp;

            assert(_res != NULL); /* invalid free? */

            if (match(_res->payload, data))
            {
                *pp = _res->prev;
                _res->release(_res->payload);
                free(_res);
                return;
            }

            pp = &_res->prev;
        }
    }

    void dummy_release(void* data)
    {
        (void)data;
    }

    bool ptrcmp(void* a, void* b)
    {
        return a == b;
    }

    void* obj_malloc(object_t* obj, size_t size)
    {
        void* ptr = objres_new(size, dummy_release);
        if (likely(ptr != NULL))
            objres_push(obj, ptr);
        return ptr;
    }

    void* obj_calloc(object_t* obj, size_t nmemb, size_t size)
    {
        size_t tabsize;
        if (unlikely(ckd_mul(&tabsize, nmemb, size)))
        {
            errno = ENOMEM;
            return NULL;
        }

        void* ptr = obj_malloc(obj, tabsize);
        if (likely(ptr != NULL))
            memset(ptr, 0, tabsize);
        return ptr;
    }

    void object_deinit(object_t* obj)
    {
        object_internals_t* priv = objectPrivate(obj);

        assert(priv->resources == NULL);

        /* Destroy the associated variables. */
        int canc = Threads::savecancel();
        var_DestroyAll(obj);
        Threads::restorecancel(canc);

        free(priv);
    }

    void object_delete(object_t* obj)
    {
        object_deinit(obj);
        free(obj);
    }

    template<typename O>
    void object_delete(O* obj)
    {
        if (!std::is_trivially_destructible<O>::value)
            obj->~O();
        object_delete(OBJECT(obj));
    }

    void* obj_memdup(object_t* obj, const void* base, size_t len)
    {
        void* ptr = obj_malloc(obj, len);
        if (likely(ptr != NULL))
            memcpy(ptr, base, len);
        return ptr;
    }

    char* obj_strdup(object_t* obj, const char* str)
    {
        return (char*)obj_memdup(obj, str, strlen(str) + 1);
    }

    void obj_free(object_t* obj, void* ptr)
    {
        objres_remove(obj, ptr, ptrcmp);
    }
} // namespace base::media::ffmpeg::video