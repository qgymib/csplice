#include <string.h>
#include "function/__init__.h"
#include "utils/json.h"
#include "json_compilation_database.h"

static int _compilation_database_parse_node(lua_State *L, pass_node_t *node, cJSON *obj)
{
    int sp = lua_gettop(L);
    {
        cJSON *j_file = cJSON_GetObjectItem(obj, "file");
        if (j_file == NULL)
        {
            return luaL_error(L, "compile_commands.json is not valid.");
        }
        const char *path = cJSON_GetStringValue(j_file);
        pass_node_set_file(node, path);
    }
    {
        cJSON *j_cmd = cJSON_GetObjectItem(obj, "command");
        if (j_cmd != NULL)
        {
            csplice_get_function(L, csplice_lfunc_splitwhitespaces.name); // sp+1
            lua_pushstring(L, cJSON_GetStringValue(j_cmd));               // sp+2
            lua_call(L, 1, 1);                                            // sp+1

            lua_pushnil(L); // sp+2
            while (lua_next(L, sp + 1))
            { /* key:sp+2, val:sp+3 */
                const char *val = lua_tostring(L, sp + 3);
                if (strncmp(val, "-I", 2) == 0)
                {
                    pass_node_file_append_search_path(node, val + 2, 0);
                }
                lua_pop(L, 1); // sp+2
            }

            lua_pop(L, 1); // sp+1
        }
    }

    lua_settop(L, sp);
    return 0;
}

int csplice_build_tree_from_compilation_database(lua_State *L, const char *path, pass_node_t *root)
{
    /* Parse input file at sp+1. */
    cJSON *json = csplice_json_parse_file(L, path);
    if (json->type != cJSON_Array)
    {
        return luaL_error(L, "compile_commands.json is not an array.");
    }

    int i;
    int node_sz = cJSON_GetArraySize(json);
    for (i = 0; i < node_sz; i++)
    {
        pass_node_t *node = pass_node_new();
        pass_node_insert(root, node, (size_t)-1);

        cJSON *obj = cJSON_GetArrayItem(json, i);
        _compilation_database_parse_node(L, node, obj);
    }

    lua_pop(L, 1);
    return 0;
}
