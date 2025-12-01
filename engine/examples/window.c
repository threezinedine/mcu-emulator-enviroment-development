#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdMemoryInitialize();
	mdWindowInitialize();

	struct MdWindowData* pWindowData = mdWindowCreate(800, 600, "MEED Application Window");

	while (pWindowData->shouldClose == MEED_FALSE)
	{
		struct MdWindowEvent windowEvent = mdWindowPollEvents(pWindowData);
		if (windowEvent.type == MD_WINDOW_EVENT_TYPE_CLOSE)
		{
			pWindowData->shouldClose = MEED_TRUE;
		}
	}

	mdWindowDestroy(pWindowData);

	mdWindowShutdown();
	mdMemoryShutdown();
	return 0;
}