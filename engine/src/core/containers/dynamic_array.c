#include "MEEDEngine/core/containers/dynamic_array.h"

struct MEEDDynamicArray* mdDynamicArrayCreate(u32 initialCapacity, MEEDNodeDataDeleteCallback pDeleteCallback)
{
	struct MEEDDynamicArray* pArray = MEED_MALLOC(struct MEEDDynamicArray);
	MEED_ASSERT(pArray != MEED_NULL);

	if (initialCapacity == 0)
	{
		initialCapacity = MEED_DEFAULT_DYNAMIC_ARRAY_CAPACITY;
	}

	pArray->count	 = 0;
	pArray->capacity = initialCapacity;
	pArray->pData	 = MEED_MALLOC_ARRAY(void*, initialCapacity);
	MEED_ASSERT(pArray->pData != MEED_NULL);
	pArray->pDeleteCallback = pDeleteCallback;

	return pArray;
}

void mdDynamicArrayPush(struct MEEDDynamicArray* pArray, void* pData)
{
	MEED_ASSERT(pArray != MEED_NULL);

	if (pArray->count >= pArray->capacity)
	{
		mdDynamicArrayResize(pArray, pArray->capacity * 2);
	}

	pArray->pData[pArray->count] = pData;
	pArray->count++;
}

u32 mdDynamicArrayCount(struct MEEDDynamicArray* pArray)
{
	MEED_ASSERT(pArray != MEED_NULL);
	return pArray->count;
}

void* mdDynamicArrayAt(struct MEEDDynamicArray* pArray, u32 index)
{
	MEED_ASSERT(pArray != MEED_NULL);

	return pArray->pData[index];
}

void mdDynamicArrayResize(struct MEEDDynamicArray* pArray, u32 newCapacity)
{
	MEED_ASSERT(pArray != MEED_NULL);

	if (newCapacity <= pArray->capacity)
	{
		MEED_THROW(MEED_EXCEPTION_TYPE_INVALID_OPERATION,
				   "New capacity %u must be greater than current capacity %u.",
				   newCapacity,
				   pArray->capacity);
	}

	void** pNewData = MEED_MALLOC_ARRAY(void*, newCapacity);
	MEED_ASSERT(pNewData != MEED_NULL);

	mdPlatformMemoryCopy(pNewData, pArray->pData, sizeof(void*) * pArray->count);
	MEED_FREE_ARRAY(pArray->pData, void*, pArray->capacity);

	pArray->pData	 = pNewData;
	pArray->capacity = newCapacity;
}

void mdDynamicArrayClear(struct MEEDDynamicArray* pArray)
{
	MEED_ASSERT(pArray != MEED_NULL);

	if (pArray->pDeleteCallback != MEED_NULL)
	{
		for (u32 i = 0; i < pArray->count; i++)
		{
			pArray->pDeleteCallback(pArray->pData[i]);
		}
	}

	pArray->count = 0;
}

void mdDynamicArrayInsert(struct MEEDDynamicArray* pArray, u32 index, void* pData)
{
	MEED_ASSERT(pArray != MEED_NULL);

	if (index > pArray->count)
	{
		MEED_THROW(MEED_EXCEPTION_TYPE_OUT_OF_INDEX,
				   "Index out of bounds: Attempted to insert at index %u in a dynamic array of size %u.",
				   index,
				   pArray->count);
	}

	if (pArray->count >= pArray->capacity)
	{
		mdDynamicArrayResize(pArray, pArray->capacity * 2);
	}

	for (u32 i = pArray->count; i > index; i--)
	{
		pArray->pData[i] = pArray->pData[i - 1];
	}

	pArray->pData[index] = pData;
	pArray->count++;
}

void mdDynamicArrayErase(struct MEEDDynamicArray* pArray, u32 index)
{
	MEED_ASSERT(pArray != MEED_NULL);

	if (index >= pArray->count)
	{
		MEED_THROW(MEED_EXCEPTION_TYPE_OUT_OF_INDEX,
				   "Index out of bounds: Attempted to erase index %u in a dynamic array of size %u.",
				   index,
				   pArray->count);
	}

	if (pArray->pDeleteCallback != MEED_NULL)
	{
		pArray->pDeleteCallback(pArray->pData[index]);
	}

	for (u32 i = index; i < pArray->count - 1; i++)
	{
		pArray->pData[i] = pArray->pData[i + 1];
	}

	pArray->count--;
}

void mdDynamicArrayDestroy(struct MEEDDynamicArray* pArray)
{
	MEED_ASSERT(pArray != MEED_NULL);

	mdDynamicArrayClear(pArray);

	MEED_FREE_ARRAY(pArray->pData, void*, pArray->capacity);
	MEED_FREE(pArray, struct MEEDDynamicArray);
}