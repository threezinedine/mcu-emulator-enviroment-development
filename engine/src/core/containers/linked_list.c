#include "MEEDEngine/core/containers/linked_list.h"

struct MdLinkedList* mdLinkedListCreate(MdNodeDataDeleteCallback pDeleteCallback)
{
	struct MdLinkedList* pList = MEED_MALLOC(struct MdLinkedList);
	MEED_ASSERT(pList != MEED_NULL);

	pList->size			   = 0;
	pList->pHead		   = MEED_NULL;
	pList->pTail		   = MEED_NULL;
	pList->pDeleteCallback = pDeleteCallback;

	return pList;
}

void mdLinkedListPush(struct MdLinkedList* pList, void* pData)
{
	MEED_ASSERT(pList != MEED_NULL);

	struct MdLinkedListNode* pNewNode = MEED_MALLOC(struct MdLinkedListNode);
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

void mdLinkedListInsert(struct MdLinkedList* pList, u32 index, void* pData)
{
	MEED_ASSERT(pList != MEED_NULL);

	if (index > pList->size)
	{
		MEED_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
				   "Index out of bounds: Attempted to insert at index %u in a linked list of size %u.",
				   index,
				   pList->size);
	}

	struct MdLinkedListNode* pNewNode = MEED_MALLOC(struct MdLinkedListNode);
	MEED_ASSERT(pNewNode != MEED_NULL);

	pNewNode->pData = pData;
	pNewNode->pNext = MEED_NULL;

	if (index == pList->size)
	{
		// Insert at the end
		if (pList->pTail != MEED_NULL)
		{
			pList->pTail->pNext = pNewNode;
		}
		else
		{
			pList->pHead = pNewNode;
		}
		pList->pTail = pNewNode;
	}
	else if (index == 0)
	{
		// Insert at the beginning
		pNewNode->pNext = pList->pHead;
		pList->pHead	= pNewNode;
		if (pList->pTail == MEED_NULL)
		{
			pList->pTail = pNewNode;
		}
	}
	else
	{
		// Insert in the middle
		struct MdLinkedListNode* pCurrent = pList->pHead;
		for (u32 i = 0; i < index - 1; i++)
		{
			pCurrent = pCurrent->pNext;
		}
		pNewNode->pNext = pCurrent->pNext;
		pCurrent->pNext = pNewNode;
	}

	pList->size++;
}

void mdLinkedListErase(struct MdLinkedList* pList, u32 index)
{
	MEED_ASSERT(pList != MEED_NULL);

	if (index >= pList->size)
	{
		MEED_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
				   "Index out of bounds: Attempted to erase index %u in a linked list of size %u.",
				   index,
				   pList->size);
	}

	struct MdLinkedListNode* pCurrent = pList->pHead;
	struct MdLinkedListNode* pPrev	  = MEED_NULL;

	for (u32 i = 0; i < index; i++)
	{
		pPrev	 = pCurrent;
		pCurrent = pCurrent->pNext;
	}

	MEED_ASSERT(pCurrent != MEED_NULL);

	if (pPrev != MEED_NULL)
	{
		pPrev->pNext = pCurrent->pNext;
	}
	else
	{
		pList->pHead = pCurrent->pNext;
	}

	if (pCurrent == pList->pTail)
	{
		pList->pTail = pPrev;
	}

	if (pList->pDeleteCallback != MEED_NULL)
	{
		pList->pDeleteCallback(pCurrent->pData);
	}

	MEED_FREE(pCurrent, struct MdLinkedListNode);
	pList->size--;
}

u32 mdLinkedListCount(struct MdLinkedList* pList)
{
	MEED_ASSERT(pList != MEED_NULL);
	return (u32)(pList->size);
}

b8 mdLinkedListEmpty(struct MdLinkedList* pList)
{
	MEED_ASSERT(pList != MEED_NULL);
	return (pList->size == 0) ? MEED_TRUE : MEED_FALSE;
}

void* mdLinkedListAt(struct MdLinkedList* pList, u32 index)
{
	MEED_ASSERT(pList != MEED_NULL);

	if (index >= pList->size)
	{
		MEED_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
				   "Index out of bounds: Attempted to access index %u in a linked list of size %u.",
				   index,
				   pList->size);
	}

	struct MdLinkedListNode* pCurrent = pList->pHead;
	for (u32 i = 0; i < index; i++)
	{
		pCurrent = pCurrent->pNext;
	}

	MEED_ASSERT(pCurrent != MEED_NULL);
	return pCurrent->pData;
}

void mdLinkedListClear(struct MdLinkedList* pList)
{
	MEED_ASSERT(pList != MEED_NULL);

	struct MdLinkedListNode* pCurrent = pList->pHead;

	while (pCurrent != MEED_NULL)
	{
		struct MdLinkedListNode* pNext = pCurrent->pNext;

		MEED_ASSERT(pCurrent != MEED_NULL);

		if (pList->pDeleteCallback != MEED_NULL)
		{
			pList->pDeleteCallback(pCurrent->pData);
		}

		MEED_FREE(pCurrent, struct MdLinkedListNode);
		pCurrent = pNext;
	}

	pList->pHead = MEED_NULL;
	pList->pTail = MEED_NULL;
	pList->size	 = 0;
}

void mdLinkedListDestroy(struct MdLinkedList* pList)
{
	MEED_ASSERT(pList != MEED_NULL);

	mdLinkedListClear(pList);

	MEED_FREE(pList, struct MdLinkedList);
}