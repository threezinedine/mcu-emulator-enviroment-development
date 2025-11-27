#include "NTTEngine/NTTEngine.h"

int main(void)
{
	meedPlatformSetColor(NTT_CONSOLE_COLOR_GREEN);
	meedPlatformPrint("Hello, NTT Engine Console Green!\n");

	meedPlatformSetColor(NTT_CONSOLE_COLOR_RED);
	meedPlatformPrint("Hello, NTT Engine Console Red!\n");

	meedPlatformSetColor(NTT_CONSOLE_COLOR_RESET);
	meedPlatformPrint("Hello, NTT Engine Console Reset!\n");
	return 0;
}