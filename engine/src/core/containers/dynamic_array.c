#include "MEEDEngine/core/containers/dynamic_array.h"

struct MdDynamicArray* mdDynamicArrayCreate(u32 initialCapacity, MdNodeDataDeleteCallback pDeleteCallback)
{
	struct MdDynamicArray* pArray = MD_MALLOC(struct MdDynamicArray);
	MD_ASSERT(pArray != MD_NULL);

	if (initialCapacity == 0)
	{
		initialCapacity = MD_DEFAULT_DYNAMIC_ARRAY_CAPACITY;
	}

	pArray->count	 = 0;
	pArray->capacity = initialCapacity;
	pArray->pData	 = MD_MALLOC_ARRAY(void*, initialCapacity);
	MD_ASSERT(pArray->pData != MD_NULL);
	pArray->pDeleteCallback = pDeleteCallback;

	return pArray;
}

void mdDynamicArrayPush(struct MdDynamicArray* pArray, void* pData)
{
	MD_ASSERT(pArray != MD_NULL);

	if (pArray->count >= pArray->capacity)
	{
		mdDynamicArrayResize(pArray, pArray->capacity * 2);
	}

	pArray->pData[pArray->count] = pData;
	pArray->count++;
}

u32 mdDynamicArrayCount(struct MdDynamicArray* pArray)
{
	MD_ASSERT(pArray != MD_NULL);
	return pArray->count;
}

void* mdDynamicArrayAt(struct MdDynamicArray* pArray, u32 index)
{
	MD_ASSERT(pArray != MD_NULL);

	return pArray->pData[index];
}

void mdDynamicArrayResize(struct MdDynamicArray* pArray, u32 newCapacity)
{
	MD_ASSERT(pArray != MD_NULL);

	if (newCapacity <= pArray->capacity)
	{
		MD_THROW(MD_EXCEPTION_TYPE_INVALID_OPERATION,
				 "New capacity %u must be greater than current capacity %u.",
				 newCapacity,
				 pArray->capacity);
	}

	void** pNewData = MD_MALLOC_ARRAY(void*, newCapacity);
	MD_ASSERT(pNewData != MD_NULL);

	mdMemoryCopy(pNewData, pArray->pData, sizeof(void*) * pArray->count);
	MD_FREE_ARRAY(pArray->pData, void*, pArray->capacity);

	pArray->pData	 = pNewData;
	pArray->capacity = newCapacity;
}

void mdDynamicArrayClear(struct MdDynamicArray* pArray)
{
	MD_ASSERT(pArray != MD_NULL);

	if (pArray->pDeleteCallback != MD_NULL)
	{
		for (u32 i = 0; i < pArray->count; i++)
		{
			pArray->pDeleteCallback(pArray->pData[i]);
		}
	}

	pArray->count = 0;
}

void mdDynamicArrayInsert(struct MdDynamicArray* pArray, u32 index, void* pData)
{
	MD_ASSERT(pArray != MD_NULL);

	if (index > pArray->count)
	{
		MD_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
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

void mdDynamicArrayErase(struct MdDynamicArray* pArray, u32 index)
{
	MD_ASSERT(pArray != MD_NULL);

	if (index >= pArray->count)
	{
		MD_THROW(MD_EXCEPTION_TYPE_OUT_OF_INDEX,
				 "Index out of bounds: Attempted to erase index %u in a dynamic array of size %u.",
				 index,
				 pArray->count);
	}

	if (pArray->pDeleteCallback != MD_NULL)
	{
		pArray->pDeleteCallback(pArray->pData[index]);
	}

	for (u32 i = index; i < pArray->count - 1; i++)
	{
		pArray->pData[i] = pArray->pData[i + 1];
	}

	pArray->count--;
}

void mdDynamicArrayDestroy(struct MdDynamicArray* pArray)
{
	MD_ASSERT(pArray != MD_NULL);

	mdDynamicArrayClear(pArray);

	MD_FREE_ARRAY(pArray->pData, void*, pArray->capacity);
	MD_FREE(pArray, struct MdDynamicArray);
}