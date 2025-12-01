#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdMemoryInitialize();
	struct MdConsoleConfig config;
	config.color = MD_CONSOLE_COLOR_GREEN;

	mdSetConsoleConfig(config);
	mdPrint("Hello, MEED Engine Console Green!\n");

	config.color = MD_CONSOLE_COLOR_RED;
	mdSetConsoleConfig(config);
	mdPrint("Hello, MEED Engine Console Red!\n");

	config.color = MD_CONSOLE_COLOR_RESET;
	mdSetConsoleConfig(config);
	mdPrint("Hello, MEED Engine Console Reset!\n");

	mdMemoryShutdown();
	return 0;
}