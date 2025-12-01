#include "MEEDEngine/modules/release_stack/release_stack.h"
#include "MEEDEngine/core/containers/stack.h"

static void releaseStackItemDestroy(void* pData)
{
	MD_ASSERT(pData != MD_NULL);
	struct MdReleaseStackItem* pItem = (struct MdReleaseStackItem*)pData;

	if (pItem->pReleaseFunc != MD_NULL)
	{
		pItem->pReleaseFunc(pItem->pData);
	}

	MD_FREE(pItem, struct MdReleaseStackItem);
}

struct MdReleaseStack* mdReleaseStackCreate()
{
	struct MdReleaseStack* pReleaseStack = MD_MALLOC(struct MdReleaseStack);
	MD_ASSERT(pReleaseStack != MD_NULL);

	pReleaseStack->pStack = mdStackCreate(releaseStackItemDestroy);
	return pReleaseStack;
}

void mdReleaseStackPush(struct MdReleaseStack* pReleaseStack, void* pData, MdReleaseFunc pReleaseFunc)
{
	MD_ASSERT(pReleaseStack != MD_NULL);
	MD_ASSERT(pReleaseStack->pStack != MD_NULL);
	MD_ASSERT(pReleaseFunc != MD_NULL);

	struct MdReleaseStackItem* pItem = MD_MALLOC(struct MdReleaseStackItem);
	MD_ASSERT(pItem != MD_NULL);

	pItem->pData		= pData;
	pItem->pReleaseFunc = pReleaseFunc;

	mdStackPush(pReleaseStack->pStack, pItem);
}

void mdReleaseStackDestroy(struct MdReleaseStack* pReleaseStack)
{
	MD_ASSERT(pReleaseStack != MD_NULL);

	while (mdStackEmpty(pReleaseStack->pStack) == MD_FALSE)
	{
		mdStackPop(pReleaseStack->pStack);
	}

	mdStackDestroy(pReleaseStack->pStack);
	MD_FREE(pReleaseStack, struct MdReleaseStack);
}