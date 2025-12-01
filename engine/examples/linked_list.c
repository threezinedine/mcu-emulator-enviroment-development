#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdPlatformMemoryInitialize();

	struct MEEDLinkedList* pList = mdLinkedListCreate(MEED_NULL);

	mdLinkedListPush(pList, (void*)1);

	mdLinkedListDestroy(pList);

	mdPlatformMemoryShutdown();
	return 0;
}