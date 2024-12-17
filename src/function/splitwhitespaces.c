#include <string.h>
#include <stdlib.h>
#include "__init__.h"

static int _csplice_function_splitwhitespaces(lua_State* L)
{
    const char* raw = luaL_checkstring(L, 1);
    char* cpy = strdup(raw);

    char* str = cpy;
    const char* delim = " \t\r\n";
    char* saveptr = NULL;
    char* ret;

    int i;
    lua_newtable(L); // sp+1
    for (i = 1; (ret = strtok_r(str, delim, &saveptr)) != NULL; i++)
    {
        lua_pushstring(L, ret);
        lua_seti(L, -2, i);
        str = NULL;
    }

    free(cpy);
    return 1;
}

const csplice_function_t csplice_function_splitwhitespaces = {
"splitwhitespaces", _csplice_function_splitwhitespaces
};
