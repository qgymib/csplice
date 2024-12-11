#include "utils/strerror.h"
#include "__init__.h"

typedef struct csplice_file
{
    FILE* handle;   /**< The file handle. */
} csplice_file_t;

static int _file_close(lua_State* L)
{
    csplice_file_t* file = lua_touserdata(L, 1);
    if (file->handle != NULL)
    {
        fclose(file->handle);
        file->handle = NULL;
    }
    return 0;
}

static int _file_gc(lua_State* L)
{
    return _file_close(L);
}

static int _file_read(lua_State* L)
{
    csplice_file_t* file = lua_touserdata(L, 1);
    if (file->handle == NULL)
    {
        return luaL_error(L, "file is closed");
    }

    int errcode = 0;
    lua_Integer size = luaL_optinteger(L, 2, -1);

    luaL_Buffer buf;
    luaL_buffinit(L, &buf);

    size_t total_read_sz = 0;
    while (!feof(file->handle))
    {
        if ((errcode = ferror(file->handle)) != 0)
        {
            goto error;
        }

        if (size > 0 && total_read_sz >= (size_t)size)
        {
            break;
        }

        char data[256];
        size_t read_sz = fread(data, 1, sizeof(data), file->handle);

        luaL_addlstring(&buf, data, read_sz);
        total_read_sz += read_sz;
    }

    luaL_pushresult(&buf);
    return 1;

error:
    csplice_lua_strerror(L, errcode);
    return luaL_error(L, "Cannot read file: %s", lua_tostring(L, -1));
}

static int _csplice_function_openfile(lua_State* L)
{
    const char* path = luaL_checkstring(L, 1);
    const char* mode = luaL_optstring(L, 2, "r");

    csplice_file_t* file = lua_newuserdata(L, sizeof(csplice_file_t));

    int errcode = fopen_s(&file->handle, path, mode);
    if (errcode != 0)
    {
        csplice_lua_strerror(L, errcode);
        return luaL_error(L, "failed to open file: %s: %s", path, lua_tostring(L, -1));
    }

    static const luaL_Reg s_file_meta[] = {
        { "__gc",   _file_gc },
        { NULL,     NULL },
    };
    static const luaL_Reg s_file_method[] = {
        { "close",  _file_close },
        { "read",   _file_read },
        { NULL,     NULL },
    };
    if (luaL_newmetatable(L, "__csplice_file") != 0)
    {
        luaL_setfuncs(L, s_file_meta, 0);

        luaL_newlib(L, s_file_method);
        lua_setfield(L, -2, "__index");
    }
    lua_setmetatable(L, -2);

    return 1;
}

const csplice_function_t csplice_function_openfile = {
"openfile", _csplice_function_openfile,
"[SYNOPSIS]\n"
"    file openfile(string path)\n"
"[DESCRIPTION]\n"
"    Opens a file.\n"
};
