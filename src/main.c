/**
 * @mainpage csplice
 * 
 * @section CSPLICE_OVERVIEW Overview
 * 
 * csplice is a tool to merge multiple C source files into a single file.
 * 
 * @section CSPLICE_TEMPLATE Template
 * 
 * @code
 * {
 *   "ifiles": [
 *     {
 *       "file": "file1.c",
 *       "command": "c:include"
 *     },
 *     {
 *       "file": "file2.c",
 *       "code": "lua code"
 *     }
 *   ]
 * }
 * @endcode
 * 
 * @section CSPLICE_USAGE Usage
 * 
 * + Usage
 * 
 * @section CSPLICE_COMMANDS Commands
 * 
 * + `c:include`: Include a file.
 * 
 */

///////////////////////////////////////////////////////////////////////////////
// Version
///////////////////////////////////////////////////////////////////////////////
#define CSPLICE_VERSION_MAJOR 0
#define CSPLICE_VERSION_MINOR 1
#define CSPLICE_VERSION_PATCH 0
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// System header
///////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <string.h>
///////////////////////////////////////////////////////////////////////////////

#include "function/__init__.h"
#include "utils/defines.h"

/**
 * @brief Version string.
 */
#define CSPLICE_VERSION_STRING \
    STRINGIFY(CSPLICE_VERSION_MAJOR) "." STRINGIFY(CSPLICE_VERSION_MINOR) "." STRINGIFY(CSPLICE_VERSION_PATCH)

typedef struct csplice_ctx
{
    lua_State* L;   /**< Lua VM. */
} csplice_ctx_t;

/**
 * @brief Global context.
 */
static csplice_ctx_t* _G = NULL;

/**
 * @brief Help message.
 */
static const char* s_help =
"csplice v" CSPLICE_VERSION_STRING " - Merge multiple C source files into a single file.\n"
"Usage:\n"
"  csplice [OPTIONS] <ofile> <ifile>\n"
"Options:\n"
"  -h, --help\n"
"    Print this help message and exit.\n"
"  -v, --version\n"
"    Print version information and exit.\n"
;

/**
 * @brief Process argc and argv.
 * @param[in] L Lua VM.
 */
static void _process_argc(lua_State* L)
{
    int argc = (int)lua_tointeger(L, 1);
    char** argv = (char**)lua_touserdata(L, 2);

    int i;
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("%s\n", s_help);
            exit(EXIT_SUCCESS);
        }

        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            printf("%s\n", CSPLICE_VERSION_STRING);
            exit(EXIT_SUCCESS);
        }
    }
}

static int _pmain(lua_State* L)
{
    luaL_openlibs(L);

    /* Register csplice functions. */
    luaopen_csplice(L);
    lua_setglobal(L, "csplice");

    /* Process argc and argv. */
    _process_argc(L);

    return 0;
}

/**
 * @brief Lua error traceback handler
 * @param[in] L Lua VM.
 * @return Always 1.
 */
static int _msg_handler(lua_State* L)
{
    const char* msg = lua_tostring(L, 1);
    if (msg == NULL)
    {  /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
        {
            return 1;  /* that is the message */
        }

        msg = lua_pushfstring(L, "(error object is a %s value)",
            luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
    return 1;  /* return the traceback */
}

static void _at_exit(void)
{
    if (_G == NULL)
    {
        return;
    }

    if (_G->L != NULL)
    {
        lua_close(_G->L);
        _G->L = NULL;
    }

    free(_G);
    _G = NULL;
}

int main(int argc, char* argv[])
{
    atexit(_at_exit);

    if ((_G = malloc(sizeof(*_G))) == NULL)
    {
        fprintf(stderr, "out of memory.\n");
        return EXIT_FAILURE;
    }

    if ((_G->L = luaL_newstate()) == NULL)
    {
        fprintf(stderr, "luaL_newstate() failed.\n");
        return EXIT_FAILURE;
    }

    lua_pushcfunction(_G->L, _msg_handler);
    lua_pushcfunction(_G->L, _pmain);
    lua_pushinteger(_G->L, argc);
    lua_pushlightuserdata(_G->L, argv);

    if (lua_pcall(_G->L, 2, 0, 1) != LUA_OK)
    {
        fprintf(stderr, "%s\n", lua_tostring(_G->L, -1));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
