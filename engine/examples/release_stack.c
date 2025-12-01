#include "MEEDEngine/MEEDEngine.h"

static void releaseInt(void* pData)
{
	mdPlatformFPrint("Releasing integer: %d\n", *(int*)pData);
}

int main(void)
{
	mdPlatformMemoryInitialize();

	int a = 3;
	int b = 4;

	struct MEEDReleaseStack* pReleaseStack = mdReleaseStackCreate();

	mdReleaseStackPush(pReleaseStack, &a, releaseInt);
	mdReleaseStackPush(pReleaseStack, &b, releaseInt);

	mdReleaseStackDestroy(pReleaseStack);
	mdPlatformMemoryShutdown();
	return 0;
}