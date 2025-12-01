#pragma once

#if __cplusplus
extern "C" {
#endif

#include "MEEDEngine/platforms/common.h"
#include "types.h"

#define MD_LOG_MESSAGE_MAX_LENGTH 1024

/**
 * @file handler.h
 * The log handler interface for the `MEEDEngine` logging system.
 */

/**
 * The log record structure, which holds information about a single log entry.
 */
struct MdLogRecord
{
	enum MdLogLevel level; ///< The log level of the message.
	const char*		file;  ///< The source file where the log message originated.
	u32				line;  ///< The line number in the source file where the log message originated.

	char message[MD_LOG_MESSAGE_MAX_LENGTH]; ///< The log message.
};

/**
 * The log handler initialization callback type.
 */
typedef void (*MdLogHandlerInitCallback)();

/**
 * The log record callback type.
 *
 * @param pRecord A pointer to the log record.
 */
typedef void (*MdLogHandlerRecordHandleCallback)(const struct MdLogRecord* pRecord);

/**
 * The log handler shutdown callback type.
 */
typedef void (*MdLogHandlerShutdownCallback)();

/**
 * The log handler structure.
 */
struct MdLogHandler
{
	MdLogHandlerInitCallback		 init;		   ///< The initialization callback.
	MdLogHandlerRecordHandleCallback recordHandle; ///< The log record callback.
	MdLogHandlerShutdownCallback	 shutdown;	   ///< The shutdown callback.
	enum MdLogLevel					 level;		   ///< The log level threshold for this handler.
};

extern struct MdLogHandler* MD_LOG_CONSOLE_HANDLER; ///< The console log handler instance.

/**
 * Initializes the console log handler.
 */
void mdInitializeConsoleLogHandler(enum MdLogLevel level);

/**
 * Shuts down the console log handler.
 */
void mdShutdownConsoleLogHandler();

#if __cplusplus
}
#endif