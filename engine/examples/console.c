#include "NTTEngine/NTTEngine.h"

int main(void)
{
	nttPlatformSetColor(NTT_CONSOLE_COLOR_GREEN);
	nttPlatformPrint("Hello, NTT Engine Console Green!\n");

	nttPlatformSetColor(NTT_CONSOLE_COLOR_RED);
	nttPlatformPrint("Hello, NTT Engine Console Red!\n");

	nttPlatformSetColor(NTT_CONSOLE_COLOR_RESET);
	nttPlatformPrint("Hello, NTT Engine Console Reset!\n");
	return 0;
}