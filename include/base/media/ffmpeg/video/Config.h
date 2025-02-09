#pragma once 

#include <base/system/Platform.h>
#include <base/concurrent/win/WinapiThreads.h>

#define SUCCESS                             0
/** Unspecified error */
#define EGENERIC                            (-2 * (1 << (sizeof (int) * 8 - 2))) /* INT_MIN */

#define CONFIG_HINT_CATEGORY                0x02  /* Start of new category */

#define CONFIG_SUBCATEGORY                  0x07 /* Set subcategory */
#define CONFIG_SECTION                      0x08 /* Start of new section */

/* Configuration item types */
#define CONFIG_ITEM_FLOAT                   (1 << 5)  /* Float option */
#define CONFIG_ITEM_INTEGER                 (2 << 5)  /* Integer option */
#define CONFIG_ITEM_RGB                     (CONFIG_ITEM_INTEGER | 0x01)  /* RGB color option */
#define CONFIG_ITEM_BOOL                    (3 << 5)  /* Bool option */
#define CONFIG_ITEM_STRING                  (4 << 5)  /* String option */
#define CONFIG_ITEM_PASSWORD                (CONFIG_ITEM_STRING | 0x01)  /* Password option (*) */
#define CONFIG_ITEM_KEY                     (CONFIG_ITEM_STRING | 0x02)  /* Hot key option */
#define CONFIG_ITEM_MODULE                  (CONFIG_ITEM_STRING | 0x04)  /* Module option */
#define CONFIG_ITEM_MODULE_CAT              (CONFIG_ITEM_STRING | 0x05)  /* Module option */
#define CONFIG_ITEM_MODULE_LIST             (CONFIG_ITEM_STRING | 0x06)  /* Module option */
#define CONFIG_ITEM_MODULE_LIST_CAT         (CONFIG_ITEM_STRING | 0x07)  /* Module option */
#define CONFIG_ITEM_LOADFILE                (CONFIG_ITEM_STRING | 0x0C)  /* Read file option */
#define CONFIG_ITEM_SAVEFILE                (CONFIG_ITEM_STRING | 0x0D)  /* Written file option */
#define CONFIG_ITEM_DIRECTORY               (CONFIG_ITEM_STRING | 0x0E)  /* Directory option */
#define CONFIG_ITEM_FONT                    (CONFIG_ITEM_STRING | 0x0F)  /* Font option */

/* reduce specific type to type class */
#define CONFIG_CLASS(x) ((x) & ~0x1F)

/* is proper option, not a special hint type? */
#define CONFIG_ITEM(x) (((x) & ~0xF) != 0)

#define IsConfigStringType(type) \
    (((type) & CONFIG_ITEM_STRING) != 0)
#define IsConfigIntegerType(type) \
    (((type) & CONFIG_ITEM_INTEGER) != 0)
#define IsConfigFloatType(type) \
    ((type) == CONFIG_ITEM_FLOAT)


namespace FFmpeg {
    struct module_t;
    struct plagin_t;

    union value_t
    {
        int64_t         i_int;
        bool            b_bool;
        float           f_float;
        char* psz_string;
        void* p_address;
        struct { int32_t x; int32_t y; } coords;

    };


    struct plugin_t
    {
        struct plugin_t* next;
        module_t* module;
        unsigned modules_count;

        const char* textdomain; /**< gettext domain (or NULL) */

        /**
         * Variables set by the module to store its config options
         */
        struct
        {
            struct param* params; /**< Table of configuration items */
            size_t size; /**< Total count of all items */
            size_t count; /**< Count of real options (excludes hints) */
            size_t booleans; /**< Count of options that are of boolean type */
        } conf;
    };

    struct module_t
    {
        plugin_t* plugin; /**< Plug-in/library containing the module */
        module_t* next;

        /** Shortcuts to the module */
        unsigned    i_shortcuts;
        const char** pp_shortcuts;

        /*
         * Variables set by the module to identify itself
         */
        const char* psz_shortname;                              /**< Module name */
        const char* psz_longname;                   /**< Module descriptive name */
        const char* psz_help;        /**< Long help plain string for "special" modules */
        const char* psz_help_html;   /**< Long help HTML string, shown instead of the plain help where it makes sense to render HTML.
                                          Supports only a limited HTML4 subset, see https://doc.qt.io/qt-6/richtext-html-subset.html */

        const char* psz_capability;                              /**< Capability */
        int      i_score;                          /**< Score for the capability */

        /* Callbacks */
        const char* activate_name;
        const char* deactivate_name;
        void* pf_activate;
    };

    union module_value_t
    {
        char* psz;
        int64_t     i;
        float       f;
    };

    struct module_config_t
    {
        uint8_t     i_type; /**< Configuration type */

        const char* psz_type; /**< Configuration subtype */
        const char* psz_name; /**< Option name */
        const char* psz_text; /**< Short comment on the configuration option */
        const char* psz_longtext; /**< Long comment on the configuration option */

        module_value_t value; /**< Current value */
        module_value_t orig; /**< Default value */
        module_value_t min; /**< Minimum value (for scalars only) */
        module_value_t max; /**< Maximum value (for scalars only) */

        /* Values list */
        uint16_t list_count; /**< Choices count */
        union
        {
            const char** psz; /**< Table of possible string choices */
            const int* i; /**< Table of possible integer choices */
        } list; /**< Possible choices */
        const char** list_text; /**< Human-readable names for list values */
    };

    struct config_chain_t
    {
        config_chain_t* p_next;     /**< Pointer on the next config_chain_t element */

        char* psz_name;      /**< Option name */
        char* psz_value;     /**< Option value */
    };

    struct param {
        union {
            /*_Atomic*/ ::std::atomic<int64_t> i; /**< Current value (if integer or boolean) */
            /*_Atomic*/ ::std::atomic<float> f; /**< Current value (if floating point) */
            ::std::atomic<char*> /*_Atomic*/ str; /**< Current value (if character string) */
        } value;

        struct


            plugin_t* owner;
        unsigned char shortname; /**< Optional short option name */
    unsigned internal:1; /**< Hidden from preferences and help */
                     unsigned unsaved : 1; /**< Not stored in persistent configuration */
                     unsigned safe : 1; /**< Safe for untrusted provisioning (playlists) */
                     unsigned obsolete : 1; /**< Ignored for backward compatibility */
                     struct module_config_t item;
    };


    static struct
    {
        param** list;
        size_t count;
    } config = { NULL, 0 };

    inline int confcmp(const void* a, const void* b)
    {
        const param* const* ca = (const param* const*)a, * const* cb = (const param* const*)b;

        return strcmp((*ca)->item.psz_name, (*cb)->item.psz_name);
    }

    inline int confnamecmp(const void* key, const void* elem)
    {
        const struct param* const* conf = (const struct param* const*)elem;

        return strcmp((const char*)key, (*conf)->item.psz_name);
    }

    inline param* param_Find(const char* name)
    {
        param* const* p;

        assert(name != NULL);
        p = (param* const*)bsearch(name, config.list, config.count, sizeof(*p), confnamecmp);
        return (p != NULL) ? *p : NULL;
    }

    inline int64_t config_GetInt(const char* name)
    {
        const struct param* _param = param_Find(name);

        /* sanity checks */
        assert(_param != NULL);
        assert(IsConfigIntegerType(_param->item.i_type));

        return atomic_load_explicit(&_param->value.i, ::std::memory_order_relaxed);
    }

    inline float config_GetFloat(const char* name)
    {
        const param* _param = (const param*)param_Find(name);

        /* sanity checks */
        assert(_param != NULL);
        assert(IsConfigFloatType(_param->item.i_type));

        return atomic_load_explicit(&_param->value.f, ::std::memory_order_relaxed);
    }

    inline char* config_GetPsz(const char* name)
    {
        const param* _param = param_Find(name);
        char* str;

        /* sanity checks */
        assert(_param != NULL);
        assert(IsConfigStringType(_param->item.i_type));

        /* return a copy of the string */
        ::Threads::rcu_read_lock();
        str = atomic_load_explicit(&_param->value.str, ::std::memory_order_acquire);
        if (str != NULL)
            str = strdup(str);
        ::Threads::rcu_read_unlock();
        return str;
    }
} // namespace FFmpeg