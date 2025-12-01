#include "MEEDEngine/core/containers/set.h"

struct MdSet* mdSetCreate(MdSetCompareCallback pCompareCallback)
{
	MEED_ASSERT(pCompareCallback != MEED_NULL);

	struct MdSet* pSet = MEED_MALLOC(struct MdSet);
	MEED_ASSERT(pSet != MEED_NULL);

	pSet->pList = mdLinkedListCreate(MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	pSet->pCompareCallback = pCompareCallback;

	return pSet;
}

u32 mdSetCount(struct MdSet* pSet)
{
	MEED_ASSERT(pSet != MEED_NULL);
	return mdLinkedListCount(pSet->pList);
}

void mdSetPush(struct MdSet* pSet, void* pData)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pCompareCallback != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	if (pData == MEED_NULL)
	{
		MEED_THROW(MD_EXCEPTION_TYPE_INVALID_OPERATION, "Cannot insert NULL data into the set.");
	}

	struct MdLinkedListNode* pCurrent = pSet->pList->pHead;
	u32						 index	  = 0;

	while (pCurrent != MEED_NULL)
	{
		i32						 cmpResult = pSet->pCompareCallback(pCurrent->pData, pData);
		struct MdLinkedListNode* pNextNode = pCurrent->pNext;

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

void* mdSetAt(struct MdSet* pSet, u32 index)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	return mdLinkedListAt(pSet->pList, index);
}

void mdSetErase(struct MdSet* pSet, u32 index)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	mdLinkedListErase(pSet->pList, index);
}

void mdSetClear(struct MdSet* pSet)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	mdLinkedListClear(pSet->pList);
}

u32 mdSetFind(struct MdSet* pSet, void* pData)
{
	MEED_ASSERT(pSet != MEED_NULL);
	MEED_ASSERT(pSet->pCompareCallback != MEED_NULL);
	MEED_ASSERT(pSet->pList != MEED_NULL);

	struct MdLinkedListNode* pCurrent = pSet->pList->pHead;
	u32						 index	  = 0;

	while (pCurrent != MEED_NULL)
	{
		i32						 cmpResult = pSet->pCompareCallback(pCurrent->pData, pData);
		struct MdLinkedListNode* pNextNode = pCurrent->pNext;

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

void mdSetDestroy(struct MdSet* pSet)
{
	MEED_ASSERT(pSet != MEED_NULL);

	mdLinkedListDestroy(pSet->pList);
	MEED_FREE(pSet, struct MdSet);
}