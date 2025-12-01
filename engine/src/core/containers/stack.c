#include "MEEDEngine/core/containers/stack.h"

struct MEEDStack* mdStackCreate(MEEDNodeDataDeleteCallback pDeleteCallback)
{
	struct MEEDStack* pStack = MEED_MALLOC(struct MEEDStack);
	MEED_ASSERT(pStack != MEED_NULL);

	pStack->pList = mdLinkedListCreate(pDeleteCallback);
	MEED_ASSERT(pStack->pList != MEED_NULL);

	return pStack;
}

b8 mdStackEmpty(struct MEEDStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);
	return mdLinkedListEmpty(pStack->pList);
}

u32 mdStackGetCount(struct MEEDStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);
	return mdLinkedListCount(pStack->pList);
}

void mdStackPush(struct MEEDStack* pStack, void* pData)
{
	MEED_ASSERT(pStack != MEED_NULL);
	mdLinkedListPush(pStack->pList, pData);
}

void* mdStackTop(struct MEEDStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);
	MEED_ASSERT(mdStackEmpty(pStack) == MEED_FALSE);

	u32 count = mdStackGetCount(pStack);
	return mdLinkedListAt(pStack->pList, count - 1);
}

void mdStackPop(struct MEEDStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);

	if (mdStackEmpty(pStack) == MEED_TRUE)
	{
		MEED_THROW(MEED_EXCEPTION_TYPE_EMPTY_CONTAINER, "Attempted to pop from an empty stack.");
	}

	u32 count = mdStackGetCount(pStack);
	mdLinkedListErase(pStack->pList, count - 1);
}

void mdStackClear(struct MEEDStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);

	while (mdStackEmpty(pStack) == MEED_FALSE)
	{
		mdStackPop(pStack);
	}
}

void mdStackDestroy(struct MEEDStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);

	mdLinkedListDestroy(pStack->pList);
	MEED_FREE(pStack, struct MEEDStack);
}