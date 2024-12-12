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
#include <cJSON.h>
///////////////////////////////////////////////////////////////////////////////

#include "function/__init__.h"
#include "utils/codenode.h"
#include "utils/defines.h"

/**
 * @brief Version string.
 */
#define CSPLICE_VERSION_STRING                                                                                         \
    STRINGIFY(CSPLICE_VERSION_MAJOR) "." STRINGIFY(CSPLICE_VERSION_MINOR) "." STRINGIFY(CSPLICE_VERSION_PATCH)

typedef struct csplice_ctx
{
    lua_State *L; /**< Lua VM. */

    char *ifile; /**< Input file. */
    char *ofile; /**< Output file. */

    code_node_t *root; /**< Root node of code tree. */
} csplice_ctx_t;

/**
 * @brief Global context.
 */
static csplice_ctx_t *_G = NULL;

// clang-format off
/**
 * @brief Help message.
 */
static const char* s_help =
CSPLICE_NAMESPACE " v" CSPLICE_VERSION_STRING " - Merge multiple C source files into a single file.\n"
"Usage:\n"
"  " CSPLICE_NAMESPACE " [OPTIONS] -o <ofile> -i <ifile>\n"
"Options:\n"
"  -i <ifile>\n"
"    Input file.\n"
"  -o <ofile>\n"
"    Output file.\n"
"  -h, --help\n"
"    Print this help message and exit.\n"
"  -v, --version\n"
"    Print version information and exit.\n"
;
// clang-format on

/**
 * @brief Process argc and argv.
 * @param[in] L Lua VM.
 */
static void _process_argc(lua_State *L)
{
    int    argc = (int)lua_tointeger(L, 1);
    char **argv = (char **)lua_touserdata(L, 2);

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

        if (strcmp(argv[i], "-i") == 0)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "missing argument to `-i`.\n");
                exit(EXIT_FAILURE);
            }
            if (_G->ifile != NULL)
            {
                free(_G->ifile);
            }
            _G->ifile = strdup(argv[i + 1]);
            continue;
        }

        if (strcmp(argv[i], "-o") == 0)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "missing argument to `-o`.\n");
                exit(EXIT_FAILURE);
            }
            if (_G->ofile != NULL)
            {
                free(_G->ofile);
            }
            _G->ofile = strdup(argv[i + 1]);
            continue;
        }
    }

    if (_G->ifile == NULL)
    {
        fprintf(stderr, "missing input file.\n");
        exit(EXIT_FAILURE);
    }
}

static int _build_code_tree(lua_State *L)
{
    int sp = lua_gettop(L);

    /* Read input file at sp+1. */
    csplice_get_function(L, "readfile");
    lua_pushstring(L, _G->ifile);
    lua_call(L, 1, 1);

    /* Parse input as json at sp+2. */
    cJSON *json = cJSON_Parse(lua_tostring(L, -1));
    if (json == NULL)
    {
        return luaL_error(L, "failed to parse input file.");
    }
    csplice_host(L, json, (csplice_host_gc)cJSON_Delete);

    if (json->type != cJSON_Array)
    {
        return luaL_error(L, "compile_commands.json is not an array.");
    }

    int i;
    int node_sz = cJSON_GetArraySize(json);
    for (i = 0; i < node_sz; i++)
    {
        cJSON* obj = cJSON_GetArrayItem(json, i);
        cJSON* j_file = cJSON_GetObjectItem(obj, "file");
        if (j_file == NULL)
        {
            return luaL_error(L, "compile_commands.json is not valid.");
        }

        const char* v_file = cJSON_GetStringValue(j_file);
        code_node_append_file(_G->root, v_file);
    }

    /* Restore stack. */
    lua_settop(L, sp);
    return 0;
}

/**
 * @brief Application entry point.
 * @param[in] L Lua VM.
 * @return Always 0.
 */
static int _pmain(lua_State *L)
{
    luaL_openlibs(L);
    csplice_openlibs(L);

    /* Process argc and argv. */
    _process_argc(L);

    _build_code_tree(L);

    return 0;
}

/**
 * @brief Lua error traceback handler
 * @param[in] L Lua VM.
 * @return Always 1.
 */
static int _msg_handler(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (msg == NULL)
    {                                            /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") && /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)      /* that produces a string? */
        {
            return 1; /* that is the message */
        }

        msg = lua_pushfstring(L, "(error object is a %s value)", luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1); /* append a standard traceback */
    return 1;                     /* return the traceback */
}

/**
 * @brief Global application exit handler.
 */
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

    if (_G->ifile != NULL)
    {
        free(_G->ifile);
        _G->ifile = NULL;
    }
    if (_G->ofile != NULL)
    {
        free(_G->ofile);
        _G->ofile = NULL;
    }
    if (_G->root != NULL)
    {
        code_node_delete(_G->root);
        _G->root = NULL;
    }

    free(_G);
    _G = NULL;
}

int main(int argc, char *argv[])
{
    atexit(_at_exit);

    if ((_G = calloc(1, sizeof(*_G))) == NULL)
    {
        fprintf(stderr, "out of memory.\n");
        return EXIT_FAILURE;
    }

    if ((_G->root = calloc(1, sizeof(code_node_t))) == NULL)
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
