#include "MEEDEngine/MEEDEngine.h"

int main()
{
	mdPlatformMemoryInitialize();

	void* ptr = mdPlatformMalloc(128);
	MEED_ASSERT_MSG(ptr != MEED_NULL, "Memory allocation failed!");

	mdPlatformFree(ptr, 128);

	void* ptr2 = mdPlatformMalloc(256);
	void* ptr3 = mdPlatformMalloc(512);

	mdPlatformFree(ptr2, 256);
	mdPlatformFree(ptr3, 512);

	mdPlatformMemoryShutdown();
	return 0;
}