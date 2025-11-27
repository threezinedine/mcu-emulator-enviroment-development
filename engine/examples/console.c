#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	meedPlatformSetColor(MEED_CONSOLE_COLOR_GREEN);
	meedPlatformPrint("Hello, MEED Engine Console Green!\n");

	meedPlatformSetColor(MEED_CONSOLE_COLOR_RED);
	meedPlatformPrint("Hello, MEED Engine Console Red!\n");

	meedPlatformSetColor(MEED_CONSOLE_COLOR_RESET);
	meedPlatformPrint("Hello, MEED Engine Console Reset!\n");
	return 0;
}