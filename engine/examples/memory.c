#include "MEEDEngine/MEEDEngine.h"

int main()
{
	mdMemoryInitialize();

	void* ptr = mdMalloc(128);
	MD_ASSERT_MSG(ptr != MD_NULL, "Memory allocation failed!");

	mdFree(ptr, 128);

	void* ptr2 = mdMalloc(256);
	void* ptr3 = mdMalloc(512);

	mdFree(ptr2, 256);
	mdFree(ptr3, 512);

	mdMemoryShutdown();
	return 0;
}