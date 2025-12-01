#include "handler.h"
#include "logger.h"
#include "types.h"

#define MD_LOG_VERBOSE(format, ...) mdLogPrint(MD_LOG_LEVEL_VERBOSE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define MD_LOG_DEBUG(format, ...)	mdLogPrint(MD_LOG_LEVEL_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define MD_LOG_INFO(format, ...)	mdLogPrint(MD_LOG_LEVEL_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define MD_LOG_WARNING(format, ...) mdLogPrint(MD_LOG_LEVEL_WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define MD_LOG_ERROR(format, ...)	mdLogPrint(MD_LOG_LEVEL_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define MD_LOG_FATAL(format, ...)	mdLogPrint(MD_LOG_LEVEL_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)