#include "MEEDEngine/modules/release_stack/release_stack.h"
#include "MEEDEngine/core/containers/stack.h"

static void releaseStackItemDestroy(void* pData)
{
	MEED_ASSERT(pData != MEED_NULL);
	struct MdReleaseStackItem* pItem = (struct MdReleaseStackItem*)pData;

	if (pItem->pReleaseFunc != MEED_NULL)
	{
		pItem->pReleaseFunc(pItem->pData);
	}

	MEED_FREE(pItem, struct MdReleaseStackItem);
}

struct MdReleaseStack* mdReleaseStackCreate()
{
	struct MdReleaseStack* pReleaseStack = MEED_MALLOC(struct MdReleaseStack);
	MEED_ASSERT(pReleaseStack != MEED_NULL);

	pReleaseStack->pStack = mdStackCreate(releaseStackItemDestroy);
	return pReleaseStack;
}

void mdReleaseStackPush(struct MdReleaseStack* pReleaseStack, void* pData, MdReleaseFunc pReleaseFunc)
{
	MEED_ASSERT(pReleaseStack != MEED_NULL);
	MEED_ASSERT(pReleaseStack->pStack != MEED_NULL);
	MEED_ASSERT(pReleaseFunc != MEED_NULL);

	struct MdReleaseStackItem* pItem = MEED_MALLOC(struct MdReleaseStackItem);
	MEED_ASSERT(pItem != MEED_NULL);

	pItem->pData		= pData;
	pItem->pReleaseFunc = pReleaseFunc;

	mdStackPush(pReleaseStack->pStack, pItem);
}

void mdReleaseStackDestroy(struct MdReleaseStack* pReleaseStack)
{
	MEED_ASSERT(pReleaseStack != MEED_NULL);

	while (mdStackEmpty(pReleaseStack->pStack) == MEED_FALSE)
	{
		mdStackPop(pReleaseStack->pStack);
	}

	mdStackDestroy(pReleaseStack->pStack);
	MEED_FREE(pReleaseStack, struct MdReleaseStack);
}