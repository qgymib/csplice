#include <string.h>
#include "defines.h"
#include "luaext.h"

/**
 * @brief Host object.
 */
typedef struct csplice_object
{
    void             *obj; /**< Pointer to the object */
    csplice_lua_gc_fn gc;  /**< GC callback */
} csplice_object_t;

static int _csplice_object_gc(lua_State *L)
{
    csplice_object_t *o = lua_touserdata(L, 1);
    if (o->gc != NULL)
    {
        o->gc(o->obj);
        o->gc = NULL;
    }

    return 0;
}

int csplice_lua_strerror(lua_State *L, int errcode)
{
    char errbuf[256];
    strerror_r(errcode, errbuf, sizeof(errbuf));
    lua_pushstring(L, errbuf);
    return 1;
}

int csplice_lua_host(lua_State *L, void *obj, csplice_lua_gc_fn cb)
{
    csplice_object_t *o = lua_newuserdata(L, sizeof(csplice_object_t));
    o->obj = obj;
    o->gc = cb;

    static const luaL_Reg s_meta[] = {
        { "__gc", _csplice_object_gc },
        { NULL,   NULL               },
    };
    if (luaL_newmetatable(L, "__csplice_object") != 0)
    {
        luaL_setfuncs(L, s_meta, 0);
    }
    lua_setmetatable(L, -2);

    return 1;
}
