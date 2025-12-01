#include "MEEDEngine/MEEDEngine.h"

static void releaseInt(void* pData)
{
	mdFormatPrint("Releasing integer: %d\n", *(int*)pData);
}

int main(void)
{
	mdMemoryInitialize();

	int a = 3;
	int b = 4;

	struct MdReleaseStack* pReleaseStack = mdReleaseStackCreate();

	mdReleaseStackPush(pReleaseStack, &a, releaseInt);
	mdReleaseStackPush(pReleaseStack, &b, releaseInt);

	mdReleaseStackDestroy(pReleaseStack);
	mdMemoryShutdown();
	return 0;
}