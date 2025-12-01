#include "MEEDEngine/core/containers/stack.h"

struct MdStack* mdStackCreate(MdNodeDataDeleteCallback pDeleteCallback)
{
	struct MdStack* pStack = MEED_MALLOC(struct MdStack);
	MEED_ASSERT(pStack != MEED_NULL);

	pStack->pList = mdLinkedListCreate(pDeleteCallback);
	MEED_ASSERT(pStack->pList != MEED_NULL);

	return pStack;
}

b8 mdStackEmpty(struct MdStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);
	return mdLinkedListEmpty(pStack->pList);
}

u32 mdStackGetCount(struct MdStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);
	return mdLinkedListCount(pStack->pList);
}

void mdStackPush(struct MdStack* pStack, void* pData)
{
	MEED_ASSERT(pStack != MEED_NULL);
	mdLinkedListPush(pStack->pList, pData);
}

void* mdStackTop(struct MdStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);
	MEED_ASSERT(mdStackEmpty(pStack) == MEED_FALSE);

	u32 count = mdStackGetCount(pStack);
	return mdLinkedListAt(pStack->pList, count - 1);
}

void mdStackPop(struct MdStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);

	if (mdStackEmpty(pStack) == MEED_TRUE)
	{
		MEED_THROW(MD_EXCEPTION_TYPE_EMPTY_CONTAINER, "Attempted to pop from an empty stack.");
	}

	u32 count = mdStackGetCount(pStack);
	mdLinkedListErase(pStack->pList, count - 1);
}

void mdStackClear(struct MdStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);

	while (mdStackEmpty(pStack) == MEED_FALSE)
	{
		mdStackPop(pStack);
	}
}

void mdStackDestroy(struct MdStack* pStack)
{
	MEED_ASSERT(pStack != MEED_NULL);

	mdLinkedListDestroy(pStack->pList);
	MEED_FREE(pStack, struct MdStack);
}