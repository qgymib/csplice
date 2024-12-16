#ifndef __CSPLICE_UTILS_LUA_EXT_H__
#define __CSPLICE_UTILS_LUA_EXT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/**
 * @brief GC callback.
 * @param[in] obj Object.
 */
typedef void (*csplice_lua_gc_fn)(void *obj);

/**
 * @brief Converts an error code to a string and push it to the stack.
 * @param[in] L Lua state.
 * @param[in] errcode Error code.
 * @return 1.
 */
int csplice_lua_strerror(lua_State *L, int errcode);

/**
 * @brief Host object on top of Lua VM.
 * @param[in] L Lua VM.
 * @param[in] obj Object.
 * @param[in] cb GC callback.
 * @return Always 1.
 */
int csplice_lua_host(lua_State *L, void *obj, csplice_lua_gc_fn cb);

#ifdef __cplusplus
}
#endif
#endif
