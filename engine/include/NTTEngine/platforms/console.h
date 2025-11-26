#pragma once

/**
 * All possible color which the console can display, using `nttPlatformSetColor` for
 *      modifying the color.
 */
enum NTTConsoleColor
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
 */
void nttPlatformSetColor(enum NTTConsoleColor color);

void nttPlatformPrint(const char *format, ...);