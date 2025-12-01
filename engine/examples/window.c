#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdPlatformMemoryInitialize();
	mdWindowInitialize();

	struct MEEDWindowData* pWindowData = mdWindowCreate(800, 600, "MEED Application Window");

	while (pWindowData->shouldClose == MEED_FALSE)
	{
		struct MEEDWindowEvent windowEvent = mdWindowPollEvents(pWindowData);
		if (windowEvent.type == MEED_WINDOW_EVENT_TYPE_CLOSE)
		{
			pWindowData->shouldClose = MEED_TRUE;
		}
	}

	mdWindowDestroy(pWindowData);

	mdWindowShutdown();
	mdPlatformMemoryShutdown();
	return 0;
}