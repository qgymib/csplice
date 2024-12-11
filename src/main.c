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

/**
 * @brief Help message.
 */
static const char* s_help =
"csplice v" CSPLICE_VERSION_STRING " - Merge multiple C source files into a single file.\n"
"Usage:\n"
"  csplice [OPTIONS] <ofile> <ifiles>\n"
"Options:\n"
"  -h, --help\n"
"    Print this help message and exit.\n"
"  -v, --version\n"
"    Print version information and exit.\n"
;

/**
 * @brief Process argc and argv.
 * @param[in] L Lua VM.
 * @return 1 if need to exit, 0 otherwise.
 */
static int _process_argc(lua_State* L)
{
    int argc = (int)lua_tointeger(L, 1);
    char** argv = (char**)lua_touserdata(L, 2);

    int i;
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("%s\n", s_help);
            return 1;
        }

        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            printf("%s\n", CSPLICE_VERSION_STRING);
            return 1;
        }
    }

    return 0;
}

static int _pmain(lua_State* L)
{
    luaL_openlibs(L);

    /* Register csplice functions. */
    luaopen_csplice(L);
    lua_setglobal(L, "csplice");

    if (_process_argc(L))
    {
        goto finish;
    }

finish:
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

int main(int argc, char* argv[])
{
    lua_State* L = luaL_newstate();
    lua_pushcfunction(L, _msg_handler);
    lua_pushcfunction(L, _pmain);
    lua_pushinteger(L, argc);
    lua_pushlightuserdata(L, argv);

    int ret = EXIT_SUCCESS;
    if (lua_pcall(L, 2, 0, 1) != LUA_OK)
    {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        ret = EXIT_FAILURE;
    }

    lua_close(L);
    return ret;
}
