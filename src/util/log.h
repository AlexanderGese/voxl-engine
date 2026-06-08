#ifndef UTIL_LOG_H
#define UTIL_LOG_H

// simple log. just prints to stderr. not thread safe (who cares)

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} log_level_t;

void log_set_level(log_level_t lvl);
void log_msg(log_level_t lvl, const char *file, int line, const char *fmt, ...);

#define LOGD(...) log_msg(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOGI(...) log_msg(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define LOGW(...) log_msg(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define LOGE(...) log_msg(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOGF(...) log_msg(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif
