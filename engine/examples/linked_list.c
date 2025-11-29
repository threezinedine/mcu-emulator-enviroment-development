#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	meedPlatformMemoryInit();

	struct MEEDLinkedList* pList = meedLinkedListCreate();

	meedLinkedListPush(pList, (void*)1);

	meedLinkedListDestroy(pList);

	meedPlatformMemoryShutdown();
	return 0;
}