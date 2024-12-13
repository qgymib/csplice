#ifndef __CSPLICE_UTILS_STR_H__
#define __CSPLICE_UTILS_STR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>

typedef struct csplice_string
{
    char  *str; /**< The string */
    size_t len; /**< The length of the string */
} csplice_string_t;

/**
 * @brief Converts a lua string to a C string.
 * @param[out] str Pointer to the string.
 * @param[in] L Lua state.
 * @param[in] idx Index of the string.
 */
void csplice_string_set_lstring(csplice_string_t *str, lua_State *L, int idx);

/**
 * @brief Sets a C string to a string.
 * @param[out] str String.
 * @param[in] s C string.
 */
void csplice_string_set_cstring(csplice_string_t* str, const char* s);

/**
 * @brief Sets a C string to a string.
 * @param[out] str String.
 * @param[in] s C string.
 * @param[in] len Length of the C string.
 */
void csplice_string_set_cstring_len(csplice_string_t* str, const char* s, size_t len);

/**
 * @brief Frees a string.
 * @param[in] str String.
 */
void csplice_string_free(csplice_string_t *str);

#ifdef __cplusplus
}
#endif
#endif
