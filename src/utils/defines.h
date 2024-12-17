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

/**
 * @def offsetof
 * @brief The offset of \p member in \p type.
 */
#if !defined(offsetof)
#   define offsetof(type, member)    ((size_t)&(((type*)0)->member))
#endif

/**
 * @def container_of
 * @brief cast a member of a structure out to the containing structure.
 */
#if !defined(container_of)
#if defined(__GNUC__) || defined(__clang__)
#   define container_of(ptr, type, member)   \
        ({ \
            const typeof(((type *)0)->member)*__mptr = (ptr); \
            (type *)((char *)__mptr - offsetof(type, member)); \
        })
#else
#   define container_of(ptr, type, member)   \
        ((type *) ((char *) (ptr) - offsetof(type, member)))
#endif
#endif

#ifndef min
#define min(x, y)   ((x) < (y) ? (x) : (y))
#endif

#if defined(_MSC_VER)
#define strerror_r(errcode, buf, len)   strerror_s(buf,len, errcode)
#define strdup(s)                       _strdup(s)
#endif

#endif
