#include "MEEDEngine/MEEDEngine.h"

void wrap()
{
	MD_ASSERT(MD_FALSE);
}

int main(void)
{
	mdMemoryInitialize();

	wrap();

	mdMemoryShutdown();
	return 0;
}
