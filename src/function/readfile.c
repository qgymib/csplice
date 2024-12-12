#include "__init__.h"

static int _csplice_function_readfile(lua_State* L)
{
    int sp = lua_gettop(L);
    const char* path = luaL_checkstring(L, 1); (void)path;

    /* Create a file object on top of stack, at SP+1. */
    csplice_get_function(L, "openfile");                    // sp+1
    lua_pushvalue(L, 1);                                    // sp+2
    lua_call(L, 1, 1);                                      // sp+1

    /* Call file:read(), result store at SP+2. */
    lua_getfield(L, sp + 1, "read");                        // sp+2
    lua_pushvalue(L, sp + 1);                               // sp+3
    lua_call(L, 1, 1);                                      // sp+2

    /* Call file:close(). */
    lua_getfield(L, sp + 1, "close");                       // sp+3
    lua_pushvalue(L, sp + 1);                               // sp+4
    lua_call(L, 1, 0);                                      // sp+2

    /* Use result at SP+2. File object at SP+1 will be garbage-collected. */
    return 1;
}

const csplice_function_t csplice_function_readfile = {
"readfile", _csplice_function_readfile, ""
};
