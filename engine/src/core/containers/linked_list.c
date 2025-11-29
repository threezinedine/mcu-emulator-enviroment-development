#include "MEEDEngine/core/containers/linked_list.h"

struct MEEDLinkedList* meedLinkedListCreate()
{
	struct MEEDLinkedList* pList = MEED_MALLOC(struct MEEDLinkedList);
	MEED_ASSERT(pList != MEED_NULL);

	pList->size	 = 0;
	pList->pHead = MEED_NULL;
	pList->pTail = MEED_NULL;

	return pList;
}

void meedLinkedListPush(struct MEEDLinkedList* pList, void* pData)
{
	MEED_ASSERT(pList != MEED_NULL);

	struct MEEDLinkedListNode* pNewNode = MEED_MALLOC(struct MEEDLinkedListNode);
	MEED_ASSERT(pNewNode != MEED_NULL);

	pNewNode->pData = pData;
	pNewNode->pNext = MEED_NULL;

	if (pList->pTail != MEED_NULL)
	{
		pList->pTail->pNext = pNewNode;
	}
	else
	{
		pList->pHead = pNewNode;
	}

	pList->pTail = pNewNode;
	pList->size++;
}

void meedLinkedListDestroy(struct MEEDLinkedList* pList)
{
	MEED_ASSERT(pList != MEED_NULL);
	MEED_FREE(pList, struct MEEDLinkedList);
}