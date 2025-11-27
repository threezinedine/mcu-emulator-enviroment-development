#if PLATFORM_IS_LINUX
#include <stdarg.h>
#include <stdio.h>

#include "MEEDEngine/platforms/console.h"

static const char *ansi[] = {
	[MEED_CONSOLE_COLOR_RESET]	 = "\033[0m",
	[MEED_CONSOLE_COLOR_RED]	 = "\033[91m",
	[MEED_CONSOLE_COLOR_GREEN]	 = "\033[92m",
	[MEED_CONSOLE_COLOR_YELLOW]	 = "\033[93m",
	[MEED_CONSOLE_COLOR_BLUE]	 = "\033[94m",
	[MEED_CONSOLE_COLOR_MAGENTA] = "\033[95m",
	[MEED_CONSOLE_COLOR_CYAN]	 = "\033[96m",
	[MEED_CONSOLE_COLOR_WHITE]	 = "\033[97m",
	[MEED_CONSOLE_COLOR_BOLD]	 = "\033[1m",
};

void meedPlatformSetColor(enum MEEDConsoleColor color)
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