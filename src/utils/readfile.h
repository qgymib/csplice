#ifndef __CSPLICE_UTILS_READFILE_H__
#define __CSPLICE_UTILS_READFILE_H__

#include "str.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Reads a file into a string.
 * @param[out] str String.
 * @param[in] path Path to the file.
 */
int csplice_readfile(csplice_string_t *str, const char *path);

#ifdef __cplusplus
}
#endif
#endif
