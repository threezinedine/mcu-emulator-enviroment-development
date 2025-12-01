#include "container_common.hpp"

class LinkedListTest : public Test
{
protected:
	void SetUp() override
	{
		s_pList			  = mdLinkedListCreate(NULL);
		s_deleteCallCount = 0;
	}

	void TearDown() override
	{
		mdLinkedListDestroy(s_pList);
	}

protected:
	struct MdLinkedList* s_pList;

	int a = 10;
	int b = 20;
	int c = 30;
};

TEST_F(LinkedListTest, PushBackAndSize)
{
	mdLinkedListPush(s_pList, &a);
	mdLinkedListPush(s_pList, &b);
	mdLinkedListPush(s_pList, &c);

	EXPECT_EQ(mdLinkedListCount(s_pList), 3u);
}

TEST_F(LinkedListTest, GetElement)
{
	mdLinkedListPush(s_pList, &a);
	mdLinkedListPush(s_pList, &b);
	mdLinkedListPush(s_pList, &c);

	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 0)), 10);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 1)), 20);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 2)), 30);
}

TEST_F(LinkedListTest, OutOfBoundsAccess)
{
	mdLinkedListPush(s_pList, &a);
	mdLinkedListPush(s_pList, &b);

	EXPECT_EXIT(
		{
			mdLinkedListAt(s_pList, 2);
			std::exit(MD_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MD_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(LinkedListTest, InsertWithIndex0)
{
	mdLinkedListInsert(s_pList, 0, &a); // List: [10]
	mdLinkedListInsert(s_pList, 0, &c); // List: [30, 10]

	EXPECT_EQ(mdLinkedListCount(s_pList), 2u);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 0)), 30);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 1)), 10);
}

TEST_F(LinkedListTest, InsertWithMiddleIndex)
{
	mdLinkedListPush(s_pList, &a);		// List: [10]
	mdLinkedListPush(s_pList, &c);		// List: [10, 30]
	mdLinkedListInsert(s_pList, 1, &b); // List: [10, 20, 30]

	EXPECT_EQ(mdLinkedListCount(s_pList), 3u);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 0)), 10);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 1)), 20);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 2)), 30);
}

TEST_F(LinkedListTest, InsertWithEndIndex)
{
	mdLinkedListPush(s_pList, &a);		// List: [10]
	mdLinkedListPush(s_pList, &b);		// List: [10, 20]
	mdLinkedListInsert(s_pList, 2, &c); // List: [10, 20, 30]

	EXPECT_EQ(mdLinkedListCount(s_pList), 3u);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 0)), 10);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 1)), 20);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 2)), 30);
}

TEST_F(LinkedListTest, InsertOutOfBounds)
{
	mdLinkedListPush(s_pList, &a); // List: [10]

	EXPECT_EXIT(
		{
			mdLinkedListInsert(s_pList, 2, &b);
			std::exit(MD_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MD_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(LinkedListTest, EraseByIndex0)
{
	mdLinkedListPush(s_pList, &a); // List: [10]
	mdLinkedListPush(s_pList, &b); // List: [10, 20]
	mdLinkedListPush(s_pList, &c); // List: [10, 20, 30]

	mdLinkedListErase(s_pList, 0); // List: [20, 30]

	EXPECT_EQ(mdLinkedListCount(s_pList), 2u);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 0)), 20);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 1)), 30);
}

TEST_F(LinkedListTest, EraseByMiddleIndex)
{
	mdLinkedListPush(s_pList, &a); // List: [10]
	mdLinkedListPush(s_pList, &b); // List: [10, 20]
	mdLinkedListPush(s_pList, &c); // List: [10, 20, 30]

	mdLinkedListErase(s_pList, 1); // List: [10, 30]

	EXPECT_EQ(mdLinkedListCount(s_pList), 2u);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 0)), 10);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 1)), 30);
}

TEST_F(LinkedListTest, EraseByEndIndex)
{
	mdLinkedListPush(s_pList, &a); // List: [10]
	mdLinkedListPush(s_pList, &b); // List: [10, 20]
	mdLinkedListPush(s_pList, &c); // List: [10, 20, 30]

	mdLinkedListErase(s_pList, 2); // List: [10, 20]

	EXPECT_EQ(mdLinkedListCount(s_pList), 2u);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 0)), 10);
	EXPECT_EQ(*(int*)(mdLinkedListAt(s_pList, 1)), 20);
}

TEST_F(LinkedListTest, EraseOutOfBounds)
{
	mdLinkedListPush(s_pList, &a); // List: [10]

	EXPECT_EXIT(
		{
			mdLinkedListErase(s_pList, 1);
			std::exit(MD_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MD_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(LinkedListTest, EraseWithCallback)
{
	struct MdLinkedList* pListWithCallback = mdLinkedListCreate(deleteTestNode);

	TestNode* pNode1 = MEED_MALLOC(TestNode);
	pNode1->value	 = 100;

	TestNode* pNode2 = MEED_MALLOC(TestNode);
	pNode2->value	 = 200;

	mdLinkedListPush(pListWithCallback, pNode1);
	mdLinkedListPush(pListWithCallback, pNode2);

	EXPECT_EQ(mdLinkedListCount(pListWithCallback), 2u);

	EXPECT_EQ(s_deleteCallCount, 0);
	mdLinkedListErase(pListWithCallback, 0);
	EXPECT_EQ(s_deleteCallCount, 1);

	EXPECT_EQ(mdLinkedListCount(pListWithCallback), 1u);
	EXPECT_EQ(((TestNode*)mdLinkedListAt(pListWithCallback, 0))->value, 200);

	mdLinkedListDestroy(pListWithCallback);
}

TEST_F(LinkedListTest, EmptyListSize)
{
	EXPECT_EQ(mdLinkedListCount(s_pList), 0u);
	EXPECT_EQ(mdLinkedListEmpty(s_pList), MEED_TRUE);
}

TEST_F(LinkedListTest, ListWithDeleteCallback)
{
	struct MdLinkedList* pListWithCallback = mdLinkedListCreate(deleteTestNode);

	TestNode* pNode1 = MEED_MALLOC(TestNode);
	pNode1->value	 = 100;

	TestNode* pNode2 = MEED_MALLOC(TestNode);
	pNode2->value	 = 200;

	mdLinkedListPush(pListWithCallback, pNode1);
	mdLinkedListPush(pListWithCallback, pNode2);

	EXPECT_EQ(mdLinkedListCount(pListWithCallback), 2u);
	EXPECT_EQ(((TestNode*)mdLinkedListAt(pListWithCallback, 0))->value, 100);
	EXPECT_EQ(((TestNode*)mdLinkedListAt(pListWithCallback, 1))->value, 200);

	mdLinkedListDestroy(pListWithCallback);
}

TEST_F(LinkedListTest, ClearWithDeleteCallback)
{
	struct MdLinkedList* pListWithCallback = mdLinkedListCreate(deleteTestNode);

	TestNode* pNode1 = MEED_MALLOC(TestNode);
	pNode1->value	 = 100;

	TestNode* pNode2 = MEED_MALLOC(TestNode);
	pNode2->value	 = 200;

	mdLinkedListPush(pListWithCallback, pNode1);
	mdLinkedListPush(pListWithCallback, pNode2);

	EXPECT_EQ(mdLinkedListCount(pListWithCallback), 2u);

	mdLinkedListClear(pListWithCallback);

	EXPECT_EQ(mdLinkedListCount(pListWithCallback), 0u);

	mdLinkedListDestroy(pListWithCallback);
}

TEST_F(LinkedListTest, Clear)
{
	mdLinkedListPush(s_pList, &a);
	mdLinkedListPush(s_pList, &b);
	mdLinkedListPush(s_pList, &c);

	EXPECT_EQ(mdLinkedListCount(s_pList), 3u);

	mdLinkedListClear(s_pList);

	EXPECT_EQ(mdLinkedListCount(s_pList), 0u);
}