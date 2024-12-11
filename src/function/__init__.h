#ifndef __CSPICE_FUNCTION_INIT_H__
#define __CSPICE_FUNCTION_INIT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct csplice_function
{
    const char*     name;       /**< Name of the function. */
    lua_CFunction   addr;       /**< Address of the function. */
    const char*     document;   /**< Documentation of the function. */
} csplice_function_t;

/**
 * @brief Registered builtin functions
 * @{
 */
extern const csplice_function_t csplice_function_dirname;
extern const csplice_function_t csplice_function_openfile;
extern const csplice_function_t csplice_function_splitlines;
/**
 * @}
 */

/**
 * @brief Register builtin functions.
 * @param[in] L Lua VM.
 * @return Always 1.
 */
int luaopen_csplice(lua_State* L);

#ifdef __cplusplus
}
#endif
#endif
