#pragma once
#include "common.h"

/**
 * All possible color which the console can display, using `nttPlatformSetColor`
 * for modifying the color.
 */
enum NTT_BINDING NTTConsoleColor : u8
{
	NTT_CONSOLE_COLOR_RESET,
	NTT_CONSOLE_COLOR_RED,
	NTT_CONSOLE_COLOR_GREEN,
	NTT_CONSOLE_COLOR_YELLOW,
	NTT_CONSOLE_COLOR_BLUE,
	NTT_CONSOLE_COLOR_MAGENTA,
	NTT_CONSOLE_COLOR_CYAN,
	NTT_CONSOLE_COLOR_WHITE,
	NTT_CONSOLE_COLOR_BOLD,
};

/**
 * Modify the following content's color which is used by `nttPlatformPrint`.
 * @param color The color to set the console to.
 */
void nttPlatformSetColor(enum NTTConsoleColor color) NTT_BINDING;

/**
 * Print formatted content to the console.
 * @param format The format string.
 * @param ... The format arguments.
 */
void nttPlatformPrint(const char *format, ...) NTT_BINDING;