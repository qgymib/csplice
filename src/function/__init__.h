#ifndef __CSPICE_FUNCTION_INIT_H__
#define __CSPICE_FUNCTION_INIT_H__

#include "utils/luaext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function information.
 */
typedef struct csplice_function
{
    const char*     name;       /**< Name of the function. */
    lua_CFunction   addr;       /**< Address of the function. */
} csplice_function_t;

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
 * @defgroup CSPLICE_FUNCTIONS Builtin functions
 * @csplice have builtin lua functions in the global namespace `csplice`.
 * @{
 */

/**
 * @defgroup CSPLICE_FUNCTION_DIRNAME dirname()
 * 
 * @section SYNOPSIS
 * 
 * \code
 * string dirname(string)
 * \endcode
 * 
 * @section DESCRIPTION
 * 
 * Returns the directory part of the path, AKA the string up to, but not including,
 * the final '/'.
 * 
 * + If path does not contain a slash, dirname() returns the string ".".
 * + If path is  the  string  "/", then dirname() return the string "/".
 * + If path is an empty string, then both dirname() return the string ".".
 * 
 * @{
 * @cond
 */
extern const csplice_function_t csplice_function_dirname;
/**
 * @endcond
 * @}
 */

/**
 * @defgroup CSPLICE_FUNCTION_OPENFILE openfile()
 * 
 * @section SYNOPSIS
 * 
 * \code
 * file openfile(string path[, string mode])
 * \endcode
 * 
 * @section DESCRIPTION
 * 
 * Open a file. By default, the file is opened in `rb` mode.
 * 
 * The open mode allow following options:
 * + `r`: Open file for reading. The stream is positioned at the beginning
 *   of the file.
 * + `r+`: Open for reading and writing. The stream is positioned at the beginning
 *   of the file.
 * + `w`: Truncate file to zero length or create text file for writing. The stream
 *    is positioned at the beginning of the file.
 * + `w+`: Open for reading and writing. The file is created if it does not exist,
 *   otherwise it is truncated. The stream is positioned at the beginning of the file.
 * + `a`: Open for appending (writing at end of file). The file is created if it
 *   does not exist. The stream is positioned at the end of the file.
 * + `a+`: Open for reading and appending (writing at end of file). The file is
 *   created if it does not exist. Output is always appended to the end of the file.
 * 
 * The file always open in binary mode.
 * 
 * The returned object has following methods:
 * + `close()`: Close the file.
 * + `write(string data)`: Write data to the file.
 * + `string read([integer size])`: Read data from the file. If no parameter set, 
 *   read the whole file.
 * 
 * @{
 * @cond
 */
extern const csplice_function_t csplice_function_openfile;
/**
 * @endcond
 * @}
 */

/**
 * @defgroup CSPLICE_FUNCTION_READFILE readfile()
 * 
 * @section SYNOPSIS
 * 
 * \code
 * string readfile(string path)
 * \endcode
 * 
 * @section DESCRIPTION
 * 
 * Reads a file and return its content as a string.
 * 
 * @{
 * @cond
 */
extern const csplice_function_t csplice_function_readfile;
/**
 * @endcond
 * @}
 */

/**
 * @defgroup CSPLICE_FUNCTION_SPLITLINES splitlines()
 * 
 * @section SYNOPSIS
 * 
 * \code
 * string[] splitlines(string s)
 * \endcode
 * 
 * @section DESCRIPTION
 * 
 * Splits string into a list of lines. Empty line is not ignored.
 * 
 * @{
 * @cond
 */
extern const csplice_function_t csplice_function_splitlines;
/**
 * @endcond
 * @}
 */

/**
 * @defgroup CSPLICE_FUNCTION_WRITEFILE writefile()
 * 
 * @section SYNOPSIS
 * 
 * \code
 * void writefile(string path, string data)
 * \endcode
 * 
 * @section DESCRIPTION
 * 
 * Writes a string to a file.
 * 
 * @{
 * @cond
 */
extern const csplice_function_t csplice_function_writefile;
/**
 * @endcond
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif
