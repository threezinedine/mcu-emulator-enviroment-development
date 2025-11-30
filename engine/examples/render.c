#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	meedPlatformMemoryInitialize();
	meedRenderInitialize();

	struct MEEDWindowData* pWindow = meedWindowCreate(1600, 1400, "MEEDEngine Vulkan Renderer Example");

	while (!pWindow->shouldClose)
	{
		meedWindowPollEvents(pWindow);
		// Rendering and update logic would go here
	}

	meedWindowDestroy(pWindow);

	meedRenderShutdown();
	meedPlatformMemoryShutdown();
	return 0;
}