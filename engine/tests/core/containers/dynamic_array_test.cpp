#include "container_common.hpp"

class DynamicArrayTest : public Test
{
protected:
	void SetUp() override
	{
		s_pArray		  = mdDynamicArrayCreate(0, nullptr);
		s_deleteCallCount = 0;
	}

	void TearDown() override
	{
		mdDynamicArrayDestroy(s_pArray);
	}

protected:
	struct MdDynamicArray* s_pArray;
	int					   a = 10;
	int					   b = 20;
	int					   c = 30;
};

TEST_F(DynamicArrayTest, CreateAndDestroy)
{
	EXPECT_NE(s_pArray, nullptr);
	EXPECT_EQ(s_pArray->count, 0u);
	EXPECT_EQ(s_pArray->capacity, MD_DEFAULT_DYNAMIC_ARRAY_CAPACITY);
}

TEST_F(DynamicArrayTest, InitialCapacity)
{
	mdDynamicArrayDestroy(s_pArray);

	s_pArray = mdDynamicArrayCreate(5, nullptr);
	EXPECT_NE(s_pArray, nullptr);
	EXPECT_EQ(s_pArray->count, 0u);
	EXPECT_EQ(s_pArray->capacity, 5u);
}

TEST_F(DynamicArrayTest, PushBackAndSize)
{
	mdDynamicArrayPush(s_pArray, &a);
	EXPECT_EQ(s_pArray->count, 1u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 10);

	mdDynamicArrayPush(s_pArray, &b);
	EXPECT_EQ(s_pArray->count, 2u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 20);

	mdDynamicArrayPush(s_pArray, &c);
	EXPECT_EQ(s_pArray->count, 3u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 2), 30);

	EXPECT_GE(s_pArray->capacity, s_pArray->count);
}

TEST_F(DynamicArrayTest, AccessOutOfBounds)
{
	mdDynamicArrayPush(s_pArray, &a);
	mdDynamicArrayPush(s_pArray, &b);

	EXPECT_EXIT(
		{
			mdDynamicArrayAt(s_pArray, 2);
			std::exit(MD_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MD_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(DynamicArrayTest, Resize)
{
	mdDynamicArrayPush(s_pArray, &a);
	mdDynamicArrayPush(s_pArray, &b);

	u32 oldCapacity = s_pArray->capacity;
	mdDynamicArrayResize(s_pArray, oldCapacity * 2);
	EXPECT_EQ(s_pArray->capacity, oldCapacity * 2);
	EXPECT_EQ(s_pArray->count, 2u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 10);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 20);
}

TEST_F(DynamicArrayTest, ResizeInvalidOperation)
{
	mdDynamicArrayPush(s_pArray, &a);
	mdDynamicArrayPush(s_pArray, &b);

	EXPECT_EXIT(
		{
			mdDynamicArrayResize(s_pArray, 1);
			std::exit(MD_EXCEPTION_TYPE_INVALID_OPERATION);
		},
		testing::ExitedWithCode(MD_EXCEPTION_TYPE_INVALID_OPERATION),
		"");
}

TEST_F(DynamicArrayTest, Clear)
{
	mdDynamicArrayPush(s_pArray, &a);
	mdDynamicArrayPush(s_pArray, &b);
	mdDynamicArrayPush(s_pArray, &c);

	EXPECT_EQ(s_pArray->count, 3u);
	u32 oldCapacity = s_pArray->capacity;

	mdDynamicArrayClear(s_pArray);

	EXPECT_EQ(s_pArray->count, 0u);
	EXPECT_EQ(s_pArray->capacity, oldCapacity);
}

TEST_F(DynamicArrayTest, InsertAtBeginning)
{
	mdDynamicArrayPush(s_pArray, &b); // Array: [20]
	mdDynamicArrayPush(s_pArray, &c); // Array: [20, 30]

	mdDynamicArrayInsert(s_pArray, 0, &a); // Array: [10, 20, 30]

	EXPECT_EQ(s_pArray->count, 3u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 10);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 20);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 2), 30);
	EXPECT_GE(s_pArray->capacity, s_pArray->count);
}

TEST_F(DynamicArrayTest, InsertAtMiddle)
{
	mdDynamicArrayPush(s_pArray, &a); // Array: [10]
	mdDynamicArrayPush(s_pArray, &c); // Array: [10, 30]

	mdDynamicArrayInsert(s_pArray, 1, &b); // Array: [10, 20, 30]

	EXPECT_EQ(s_pArray->count, 3u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 10);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 20);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 2), 30);
	EXPECT_GE(s_pArray->capacity, s_pArray->count);
}

TEST_F(DynamicArrayTest, InsertAtEnd)
{
	mdDynamicArrayPush(s_pArray, &a); // Array: [10]
	mdDynamicArrayPush(s_pArray, &b); // Array: [10, 20]

	mdDynamicArrayInsert(s_pArray, 2, &c); // Array: [10, 20, 30]

	EXPECT_EQ(s_pArray->count, 3u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 10);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 20);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 2), 30);
	EXPECT_GE(s_pArray->capacity, s_pArray->count);
}

TEST_F(DynamicArrayTest, InsertOutOfBounds)
{
	mdDynamicArrayPush(s_pArray, &a); // Array: [10]

	EXPECT_EXIT(
		{
			mdDynamicArrayInsert(s_pArray, 2, &b);
			std::exit(MD_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MD_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(DynamicArrayTest, EraseAtBeginning)
{
	mdDynamicArrayPush(s_pArray, &a); // Array: [10]
	mdDynamicArrayPush(s_pArray, &b); // Array: [10, 20]
	mdDynamicArrayPush(s_pArray, &c); // Array: [10, 20, 30]

	mdDynamicArrayErase(s_pArray, 0); // Array: [20, 30]

	EXPECT_EQ(s_pArray->count, 2u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 20);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 30);
}

TEST_F(DynamicArrayTest, EraseAtMiddle)
{
	mdDynamicArrayPush(s_pArray, &a); // Array: [10]
	mdDynamicArrayPush(s_pArray, &b); // Array: [10, 20]
	mdDynamicArrayPush(s_pArray, &c); // Array: [10, 20, 30]

	mdDynamicArrayErase(s_pArray, 1); // Array: [10, 30]

	EXPECT_EQ(s_pArray->count, 2u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 10);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 30);
}

TEST_F(DynamicArrayTest, EraseAtEnd)
{
	mdDynamicArrayPush(s_pArray, &a); // Array: [10]
	mdDynamicArrayPush(s_pArray, &b); // Array: [10, 20]
	mdDynamicArrayPush(s_pArray, &c); // Array: [10, 20, 30]

	mdDynamicArrayErase(s_pArray, 2); // Array: [10, 20]

	EXPECT_EQ(s_pArray->count, 2u);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 0), 10);
	EXPECT_EQ(*(int*)mdDynamicArrayAt(s_pArray, 1), 20);
}

TEST_F(DynamicArrayTest, EraseOutOfBounds)
{
	mdDynamicArrayPush(s_pArray, &a); // Array: [10]

	EXPECT_EXIT(
		{
			mdDynamicArrayErase(s_pArray, 1);
			std::exit(MD_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MD_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(DynamicArrayTest, WithDeleteCallback)
{
	struct MdDynamicArray* pArrayWithCallback = mdDynamicArrayCreate(0, deleteTestNode);

	TestNode* pNode1 = MD_MALLOC(TestNode);
	pNode1->value	 = 100;

	TestNode* pNode2 = MD_MALLOC(TestNode);
	pNode2->value	 = 200;

	mdDynamicArrayPush(pArrayWithCallback, pNode1);
	mdDynamicArrayPush(pArrayWithCallback, pNode2);

	EXPECT_EQ(pArrayWithCallback->count, 2u);
	EXPECT_EQ(((TestNode*)mdDynamicArrayAt(pArrayWithCallback, 0))->value, 100);
	EXPECT_EQ(((TestNode*)mdDynamicArrayAt(pArrayWithCallback, 1))->value, 200);

	mdDynamicArrayDestroy(pArrayWithCallback);
}

TEST_F(DynamicArrayTest, ClearWithDeleteCallback)
{
	struct MdDynamicArray* pArrayWithCallback = mdDynamicArrayCreate(0, deleteTestNode);

	TestNode* pNode1 = MD_MALLOC(TestNode);
	pNode1->value	 = 100;

	TestNode* pNode2 = MD_MALLOC(TestNode);
	pNode2->value	 = 200;

	mdDynamicArrayPush(pArrayWithCallback, pNode1);
	mdDynamicArrayPush(pArrayWithCallback, pNode2);

	EXPECT_EQ(pArrayWithCallback->count, 2u);

	mdDynamicArrayClear(pArrayWithCallback);

	EXPECT_EQ(pArrayWithCallback->count, 0u);
	EXPECT_EQ(s_deleteCallCount, 2u);

	mdDynamicArrayDestroy(pArrayWithCallback);
}

TEST_F(DynamicArrayTest, DeleteWithDeleteCallback)
{
	struct MdDynamicArray* pArrayWithCallback = mdDynamicArrayCreate(0, deleteTestNode);

	TestNode* pNode1 = MD_MALLOC(TestNode);
	pNode1->value	 = 100;

	TestNode* pNode2 = MD_MALLOC(TestNode);
	pNode2->value	 = 200;

	mdDynamicArrayPush(pArrayWithCallback, pNode1);
	mdDynamicArrayPush(pArrayWithCallback, pNode2);

	EXPECT_EQ(pArrayWithCallback->count, 2u);

	EXPECT_EQ(s_deleteCallCount, 0);
	mdDynamicArrayErase(pArrayWithCallback, 0);
	EXPECT_EQ(s_deleteCallCount, 1);

	EXPECT_EQ(pArrayWithCallback->count, 1u);
	EXPECT_EQ(((TestNode*)mdDynamicArrayAt(pArrayWithCallback, 0))->value, 200);

	mdDynamicArrayDestroy(pArrayWithCallback);
}