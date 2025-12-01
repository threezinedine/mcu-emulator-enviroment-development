#include <gtest/gtest.h>
#include <iostream>

extern "C" {
#include "MEEDEngine/MEEDEngine.h"
}

int main(int argc, char** argv)
{
	mdPlatformMemoryInitialize();
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	mdPlatformMemoryShutdown();
	return result;
}