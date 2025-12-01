#include "MEEDEngine/core/containers/set.h"

struct MdSet* mdSetCreate(MdSetCompareCallback pCompareCallback)
{
	MD_ASSERT(pCompareCallback != MD_NULL);

	struct MdSet* pSet = MD_MALLOC(struct MdSet);
	MD_ASSERT(pSet != MD_NULL);

	pSet->pList = mdLinkedListCreate(MD_NULL);
	MD_ASSERT(pSet->pList != MD_NULL);

	pSet->pCompareCallback = pCompareCallback;

	return pSet;
}

u32 mdSetCount(struct MdSet* pSet)
{
	MD_ASSERT(pSet != MD_NULL);
	return mdLinkedListCount(pSet->pList);
}

void mdSetPush(struct MdSet* pSet, void* pData)
{
	MD_ASSERT(pSet != MD_NULL);
	MD_ASSERT(pSet->pCompareCallback != MD_NULL);
	MD_ASSERT(pSet->pList != MD_NULL);

	if (pData == MD_NULL)
	{
		MD_THROW(MD_EXCEPTION_TYPE_INVALID_OPERATION, "Cannot insert NULL data into the set.");
	}

	struct MdLinkedListNode* pCurrent = pSet->pList->pHead;
	u32						 index	  = 0;

	while (pCurrent != MD_NULL)
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
	MD_ASSERT(pSet != MD_NULL);
	MD_ASSERT(pSet->pList != MD_NULL);

	return mdLinkedListAt(pSet->pList, index);
}

void mdSetErase(struct MdSet* pSet, u32 index)
{
	MD_ASSERT(pSet != MD_NULL);
	MD_ASSERT(pSet->pList != MD_NULL);

	mdLinkedListErase(pSet->pList, index);
}

void mdSetClear(struct MdSet* pSet)
{
	MD_ASSERT(pSet != MD_NULL);
	MD_ASSERT(pSet->pList != MD_NULL);

	mdLinkedListClear(pSet->pList);
}

u32 mdSetFind(struct MdSet* pSet, void* pData)
{
	MD_ASSERT(pSet != MD_NULL);
	MD_ASSERT(pSet->pCompareCallback != MD_NULL);
	MD_ASSERT(pSet->pList != MD_NULL);

	struct MdLinkedListNode* pCurrent = pSet->pList->pHead;
	u32						 index	  = 0;

	while (pCurrent != MD_NULL)
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

	return MD_SET_NOT_FOUND_INDEX;
}

void mdSetDestroy(struct MdSet* pSet)
{
	MD_ASSERT(pSet != MD_NULL);

	mdLinkedListDestroy(pSet->pList);
	MD_FREE(pSet, struct MdSet);
}