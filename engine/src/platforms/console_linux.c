#if PLATFORM_IS_LINUX
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#if MD_DEBUG
#include <execinfo.h>
#include <pthread.h>
#include <unistd.h>
#endif

#include "MEEDEngine/platforms/console.h"

static const char* ansi[] = {
	[MD_CONSOLE_COLOR_RESET]   = "\033[0m",
	[MD_CONSOLE_COLOR_RED]	   = "\033[91m",
	[MD_CONSOLE_COLOR_GREEN]   = "\033[92m",
	[MD_CONSOLE_COLOR_YELLOW]  = "\033[93m",
	[MD_CONSOLE_COLOR_BLUE]	   = "\033[94m",
	[MD_CONSOLE_COLOR_MAGENTA] = "\033[95m",
	[MD_CONSOLE_COLOR_CYAN]	   = "\033[96m",
	[MD_CONSOLE_COLOR_WHITE]   = "\033[97m",
	[MD_CONSOLE_COLOR_BOLD]	   = "\033[1m",
};

void mdSetConsoleConfig(struct MdConsoleConfig config)
{
	printf("%s", ansi[config.color]);
}

void mdFormatString(char* buffer, mdSize length, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, length, format, args);
	va_end(args);
}

void mdFormatPrint(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void mdPrint(const char* str)
{
	printf("%s", str);
}

void mdPrintTrace(struct MdTraceInfo* pTraceInfo)
{
#if MD_DEBUG
	struct MdTraceInfo* pPrintTraceInfo = pTraceInfo;

	if (pTraceInfo == MD_NULL)
	{
		pPrintTraceInfo = MD_MALLOC(struct MdTraceInfo);
		mdMemorySet(pPrintTraceInfo, 0, sizeof(struct MdTraceInfo));

		pPrintTraceInfo->framesCount = backtrace(pPrintTraceInfo->frames, MD_MAX_TRACE_FRAMES);
		pPrintTraceInfo->threadId	 = getpid();
	}

	MD_ASSERT(pPrintTraceInfo->framesCount > 0);

	struct MdConsoleConfig config;
	config.color = MD_CONSOLE_COLOR_RED;
	mdSetConsoleConfig(config);

	mdFormatPrint("=== Stack Trace: ===\n");
#if 0
	char cmd[512];
	snprintf(cmd, sizeof(cmd), "addr2line -e /proc/%d/exe -pif", pPrintTraceInfo->threadId);
#endif
	for (mdSize i = 0; i < pPrintTraceInfo->framesCount; i++)
	{
		char** function = backtrace_symbols(&pPrintTraceInfo->frames[i], 1);
		mdPrint(function[0]);
		mdPrint("\n");
	}

	config.color = MD_CONSOLE_COLOR_RESET;
	mdSetConsoleConfig(config);
#else
	MD_UNUSED(pTraceInfo);
#endif
}

#endif // PLATFORM_IS_LINUX