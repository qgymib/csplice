#include <lauxlib.h>
#include <lualib.h>
#include <string.h>
#include <stdlib.h>
#include "str.h"

void csplice_string_set_lstring(csplice_string_t *str, lua_State *L, int idx)
{
    size_t len = 0;
    const char *s = lua_tolstring(L, idx, &len);
    csplice_string_set_cstring_len(str, s, len);
}

void csplice_string_set_cstring(csplice_string_t* str, const char* s)
{
    size_t len = strlen(s);
    csplice_string_set_cstring_len(str, s, len);
}

void csplice_string_set_cstring_len(csplice_string_t* str, const char* s, size_t len)
{
    csplice_string_free(str);

    str->len = len;
    if ((str->str = malloc(str->len + 1)) == NULL)
    {
        abort();
    }
    memcpy(str->str, s, len);
    str->str[len] = '\0';
}

void csplice_string_free(csplice_string_t *str)
{
    if (str->str != NULL)
    {
        free(str->str);
        str->str = NULL;
    }
    str->len = 0;
}
