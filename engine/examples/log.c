#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdMemoryInitialize();
	mdInitializeConsoleLogHandler(MD_LOG_LEVEL_DEBUG);
	mdLogInitialize(MD_LOG_LEVEL_DEBUG);
	mdLogAddHandler(MD_LOG_CONSOLE_HANDLER);

	MD_LOG_VERBOSE("MEED Verbose");
	MD_LOG_DEBUG("MEED Debug");
	MD_LOG_INFO("MEED Info");
	MD_LOG_WARNING("MEED Warning");
	MD_LOG_ERROR("MEED Error");
	MD_LOG_FATAL("MEED Fatal");

	mdLogShutdown();
	mdShutdownConsoleLogHandler();
	mdMemoryShutdown();
	return 0;
}