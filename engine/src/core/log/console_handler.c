#include "MEEDEngine/core/log/log.h"

static void mdConsoleLogHandlerInit()
{
	// No initialization needed for console logging.
}

static void mdConsoleLogHandlerRecordHandle(const struct MdLogRecord* pRecord)
{
	const char*			   levelStr = "";
	struct MdConsoleConfig config;

	switch (pRecord->level)
	{
	case MD_LOG_LEVEL_VERBOSE:
		levelStr	 = "VERBOSE";
		config.color = MD_CONSOLE_COLOR_WHITE;
		break;
	case MD_LOG_LEVEL_DEBUG:
		levelStr	 = "DEBUG";
		config.color = MD_CONSOLE_COLOR_BLUE;
		break;
	case MD_LOG_LEVEL_INFO:
		levelStr	 = "INFO";
		config.color = MD_CONSOLE_COLOR_GREEN;
		break;
	case MD_LOG_LEVEL_WARNING:
		levelStr	 = "WARNING";
		config.color = MD_CONSOLE_COLOR_YELLOW;
		break;
	case MD_LOG_LEVEL_ERROR:
		levelStr	 = "ERROR";
		config.color = MD_CONSOLE_COLOR_RED;
		break;
	case MD_LOG_LEVEL_FATAL:
		levelStr	 = "FATAL";
		config.color = MD_CONSOLE_COLOR_MAGENTA;
		break;
	default:
		MD_UNTOUCHABLE();
		break;
	}

	mdSetConsoleConfig(config);
	mdFormatPrint("[%7s] - [%7s] - %s\n", "ENGINE", levelStr, pRecord->message);

	config.color = MD_CONSOLE_COLOR_RESET;
	mdSetConsoleConfig(config);
}

static void mdConsoleLogHandlerShutdown()
{
	// No shutdown needed for console logging.
}

struct MdLogHandler* MD_LOG_CONSOLE_HANDLER = MD_NULL;

void mdInitializeConsoleLogHandler(enum MdLogLevel level)
{
	MD_ASSERT(MD_LOG_CONSOLE_HANDLER == MD_NULL);

	MD_LOG_CONSOLE_HANDLER				 = MD_MALLOC(struct MdLogHandler);
	MD_LOG_CONSOLE_HANDLER->init		 = mdConsoleLogHandlerInit;
	MD_LOG_CONSOLE_HANDLER->recordHandle = mdConsoleLogHandlerRecordHandle;
	MD_LOG_CONSOLE_HANDLER->shutdown	 = mdConsoleLogHandlerShutdown;
	MD_LOG_CONSOLE_HANDLER->level		 = MD_LOG_LEVEL_DEBUG; // Default log level
}

void mdShutdownConsoleLogHandler()
{
	MD_ASSERT(MD_LOG_CONSOLE_HANDLER != MD_NULL);

	MD_FREE(MD_LOG_CONSOLE_HANDLER, struct MdLogHandler);
	MD_LOG_CONSOLE_HANDLER = MD_NULL;
}