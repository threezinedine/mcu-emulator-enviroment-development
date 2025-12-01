#pragma once
#include "common.h"

#if __cplusplus
extern "C" {
#endif

/**
 * All possible color which the console can display, using
 * `mdPlatformSetColor` for modifying the color.
 */
enum MEED_BINDING MdConsoleColor : u8
{
	MD_CONSOLE_COLOR_RESET,
	MD_CONSOLE_COLOR_RED,
	MD_CONSOLE_COLOR_GREEN,
	MD_CONSOLE_COLOR_YELLOW,
	MD_CONSOLE_COLOR_BLUE,
	MD_CONSOLE_COLOR_MAGENTA,
	MD_CONSOLE_COLOR_CYAN,
	MD_CONSOLE_COLOR_WHITE,
	MD_CONSOLE_COLOR_BOLD,
};

/**
 * Configuration structure for the console platform.
 */
struct MEED_BINDING MdConsoleConfig
{
	enum MdConsoleColor color; ///< The color to set the console to.
};

/**
 * Modify the following content's color which is used by `mdPrint`.
 * @param color The color to set the console to.
 */
void mdSetConsoleConfig(struct MdConsoleConfig config) MEED_BINDING;

/**
 * Print formatted content to a buffer.
 * @param buffer The buffer to print to.
 * @param length The length of the buffer.
 * @param format The format string.
 * @param ... The format arguments.
 */
void mdFormatString(char* buffer, mdSize length, char* format, ...);

/**
 * Print formatted content to the console.
 * @param format The format string.
 * @param ... The format arguments.
 */
void mdFormatPrint(const char* format, ...);

/**
 * Print a string to the console.
 * @param str The string to print.
 */
void mdPrint(const char* str) MEED_BINDING;

#if __cplusplus
}
#endif