#ifndef __CSPICE_FUNCTION_INIT_H__
#define __CSPICE_FUNCTION_INIT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/**
 * @brief Function information.
 */
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
extern const csplice_function_t csplice_function_readfile;
extern const csplice_function_t csplice_function_splitlines;
/**
 * @}
 */

/**
 * @brief GC callback.
 * @param[in] obj Object.
 */
typedef void (*csplice_host_gc)(void* obj);

/**
 * @brief Register builtin functions.
 * @param[in] L Lua VM.
 * @return Always 0.
 */
int csplice_openlibs(lua_State* L);

/**
 * @brief Get the function from the global namespace and push it on the stack.
 * @param[in] L Lua VM.
 * @param[in] name Name of the function.
 * @return Always 1.
 */
int csplice_get_function(lua_State* L, const char* name);

/**
 * @brief Host object on Lua VM.
 * @param[in] L Lua VM.
 * @param[in] obj Object.
 * @param[in] cb GC callback.
 * @return Always 1.
 */
int csplice_host(lua_State* L, void* obj, csplice_host_gc cb);

#ifdef __cplusplus
}
#endif
#endif
