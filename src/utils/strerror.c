#include <string.h>
#include "defines.h"
#include "strerror.h"

int csplice_lua_strerror(lua_State* L, int errcode)
{
    char errbuf[256];
    strerror_r(errcode, errbuf, sizeof(errbuf));
    lua_pushstring(L, errbuf);
    return 1;
}
