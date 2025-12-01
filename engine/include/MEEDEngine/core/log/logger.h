#pragma once

#if __cplusplus
extern "C" {
#endif

#include "MEEDEngine/platforms/common.h"
#include "handler.h"
#include "types.h"

/**
 * Initializes the logging system.
 * Must be called before using any logging functions.
 */
void mdLogInitialize(enum MdLogLevel level);

/**
 * Adds a log handler to the logging system.
 *
 * @param pHandler A pointer to the log handler to add.
 */
void mdLogAddHandler(const struct MdLogHandler* pHandler);

/**
 * Logs a message with the specified log level.
 *
 * @param level The log level of the message.
 * @param file The source file where the log message originated.
 * @param line The line number in the source file where the log message originated.
 * @param format The format string (printf-style) for the log message.
 * @param ... Additional arguments for the format string.
 */
void mdLogPrint(enum MdLogLevel level, const char* file, u32 line, const char* format, ...);

/**
 * Shuts down the logging system.
 * Should be called when logging is no longer needed.
 */
void mdLogShutdown();

#if __cplusplus
}
#endif
