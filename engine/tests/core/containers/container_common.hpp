#include "common.hpp"

namespace {
static u32 s_deleteCallCount = 0;

struct TestNode
{
	int value;
};

void deleteTestNode(void* pData)
{
	TestNode* pNode = (TestNode*)pData;
	MD_FREE(pNode, TestNode);
	pData = MD_NULL;
	s_deleteCallCount++;
}
} // anonymous namespace
