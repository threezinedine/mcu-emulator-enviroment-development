#include "MEEDEngine/core/containers/linked_list.h"

struct MdLinkedList* mdLinkedListCreate(MdNodeDataDeleteCallback pDeleteCallback)
{
	struct MdLinkedList* pList = MD_MALLOC(struct MdLinkedList);
	MD_ASSERT(pList != MD_NULL);

	pList->size			   = 0;
	pList->pHead		   = MD_NULL;
	pList->pTail		   = MD_NULL;
	pList->pDeleteCallback = pDeleteCallback;

	return pList;
}

void mdLinkedListPush(struct MdLinkedList* pList, void* pData)
{
	MD_ASSERT(pList != MD_NULL);

	struct MdLinkedListNode* pNewNode = MD_MALLOC(struct MdLinkedListNode);
	MD_ASSERT(pNewNode != MD_NULL);

	pNewNode->pData = pData;
	pNewNode->pNext = MD_NULL;

	if (pList->pTail != MD_NULL)
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
	MD_ASSERT(pList != MD_NULL);

	if (index > pList->size)
	{
		MD_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
				 "Index out of bounds: Attempted to insert at index %u in a linked list of size %u.",
				 index,
				 pList->size);
	}

	struct MdLinkedListNode* pNewNode = MD_MALLOC(struct MdLinkedListNode);
	MD_ASSERT(pNewNode != MD_NULL);

	pNewNode->pData = pData;
	pNewNode->pNext = MD_NULL;

	if (index == pList->size)
	{
		// Insert at the end
		if (pList->pTail != MD_NULL)
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
		if (pList->pTail == MD_NULL)
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
	MD_ASSERT(pList != MD_NULL);

	if (index >= pList->size)
	{
		MD_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
				 "Index out of bounds: Attempted to erase index %u in a linked list of size %u.",
				 index,
				 pList->size);
	}

	struct MdLinkedListNode* pCurrent = pList->pHead;
	struct MdLinkedListNode* pPrev	  = MD_NULL;

	for (u32 i = 0; i < index; i++)
	{
		pPrev	 = pCurrent;
		pCurrent = pCurrent->pNext;
	}

	MD_ASSERT(pCurrent != MD_NULL);

	if (pPrev != MD_NULL)
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

	if (pList->pDeleteCallback != MD_NULL)
	{
		pList->pDeleteCallback(pCurrent->pData);
	}

	MD_FREE(pCurrent, struct MdLinkedListNode);
	pList->size--;
}

u32 mdLinkedListCount(struct MdLinkedList* pList)
{
	MD_ASSERT(pList != MD_NULL);
	return (u32)(pList->size);
}

b8 mdLinkedListEmpty(struct MdLinkedList* pList)
{
	MD_ASSERT(pList != MD_NULL);
	return (pList->size == 0) ? MD_TRUE : MD_FALSE;
}

void* mdLinkedListAt(struct MdLinkedList* pList, u32 index)
{
	MD_ASSERT(pList != MD_NULL);

	if (index >= pList->size)
	{
		MD_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
				 "Index out of bounds: Attempted to access index %u in a linked list of size %u.",
				 index,
				 pList->size);
	}

	struct MdLinkedListNode* pCurrent = pList->pHead;
	for (u32 i = 0; i < index; i++)
	{
		pCurrent = pCurrent->pNext;
	}

	MD_ASSERT(pCurrent != MD_NULL);
	return pCurrent->pData;
}

void mdLinkedListClear(struct MdLinkedList* pList)
{
	MD_ASSERT(pList != MD_NULL);

	struct MdLinkedListNode* pCurrent = pList->pHead;

	while (pCurrent != MD_NULL)
	{
		struct MdLinkedListNode* pNext = pCurrent->pNext;

		MD_ASSERT(pCurrent != MD_NULL);

		if (pList->pDeleteCallback != MD_NULL)
		{
			pList->pDeleteCallback(pCurrent->pData);
		}

		MD_FREE(pCurrent, struct MdLinkedListNode);
		pCurrent = pNext;
	}

	pList->pHead = MD_NULL;
	pList->pTail = MD_NULL;
	pList->size	 = 0;
}

void mdLinkedListDestroy(struct MdLinkedList* pList)
{
	MD_ASSERT(pList != MD_NULL);

	mdLinkedListClear(pList);

	MD_FREE(pList, struct MdLinkedList);
}