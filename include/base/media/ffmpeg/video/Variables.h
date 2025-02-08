#pragma once 

#include "Object.h"

#define VAR_TYPE                0x00ff
#define VAR_CLASS               0x00f0
#define VAR_FLAGS               0xff00
#define VAR_VOID                0x0010
#define VAR_BOOL                0x0020
#define VAR_INTEGER             0x0030
#define VAR_STRING              0x0040
#define VAR_FLOAT               0x0050
#define VAR_ADDRESS             0x0070
#define VAR_COORDS              0x00A0
#define VAR_HASCHOICE           0x0100
#define VAR_ISCOMMAND           0x2000
#define VAR_DOINHERIT           0x8000
#define VAR_SETSTEP             0x0012
#define VAR_SETVALUE            0x0013
#define VAR_SETTEXT             0x0014
#define VAR_GETTEXT             0x0015
#define VAR_GETMIN              0x0016
#define VAR_GETMAX              0x0017
#define VAR_GETSTEP             0x0018
#define VAR_ADDCHOICE           0x0020
#define VAR_DELCHOICE           0x0021
#define VAR_CLEARCHOICES        0x0022
#define VAR_GETCHOICES          0x0024
#define VAR_CHOICESCOUNT        0x0026
#define VAR_SETMINMAX           0x0027


namespace FFmpeg {
    typedef int (*variable_ops_cmp) (value_t, value_t);
    typedef void (*variable_ops_dup) (value_t*);
    typedef void (*variable_ops_free) (value_t*);

    struct variable_ops_t
    {
        variable_ops_cmp pf_cmp;
        variable_ops_dup pf_dup;
        variable_ops_free pf_free;
    };

    typedef int (*callback_t) (object_t*,      /* variable's object */
        char const*,            /* variable name */
        value_t,                 /* old value */
        value_t,                 /* new value */
        void*);                /* callback data */

    /*****************************************************************************
     * List callbacks: called when elements are added/removed from the list
     *****************************************************************************/
    typedef int (*list_callback_t) (object_t*,      /* variable's object */
        char const*,            /* variable name */
        int,                  /* VAR_* action */
        value_t*,      /* new/deleted value  */
        void*);                 /* callback data */


    struct callback_entry_t
    {
        struct callback_entry_t* next;
        union
        {
            callback_t       pf_value_callback;
            list_callback_t  pf_list_callback;
            void* p_callback;
        };
        void* p_data;
    };

    struct variable_t
    {
        char* psz_name; /**< The variable unique name (must be first) */

        /** The variable's exported value */
        value_t  val;

        /** The variable display name, mainly for use by the interfaces */
        char* psz_text;

        const variable_ops_t* ops;

        int          i_type;   /**< The type of the variable */
        unsigned     i_usage;  /**< Reference count */

        /** If the variable has min/max/step values */
        value_t  min, max, step;

        /** List of choices */
        value_t* choices;
        /** List of friendly names for the choices */
        char** choices_text;
        size_t       choices_count;

        /** Set to TRUE if the variable is in a callback */
        bool   b_incallback;

        /** Registered value callbacks */
        callback_entry_t* value_callbacks;
        /** Registered list callbacks */
        callback_entry_t* list_callbacks;

        ::Threads::cond_t   wait;
    };


    int CmpBool(value_t v, value_t w);
    int CmpInt(value_t v, value_t w);
  
    int CmpString(value_t v, value_t w);

    int CmpFloat(value_t v, value_t w);
    int CmpAddress(value_t v, value_t w);

    void DupDummy(value_t* p_val);
    void DupString(value_t* p_val);

    void FreeDummy(value_t* p_val);
    void FreeString(value_t* p_val);

    static const variable_ops_t
        void_ops =      {   NULL,       DupDummy,  FreeDummy, },
        addr_ops =      {   CmpAddress, DupDummy,  FreeDummy, },
        bool_ops =      {   CmpBool,    DupDummy,  FreeDummy, },
        float_ops =     {   CmpFloat,   DupDummy,  FreeDummy, },
        int_ops =       {   CmpInt,     DupDummy,  FreeDummy, },
        string_ops =    {   CmpString,  DupString, FreeString, },
        coords_ops =    {   NULL,       DupDummy,  FreeDummy, };

    int varcmp(const void* a, const void* b);
    variable_t* LookupVar(object_t* obj, const char* psz_name);

    variable_t* LookupVariable(object_t* obj, const char* psz_name);

    int var_Inherit(object_t* p_this, const char* psz_name, int i_type,
        value_t* p_val);


    void Destroy(variable_t* p_var);

    void CleanupVar(void* var);
    void var_DestroyAll(object_t* obj);

    int (var_Create)(object_t* p_this, const char* psz_name, int i_type);

    int64_t var_GetInteger(object_t* p_obj, const char* psz_name);
    int64_t var_CreateGetInteger(object_t* p_obj, const char* psz_name);

    int var_GetChecked(object_t* p_this, const char* psz_name,
        int expected_type, value_t* p_val);
}