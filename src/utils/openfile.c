#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "utils/defines.h"
#include "openfile.h"

typedef struct csplice_file_impl
{
    csplice_file_t basis;
    intptr_t       handle;
} csplice_file_impl_t;

#if defined(_WIN32)

#include <windows.h>

static void _csplice_openfile_close(struct csplice_file *thiz)
{
    csplice_file_impl_t *impl = container_of(thiz, csplice_file_impl_t, basis);
    if ((HANDLE)impl->handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle((HANDLE)impl->handle);
        impl->handle = (intptr_t)INVALID_HANDLE_VALUE;
    }
}

static int64_t _csplice_openfile_read(struct csplice_file* thiz, void* buff, size_t size)
{
    csplice_file_impl_t *impl = container_of(thiz, csplice_file_impl_t, basis);
    if ((HANDLE)impl->handle == INVALID_HANDLE_VALUE)
    {
        return -EINVAL;
    }

    DWORD read_sz = 0;
    if (ReadFile((HANDLE)impl->handle, buff, (DWORD)size, &read_sz, NULL))
    {
        return read_sz;
    }

    return -EPIPE;
}

static int64_t _csplice_openfile_write(struct csplice_file* thiz, const void* buff, size_t size)
{
    csplice_file_impl_t *impl = container_of(thiz, csplice_file_impl_t, basis);
    if ((HANDLE)impl->handle == INVALID_HANDLE_VALUE)
    {
        return -EINVAL;
    }

    DWORD write_sz = 0;
    if (WriteFile((HANDLE)impl->handle, buff, (DWORD)size, &write_sz, NULL))
    {
        return write_sz;
    }
    return -EPIPE;
}

static int _fopen_s(intptr_t* h, const char* path, const char* mode)
{

}

#else

#include <unistd.h>
#include <fcntl.h>

typedef struct open_mode_map
{
    const char *mode;
    int         flag;
} open_mode_map_t;

static void _csplice_openfile_close(struct csplice_file *thiz)
{
    csplice_file_impl_t *impl = container_of(thiz, csplice_file_impl_t, basis);
    if (impl->handle >= 0)
    {
        close(impl->handle);
        impl->handle = -1;
    }
}

static int64_t _csplice_openfile_read(struct csplice_file *thiz, void *buff, size_t size)
{
    csplice_file_impl_t *impl = container_of(thiz, csplice_file_impl_t, basis);
    ssize_t              read_sz = read(impl->handle, buff, size);
    if (read_sz < 0)
    {
        return -errno;
    }
    return read_sz;
}

static int64_t _csplice_openfile_write(struct csplice_file *thiz, const void *buff, size_t size)
{
    csplice_file_impl_t *impl = container_of(thiz, csplice_file_impl_t, basis);
    ssize_t              write_sz = write(impl->handle, buff, size);
    if (write_sz < 0)
    {
        return -errno;
    }
    return write_sz;
}

static int _fopen_s(intptr_t* h, const char *path, const char *mode)
{
    static const open_mode_map_t open_map[] = {
        { "r",  O_RDONLY                    },
        { "r+", O_RDWR                      },
        { "w",  O_WRONLY | O_TRUNC          },
        { "w+", O_RDWR | O_CREAT | O_TRUNC  },
        { "a",  O_WRONLY | O_APPEND         },
        { "a+", O_RDWR | O_CREAT | O_APPEND },
    };

    int    open_flag = 0;
    size_t i;
    for (i = 0; i < ARRAY_SIZE(open_map); i++)
    {
        if (strcmp(open_map[i].mode, mode) == 0)
        {
            open_flag = open_map[i].flag;
            goto open_file;
        }
    }

    return -EINVAL;

open_file:
    int ret = open(path, open_flag, S_IRWXU);
    if (ret < 0)
    {
        return -errno;
    }

    *h = ret;
    return 0;
}

#endif

static void _csplice_openfile_release(struct csplice_file *thiz)
{
    thiz->close(thiz);
    csplice_file_impl_t *impl = container_of(thiz, csplice_file_impl_t, basis);
    free(impl);
}

int csplice_openfile(csplice_file_t **file, const char *path, const char *mode)
{
    csplice_file_impl_t *impl = malloc(sizeof(csplice_file_impl_t));

    impl->basis.release = _csplice_openfile_release;
    impl->basis.close = _csplice_openfile_close;
    impl->basis.read = _csplice_openfile_read;
    impl->basis.write = _csplice_openfile_write;

    int errcode;
    if ((errcode = _fopen_s(&impl->handle, path, mode)) < 0)
    {
        free(impl);
        return errcode;
    }

    *file = &impl->basis;
    return 0;
}
