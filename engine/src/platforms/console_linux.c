#if PLATFORM_IS_LINUX
#include <stdarg.h>
#include <stdio.h>

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

void mdFormatString(char* buffer, mdSize length, char* format, ...)
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

#endif // PLATFORM_IS_LINUX