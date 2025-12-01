#if PLATFORM_IS_WEB
#include <emscripten/emscripten.h>

#include <stdarg.h>
#include <stdio.h>

#include "MEEDEngine/platforms/console.h"

void mdSetConsoleConfig(struct MdConsoleConfig config)
{
}

void mdFormatPrint(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void mdFormatString(char* buffer, mdSize length, char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, length, format, args);
	va_end(args);
}

void mdPrintTrace(struct MdTraceInfo* pTraceInfo)
{
#if MD_DEBUG
	MD_UNUSED(pTraceInfo);
#else
	MD_UNUSED(pTraceInfo);
#endif
}

void mdPrint(const char* str)
{
	printf("%s", str);
}

#endif // PLATFORM_IS_WEB
