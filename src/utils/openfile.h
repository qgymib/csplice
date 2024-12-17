#ifndef __CSPLICE_UTILS_OPENFILE_H__
#define __CSPLICE_UTILS_OPENFILE_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct csplice_file
{
    /**
     * @brief Delete this object.
     * @param[in] thiz Object.
     */
    void (*release)(struct csplice_file* thiz);

    /**
     * @brief Close file.
     * @param[in] thiz Object.
     */
    void (*close)(struct csplice_file* thiz);

    /**
     * @brief Read data from file.
     * @param[in] thiz Object.
     * @param[out] buff Buffer.
     * @param[in] size Size of buffer.
     * @return Number of bytes read.
     */
    int64_t (*read)(struct csplice_file* thiz, void* buff, size_t size);

    /**
     * @brief Write data to file.
     * @param[in] thiz Object.
     * @param[in] buff Buffer.
     * @param[in] size Size of buffer.
     * @return Number of bytes written.
     */
    int64_t (*write)(struct csplice_file* thiz, const void* buff, size_t size);
} csplice_file_t;

/**
 * @brief Open file.
 * @param[out] file Object.
 * @param[in] path Path to file.
 * @param[in] mode Mode.
 * @return 0 on success.
 */
int csplice_openfile(csplice_file_t** file, const char* path, const char* mode);

#ifdef __cplusplus
}
#endif
#endif
