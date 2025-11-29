#include "MEEDEngine/MEEDEngine.h"

int main()
{
	meedPlatformMemoryInit();

	void* ptr = meedPlatformMalloc(128);
	MEED_ASSERT_MSG(ptr != MEED_NULL, "Memory allocation failed!");

	meedPlatformFree(ptr);

	meedPlatformMemoryShutdown();
	return 0;
}