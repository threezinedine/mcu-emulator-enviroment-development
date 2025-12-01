#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	struct MEEDPlatformConsoleConfig config;
	config.color = MEED_CONSOLE_COLOR_GREEN;

	mdPlatformSetConsoleConfig(config);
	mdPlatformPrint("Hello, MEED Engine Console Green!\n");

	config.color = MEED_CONSOLE_COLOR_RED;
	mdPlatformSetConsoleConfig(config);
	mdPlatformPrint("Hello, MEED Engine Console Red!\n");

	config.color = MEED_CONSOLE_COLOR_RESET;
	mdPlatformSetConsoleConfig(config);
	mdPlatformPrint("Hello, MEED Engine Console Reset!\n");
	return 0;
}