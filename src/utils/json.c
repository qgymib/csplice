#include "readfile.h"
#include "json.h"

cJSON *csplice_json_parse_file(lua_State *L, const char *path)
{
    csplice_string_t data = CSPLICE_STRING_INIT;
    int              ret = csplice_readfile(&data, path);
    if (ret != 0)
    {
        csplice_lua_strerror(L, -ret);
        lua_pushfstring(L, "failed to read file: %s", lua_tostring(L, -1));
        lua_error(L);
        return NULL;
    }

    cJSON *json = cJSON_ParseWithLength(data.str, data.len);
    csplice_string_free(&data);

    if (json == NULL)
    {
        lua_pushfstring(L, "failed to parse json: %s", cJSON_GetErrorPtr());
        lua_error(L);
        return NULL;
    }

    csplice_lua_host(L, json, (csplice_lua_gc_fn)cJSON_Delete);
    return json;
}
