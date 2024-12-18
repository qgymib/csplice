#ifndef __CSPLICE_UTILS_JSON_H__
#define __CSPLICE_UTILS_JSON_H__

#include <cJSON.h>
#include "luaext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse a JSON file as cJSON, push it on top of \p L stack, and
 *   return the result.
 * @param[in] L Lua state.
 * @param[in] path Path to the JSON file.
 * @return cJSON object.
 */
cJSON *csplice_json_parse_file(lua_State *L, const char *path);

#ifdef __cplusplus
}
#endif
#endif
