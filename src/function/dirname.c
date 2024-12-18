#include <string.h>
#include <stdlib.h>
#include "__init__.h"

static char* _dirname(char* s)
{
    size_t i;
    if (!s || !*s) return ".";
    i = strlen(s) - 1;
    for (; s[i] == '/'; i--) if (!i) return "/";
    for (; s[i] != '/'; i--) if (!i) return ".";
    for (; s[i] == '/'; i--) if (!i) return "/";
    s[i + 1] = 0;
    return s;
}

static int _csplice_function_dirname(lua_State* L)
{
    const char* path = luaL_checkstring(L, 1);
    char* cpy_path = strdup(path);

    char* name = _dirname(cpy_path);
    lua_pushstring(L, name);
    free(cpy_path);

    return 1;
}

const csplice_function_t csplice_lfunc_dirname = {
"dirname", _csplice_function_dirname
};
