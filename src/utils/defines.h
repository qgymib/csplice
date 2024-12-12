#ifndef __CSPLICE_UTILS_DEFINES_H__
#define __CSPLICE_UTILS_DEFINES_H__

/**
 * @brief Namespace.
 */
#define CSPLICE_NAMESPACE   "csplice"

/**
 * @brief Array size.
 * @param[in] x array.
 */
#define ARRAY_SIZE(x)       (sizeof(x) / sizeof(x[0]))

/**
 * @brief Stringify macro.
 * @{
 */
#define STRINGIFY(x)        STRINGIFY2(x)
#define STRINGIFY2(x)       #x
/**
 * @}
 */

#if defined(_MSC_VER)
#define strerror_r(errcode, buf, len)   strerror_s(buf,len, errcode)
#define strdup(s)                       _strdup(s)
#endif

#endif
