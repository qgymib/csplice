#include "utils/str.h"
#include "utils/openfile.h"
#include "utils/defines.h"
#include "__init__.h"

typedef struct csplice_lfile
{
    csplice_file_t *handle; /**< The file handle. */
} csplice_lfile_t;

static int _file_close(lua_State *L)
{
    csplice_lfile_t *file = lua_touserdata(L, 1);
    if (file->handle != NULL)
    {
        file->handle->close(file->handle);
        file->handle = NULL;
    }
    return 0;
}

static int _file_gc(lua_State *L)
{
    return _file_close(L);
}

static int _file_read(lua_State *L)
{
    csplice_lfile_t *file = lua_touserdata(L, 1);
    if (file->handle == NULL)
    {
        return luaL_error(L, "file is closed");
    }

    int         errcode = 0;
    lua_Integer size = luaL_optinteger(L, 2, -1);

    char        data[256];
    luaL_Buffer buf;
    luaL_buffinit(L, &buf);

    size_t total_read_sz = 0;
    while (1)
    {
        size_t  remain_read_sz = size < 0 ? sizeof(data) : min(sizeof(data), size - total_read_sz);
        int64_t read_sz = file->handle->read(file->handle, data, remain_read_sz);
        if (read_sz < 0)
        { /* error */
            goto error;
        }

        if (read_sz == 0)
        { /* end of file. */
            break;
        }

        luaL_addlstring(&buf, data, read_sz);
        total_read_sz += read_sz;
    }

    luaL_pushresult(&buf);
    return 1;

error:
    csplice_lua_strerror(L, errcode);
    return luaL_error(L, "Cannot read file: %s", lua_tostring(L, -1));
}

static int _write_file(lua_State *L)
{
    csplice_lfile_t *file = lua_touserdata(L, 1);
    if (file->handle == NULL)
    {
        return luaL_error(L, "file is closed");
    }

    size_t      data_sz = 0;
    const char *data = luaL_checklstring(L, 2, &data_sz);

    int64_t write_sz = file->handle->write(file->handle, data, data_sz);
    if (write_sz != (int64_t)data_sz)
    {
        return luaL_error(L, "failed to write file");
    }

    return 0;
}

static int _csplice_function_openfile(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    const char *mode = luaL_optstring(L, 2, "r");

    csplice_lfile_t *file = lua_newuserdata(L, sizeof(csplice_file_t));

    int errcode = csplice_openfile(&file->handle, path, mode);
    if (errcode != 0)
    {
        csplice_lua_strerror(L, -errcode);
        return luaL_error(L, "failed to open file: %s: %s", path, lua_tostring(L, -1));
    }

    static const luaL_Reg s_file_meta[] = {
        { "__gc", _file_gc },
        { NULL,   NULL     },
    };
    static const luaL_Reg s_file_method[] = {
        { "close", _file_close },
        { "read",  _file_read  },
        { "write", _write_file },
        { NULL,    NULL        },
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

const csplice_function_t csplice_lfunc_openfile = { "openfile", _csplice_function_openfile };
