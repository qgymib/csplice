#ifndef __CSPLICE_UTILS_DEFINES_H__
#define __CSPLICE_UTILS_DEFINES_H__

/**
 * @brief Array size.
 * @param[in] x array.
 */
#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(x[0]))

/**
 * @brief Stringify macro.
 * @{
 */
#define STRINGIFY(x)    STRINGIFY2(x)
#define STRINGIFY2(x)   #x
/**
 * @}
 */

#endif
