#include "openfile.h"
#include "readfile.h"

int csplice_readfile(csplice_string_t *str, const char *path)
{
    csplice_file_t *file = NULL;
    int             err = csplice_openfile(&file, path, "r");
    if (err != 0)
    {
        return err;
    }

    char buf[256];
    while (1)
    {
        int64_t read_sz = file->read(file, buf, sizeof(buf));
        if (read_sz < 0)
        {
            file->release(file);
            return read_sz;
        }

        if (read_sz == 0)
        {
            break;
        }

        csplice_string_append(str, buf, read_sz);
    }

    file->release(file);
    return 0;
}
