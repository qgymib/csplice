#include "utils/defines.h"
#include "__init__.h"

static const csplice_function_t* s_function_list[] = {
    &csplice_function_dirname,
    &csplice_function_openfile,
    &csplice_function_splitlines,
};

int luaopen_csplice(lua_State* L)
{
    lua_newtable(L);

    size_t i;
    for (i = 0; i < ARRAY_SIZE(s_function_list); i++)
    {
        lua_pushcfunction(L, s_function_list[i]->addr);
        lua_setfield(L, -2, s_function_list[i]->name);
    }

    return 1;
}
