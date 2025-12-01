#include "container_common.hpp"

class StackTest : public Test
{
protected:
	void SetUp() override
	{
		s_pStack		  = mdStackCreate(NULL);
		s_deleteCallCount = 0;
	}

	void TearDown() override
	{
		mdStackDestroy(s_pStack);
	}

protected:
	struct MEEDStack* s_pStack;
	int				  a = 10;
	int				  b = 20;
	int				  c = 30;
};

TEST_F(StackTest, PushAndTop)
{
	mdStackPush(s_pStack, &a);
	EXPECT_EQ(*(int*)mdStackTop(s_pStack), 10);

	mdStackPush(s_pStack, &b);
	EXPECT_EQ(*(int*)mdStackTop(s_pStack), 20);

	mdStackPush(s_pStack, &c);
	EXPECT_EQ(*(int*)mdStackTop(s_pStack), 30);
}

TEST_F(StackTest, PopWithEmptyStack)
{
	mdStackPush(s_pStack, &a);
	mdStackPush(s_pStack, &b);
	mdStackPush(s_pStack, &c);

	mdStackPop(s_pStack);
	EXPECT_EQ(*(int*)mdStackTop(s_pStack), 20);

	mdStackPop(s_pStack);
	EXPECT_EQ(*(int*)mdStackTop(s_pStack), 10);

	mdStackPop(s_pStack);
	EXPECT_TRUE(mdStackEmpty(s_pStack) == MEED_TRUE);

	EXPECT_EXIT(
		{
			mdStackPop(s_pStack);
			std::exit(MEED_EXCEPTION_TYPE_EMPTY_CONTAINER);
		},
		testing::ExitedWithCode(MEED_EXCEPTION_TYPE_EMPTY_CONTAINER),
		"");
}

TEST_F(StackTest, EmptyStack)
{
	EXPECT_TRUE(mdStackEmpty(s_pStack) == MEED_TRUE);

	mdStackPush(s_pStack, &a);
	EXPECT_TRUE(mdStackEmpty(s_pStack) == MEED_FALSE);

	mdStackPop(s_pStack);
	EXPECT_TRUE(mdStackEmpty(s_pStack) == MEED_TRUE);
}

TEST_F(StackTest, GetCount)
{
	EXPECT_EQ(mdStackGetCount(s_pStack), 0u);

	mdStackPush(s_pStack, &a);
	EXPECT_EQ(mdStackGetCount(s_pStack), 1u);

	mdStackPush(s_pStack, &b);
	EXPECT_EQ(mdStackGetCount(s_pStack), 2u);

	mdStackPush(s_pStack, &c);
	EXPECT_EQ(mdStackGetCount(s_pStack), 3u);

	mdStackPop(s_pStack);
	EXPECT_EQ(mdStackGetCount(s_pStack), 2u);

	mdStackPop(s_pStack);
	EXPECT_EQ(mdStackGetCount(s_pStack), 1u);

	mdStackPop(s_pStack);
	EXPECT_EQ(mdStackGetCount(s_pStack), 0u);
}

TEST_F(StackTest, Clear)
{
	mdStackPush(s_pStack, &a);
	mdStackPush(s_pStack, &b);
	mdStackPush(s_pStack, &c);

	EXPECT_EQ(mdStackGetCount(s_pStack), 3u);

	mdStackClear(s_pStack);
	EXPECT_EQ(mdStackGetCount(s_pStack), 0u);
	EXPECT_TRUE(mdStackEmpty(s_pStack) == MEED_TRUE);
}

TEST_F(StackTest, TestWithCallback)
{
	struct MEEDStack* pStackWithCallback = mdStackCreate(deleteTestNode);

	TestNode* pNode1 = MEED_MALLOC(TestNode);
	pNode1->value	 = 100;

	TestNode* pNode2 = MEED_MALLOC(TestNode);
	pNode2->value	 = 200;

	mdStackPush(pStackWithCallback, pNode1);
	mdStackPush(pStackWithCallback, pNode2);

	EXPECT_EQ(mdStackGetCount(pStackWithCallback), 2u);

	EXPECT_EQ(s_deleteCallCount, 0);
	mdStackPop(pStackWithCallback);
	EXPECT_EQ(s_deleteCallCount, 1);

	EXPECT_EQ(mdStackGetCount(pStackWithCallback), 1u);
	EXPECT_EQ(((TestNode*)mdStackTop(pStackWithCallback))->value, 100);

	mdStackClear(pStackWithCallback);
	EXPECT_EQ(s_deleteCallCount, 2);

	TestNode* pNode3 = MEED_MALLOC(TestNode);
	pNode3->value	 = 300;

	mdStackPush(pStackWithCallback, pNode3);

	mdStackDestroy(pStackWithCallback);
}