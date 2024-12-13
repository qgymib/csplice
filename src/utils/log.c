#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include "defines.h"
#include "log.h"

static const char* s_log_level_name[] = {
    "D", "I", "W", "E",
};

static const char* _filename(const char* file)
{
    const char* pos = file;

    for (; *file; ++file)
    {
        if (*file == '\\' || *file == '/')
        {
            pos = file + 1;
        }
    }
    return pos;
}

void csplice_log(csplice_log_level_t level, const char* file, const char* func,
    int line, const char* fmt, ...)
{
    assert(level >= CSPICE_LOG_LEVEL_DEBUG && level <= CSPICE_LOG_LEVEL_ERROR);

    file = _filename(file);
    fprintf(stderr, "[%s][%s:%d][%s] ", s_log_level_name[level], file, line, func);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fprintf(stderr, "\n");
}
