#ifndef __CSPLICE_UTILS_STRERROR_H__
#define __CSPLICE_UTILS_STRERROR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>

/**
 * @brief Converts an error code to a string and push it to the stack.
 * @param[in] L Lua state.
 * @param[in] errcode Error code.
 * @return 1.
 */
int csplice_lua_strerror(lua_State* L, int errcode);

#ifdef __cplusplus
}
#endif
#endif
