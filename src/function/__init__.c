#include "utils/defines.h"
#include "__init__.h"

/**
 * @brief Host object.
 */
typedef struct csplice_object
{
    void*           obj;    /**< Pointer to the object */
    csplice_host_gc gc;     /**< GC callback */
} csplice_object_t;

static const csplice_function_t* s_function_list[] = {
    &csplice_function_dirname,
    &csplice_function_openfile,
    &csplice_function_readfile,
    &csplice_function_splitlines,
};

static int _csplice_object_gc(lua_State* L)
{
    csplice_object_t* o = lua_touserdata(L, 1);
    if (o->gc != NULL)
    {
        o->gc(o->obj);
        o->gc = NULL;
    }

    return 0;
}

int csplice_openlibs(lua_State* L)
{
    lua_newtable(L);

    size_t i;
    for (i = 0; i < ARRAY_SIZE(s_function_list); i++)
    {
        lua_pushcfunction(L, s_function_list[i]->addr);
        lua_setfield(L, -2, s_function_list[i]->name);
    }

    lua_setglobal(L, CSPLICE_NAMESPACE);

    return 0;
}

int csplice_get_function(lua_State* L, const char* name)
{
    int sp = lua_gettop(L);

    int type = lua_getglobal(L, CSPLICE_NAMESPACE); // sp+1
    if (type != LUA_TTABLE)
    {
        return luaL_error(L, "namespace %s is not a table.", CSPLICE_NAMESPACE);
    }

    type = lua_getfield(L, sp + 1, name);   // sp+2
    if (type != LUA_TFUNCTION)
    {
        return luaL_error(L, "`%s.%s` is not a function.", CSPLICE_NAMESPACE, name);
    }

    lua_remove(L, sp + 1);                  // sp+1

    return 1;
}

int csplice_host(lua_State* L, void* obj, csplice_host_gc cb)
{
    csplice_object_t* o = lua_newuserdata(L, sizeof(csplice_object_t));
    o->obj = obj;
    o->gc = cb;

    static const luaL_Reg s_meta[] = {
        { "__gc",   _csplice_object_gc },
        { NULL,     NULL },
    };
    if (luaL_newmetatable(L, "__csplice_object") != 0)
    {
        luaL_setfuncs(L, s_meta, 0);
    }
    lua_setmetatable(L, -2);

    return 1;
}
