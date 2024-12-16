#ifndef __CSPLICE_UTILS_LOG_H__
#define __CSPLICE_UTILS_LOG_H__

// clang-format off

/**
 * @brief Debug logging.
 * @param[in] fmt Format string.
 * @param[in] ... Variable arguments.
 */
#define LOG_D(fmt, ...) \
    csplice_log(CSPICE_LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @brief Info logging.
 * @param[in] fmt Format string.
 * @param[in] ... Variable arguments.
 */
#define LOG_I(fmt, ...) \
    csplice_log(CSPICE_LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @brief Warn logging.
 * @param[in] fmt Format string.
 * @param[in] ... Variable arguments.
 */
#define LOG_W(fmt, ...) \
    csplice_log(CSPICE_LOG_LEVEL_WARN, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @brief Error logging.
 * @param[in] fmt Format string.
 * @param[in] ... Variable arguments.
 */
#define LOG_E(fmt, ...) \
    csplice_log(CSPICE_LOG_LEVEL_ERROR, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

// clang-format on

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Logging level.
 */
typedef enum csplice_log_level
{
    CSPICE_LOG_LEVEL_DEBUG = 0,
    CSPICE_LOG_LEVEL_INFO,
    CSPICE_LOG_LEVEL_WARN,
    CSPICE_LOG_LEVEL_ERROR,
} csplice_log_level_t;

/**
 * @brief Logging function.
 * @param[in] level Logging level.
 * @param[in] file File name.
 * @param[in] func Function name.
 * @param[in] line Line number.
 * @param[in] fmt Format string.
 * @param[in] ... Variable arguments.
 */
void csplice_log(csplice_log_level_t level, const char *file, const char *func,
    int line, const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
