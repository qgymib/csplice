#include <string.h>
#include "strerror.h"

#if defined(_MSC_VER)
#define strerror_r(errcode, buf, len)   strerror_s(buf,len, errcode)
#endif

int csplice_lua_strerror(lua_State* L, int errcode)
{
    char errbuf[256];
    strerror_r(errcode, errbuf, sizeof(errbuf));
    lua_pushstring(L, errbuf);
    return 1;
}
