#if PLATFORM_IS_LINUX
#include <stdarg.h>
#include <stdio.h>

#include "NTTEngine/platforms/console.h"

static const char *ansi[] = {
	[NTT_CONSOLE_COLOR_RESET]	= "\033[0m",
	[NTT_CONSOLE_COLOR_RED]		= "\033[91m",
	[NTT_CONSOLE_COLOR_GREEN]	= "\033[92m",
	[NTT_CONSOLE_COLOR_YELLOW]	= "\033[93m",
	[NTT_CONSOLE_COLOR_BLUE]	= "\033[94m",
	[NTT_CONSOLE_COLOR_MAGENTA] = "\033[95m",
	[NTT_CONSOLE_COLOR_CYAN]	= "\033[96m",
	[NTT_CONSOLE_COLOR_WHITE]	= "\033[97m",
	[NTT_CONSOLE_COLOR_BOLD]	= "\033[1m",
};

void meedPlatformSetColor(enum NTTConsoleColor color)
{
	printf("%s", ansi[color]);
}

void meedPlatformPrint(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

#endif