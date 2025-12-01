#include "MEEDEngine/core/containers/stack.h"

struct MdStack* mdStackCreate(MdNodeDataDeleteCallback pDeleteCallback)
{
	struct MdStack* pStack = MD_MALLOC(struct MdStack);
	MD_ASSERT(pStack != MD_NULL);

	pStack->pList = mdLinkedListCreate(pDeleteCallback);
	MD_ASSERT(pStack->pList != MD_NULL);

	return pStack;
}

b8 mdStackEmpty(struct MdStack* pStack)
{
	MD_ASSERT(pStack != MD_NULL);
	return mdLinkedListEmpty(pStack->pList);
}

u32 mdStackGetCount(struct MdStack* pStack)
{
	MD_ASSERT(pStack != MD_NULL);
	return mdLinkedListCount(pStack->pList);
}

void mdStackPush(struct MdStack* pStack, void* pData)
{
	MD_ASSERT(pStack != MD_NULL);
	mdLinkedListPush(pStack->pList, pData);
}

void* mdStackTop(struct MdStack* pStack)
{
	MD_ASSERT(pStack != MD_NULL);
	MD_ASSERT(mdStackEmpty(pStack) == MD_FALSE);

	u32 count = mdStackGetCount(pStack);
	return mdLinkedListAt(pStack->pList, count - 1);
}

void mdStackPop(struct MdStack* pStack)
{
	MD_ASSERT(pStack != MD_NULL);

	if (mdStackEmpty(pStack) == MD_TRUE)
	{
		MD_THROW(MD_EXCEPTION_TYPE_EMPTY_CONTAINER, "Attempted to pop from an empty stack.");
	}

	u32 count = mdStackGetCount(pStack);
	mdLinkedListErase(pStack->pList, count - 1);
}

void mdStackClear(struct MdStack* pStack)
{
	MD_ASSERT(pStack != MD_NULL);

	while (mdStackEmpty(pStack) == MD_FALSE)
	{
		mdStackPop(pStack);
	}
}

void mdStackDestroy(struct MdStack* pStack)
{
	MD_ASSERT(pStack != MD_NULL);

	mdLinkedListDestroy(pStack->pList);
	MD_FREE(pStack, struct MdStack);
}