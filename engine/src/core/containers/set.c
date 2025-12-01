#include "MEEDEngine/core/containers/set.h"

struct MEEDSet* mdSetCreate(MEEDSetCompareCallback pCompareCallback)
{
	MEED_ASSERT(pCompareCallback != MEED_NULL);

	struct MEEDSet* pSet = MEED_MALLOC(struct MEEDSet);
	MEED_ASSERT(pSet != MEED_NULL);

	pSet->pList = mdLinkedListCreate(MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	pSet->pCompareCallback = pCompareCallback;

	return pSet;
}

u32 mdSetCount(struct MEEDSet* pSet)
{
	MEED_ASSERT(pSet != MEED_NULL);
	return mdLinkedListCount(pSet->pList);
}

void mdSetPush(struct MEEDSet* pSet, void* pData)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pCompareCallback != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	if (pData == MEED_NULL)
	{
		MEED_THROW(MEED_EXCEPTION_TYPE_INVALID_OPERATION, "Cannot insert NULL data into the set.");
	}

	struct MEEDLinkedListNode* pCurrent = pSet->pList->pHead;
	u32						   index	= 0;

	while (pCurrent != MEED_NULL)
	{
		i32						   cmpResult = pSet->pCompareCallback(pCurrent->pData, pData);
		struct MEEDLinkedListNode* pNextNode = pCurrent->pNext;

		if (cmpResult == 0)
		{
			return;
		}
		else if (cmpResult > 0)
		{
			mdLinkedListInsert(pSet->pList, index, pData);
			return;
		}
		else
		{
			pCurrent = pNextNode;
			index++;
		}
	}

	mdLinkedListPush(pSet->pList, pData);
}

void* mdSetAt(struct MEEDSet* pSet, u32 index)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	return mdLinkedListAt(pSet->pList, index);
}

void mdSetErase(struct MEEDSet* pSet, u32 index)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	mdLinkedListErase(pSet->pList, index);
}

void mdSetClear(struct MEEDSet* pSet)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	mdLinkedListClear(pSet->pList);
}

u32 mdSetFind(struct MEEDSet* pSet, void* pData)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pCompareCallback != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	struct MEEDLinkedListNode* pCurrent = pSet->pList->pHead;
	u32						   index	= 0;

	while (pCurrent != MEED_NULL)
	{
		i32						   cmpResult = pSet->pCompareCallback(pCurrent->pData, pData);
		struct MEEDLinkedListNode* pNextNode = pCurrent->pNext;

		if (cmpResult == 0)
		{
			return index;
		}
		else if (cmpResult > 0)
		{
			break;
		}
		else
		{
			pCurrent = pNextNode;
			index++;
		}
	}

	return MEED_SET_NOT_FOUND_INDEX;
}

void mdSetDestroy(struct MEEDSet* pSet)
{
	MEED_ASSERT(pSet != MEED_NULL);

	mdLinkedListDestroy(pSet->pList);
	MEED_FREE(pSet, struct MEEDSet);
}