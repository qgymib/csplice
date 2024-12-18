#include "__init__.h"

static int _csplice_function_writefile(lua_State *L)
{
    int sp = lua_gettop(L);
    luaL_checktype(L, 1, LUA_TSTRING);
    luaL_checktype(L, 2, LUA_TSTRING);

    /* Create a file object on top of stack, at SP+1. */
    csplice_get_function(L, csplice_lfunc_openfile.name); // sp+1
    lua_pushvalue(L, 1);                                  // sp+2
    lua_pushstring(L, "w+");                              // sp+3
    lua_call(L, 2, 1);                                    // sp+1

    /* Call file:write(). */
    lua_getfield(L, sp + 1, "write"); // sp+2
    lua_pushvalue(L, sp + 1);         // sp+3
    lua_pushvalue(L, 2);              // sp+4
    lua_call(L, 2, 0);                // sp+1

    /* Call file:close(). */
    lua_getfield(L, sp + 1, "close"); // sp+2
    lua_pushvalue(L, sp + 1);         // sp+3
    lua_call(L, 1, 0);                // sp+1

    /* No result. File object at SP+1 will be garbage-collected. */
    return 0;
}

const csplice_function_t csplice_lfunc_writefile = { "writefile", _csplice_function_writefile };
