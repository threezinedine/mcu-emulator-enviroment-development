#include <gtest/gtest.h>
#include <iostream>

extern "C" {
#include "MEEDEngine/MEEDEngine.h"
}

int main(int argc, char** argv)
{
	mdMemoryInitialize();
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	mdMemoryShutdown();
	return result;
}