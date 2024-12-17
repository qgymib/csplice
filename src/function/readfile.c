#include "utils/readfile.h"
#include "__init__.h"

static int _csplice_function_readfile(lua_State* L)
{
    const char* path = luaL_checkstring(L, 1);

    csplice_string_t data = CSPLICE_STRING_INIT;
    int err = csplice_readfile(&data, path);
    if (err != 0)
    {
        csplice_lua_strerror(L, -err);
        return luaL_error(L, "failed to read file: %s", lua_tostring(L, -1));
    }

    lua_pushlstring(L, data.str, data.len);
    csplice_string_free(&data);

    return 1;
}

const csplice_function_t csplice_function_readfile = {
"readfile", _csplice_function_readfile
};
