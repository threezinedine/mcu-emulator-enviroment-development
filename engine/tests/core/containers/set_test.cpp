#include "common.hpp"

static i32 compareIntCallback(const void* pA, const void* pB)
{
	return (*(int*)pA) - (*(int*)pB);
}

class SetTest : public Test
{
protected:
	void SetUp() override
	{
		s_pSet = mdSetCreate(compareIntCallback);
	}
	void TearDown() override
	{
		mdSetDestroy(s_pSet);
	}

protected:
	struct MEEDSet* s_pSet;
	int				a = 10;
	int				b = 20;
	int				c = 30;
};

TEST_F(SetTest, CreateAndDestroy)
{
	EXPECT_NE(s_pSet, MEED_NULL);
	EXPECT_EQ(mdSetCount(s_pSet), 0u);
}

TEST_F(SetTest, Count)
{
	mdSetPush(s_pSet, &a);
	EXPECT_EQ(mdSetCount(s_pSet), 1u);
	EXPECT_EQ(*(int*)mdSetAt(s_pSet, 0), 10);
}

TEST_F(SetTest, PushAndAt)
{
	mdSetPush(s_pSet, &b);
	mdSetPush(s_pSet, &a);
	mdSetPush(s_pSet, &c);

	EXPECT_EQ(mdSetCount(s_pSet), 3u);
	EXPECT_EQ(*(int*)mdSetAt(s_pSet, 0), 10);
	EXPECT_EQ(*(int*)mdSetAt(s_pSet, 1), 20);
	EXPECT_EQ(*(int*)mdSetAt(s_pSet, 2), 30);
}

TEST_F(SetTest, PushDuplicate)
{
	mdSetPush(s_pSet, &b);
	mdSetPush(s_pSet, &a);
	mdSetPush(s_pSet, &c);

	EXPECT_EQ(mdSetCount(s_pSet), 3u);

	// Attempt to push duplicate
	mdSetPush(s_pSet, &b);
	EXPECT_EQ(mdSetCount(s_pSet), 3u);
}

TEST_F(SetTest, AtOutOfBounds)
{
	mdSetPush(s_pSet, &a);
	mdSetPush(s_pSet, &b);

	EXPECT_EXIT(
		{
			mdSetAt(s_pSet, 2);
			std::exit(MEED_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MEED_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(SetTest, PushNullData)
{
	EXPECT_EXIT(
		{
			mdSetPush(s_pSet, MEED_NULL);
			std::exit(MEED_EXCEPTION_TYPE_INVALID_OPERATION);
		},
		testing::ExitedWithCode(MEED_EXCEPTION_TYPE_INVALID_OPERATION),
		"");
}

TEST_F(SetTest, EraseElement)
{
	mdSetPush(s_pSet, &a);
	mdSetPush(s_pSet, &b);
	mdSetPush(s_pSet, &c);

	EXPECT_EQ(mdSetCount(s_pSet), 3u);

	mdSetErase(s_pSet, 1);

	EXPECT_EQ(mdSetCount(s_pSet), 2u);
	EXPECT_EQ(*(int*)mdSetAt(s_pSet, 0), 10);
	EXPECT_EQ(*(int*)mdSetAt(s_pSet, 1), 30);
}

TEST_F(SetTest, EraseOutOfBounds)
{
	mdSetPush(s_pSet, &a);

	EXPECT_EXIT(
		{
			mdSetErase(s_pSet, 1);
			std::exit(MEED_EXCEPTION_TYPE_OUT_OF_INDEX);
		},
		testing::ExitedWithCode(MEED_EXCEPTION_TYPE_OUT_OF_INDEX),
		"");
}

TEST_F(SetTest, EraseAllElements)
{
	mdSetPush(s_pSet, &a);
	mdSetPush(s_pSet, &b);
	mdSetPush(s_pSet, &c);

	EXPECT_EQ(mdSetCount(s_pSet), 3u);

	mdSetClear(s_pSet);

	EXPECT_EQ(mdSetCount(s_pSet), 0u);
}

TEST_F(SetTest, FindElement)
{
	mdSetPush(s_pSet, &a);
	mdSetPush(s_pSet, &c);

	u32 aIndex = mdSetFind(s_pSet, &a);
	u32 bIndex = mdSetFind(s_pSet, &b);

	EXPECT_EQ(aIndex, 0u);
	EXPECT_EQ(bIndex, MEED_SET_NOT_FOUND_INDEX);
}