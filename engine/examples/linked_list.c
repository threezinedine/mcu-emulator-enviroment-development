#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdMemoryInitialize();

	struct MdLinkedList* pList = mdLinkedListCreate(MD_NULL);

	mdLinkedListPush(pList, (void*)1);

	mdLinkedListDestroy(pList);

	mdMemoryShutdown();
	return 0;
}