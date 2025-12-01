#if PLATFORM_IS_WEB

#include "MEEDEngine/platforms/common.h"
#include "MEEDEngine/platforms/window.h"

void mdWindowInitialize(void)
{
	// No window initialization needed for web builds.
}

struct MEEDWindowData* mdWindowCreate(u32 width, u32 height, const char* title)
{
	MEED_UNUSED(width);	 // Unused parameter in web builds.
	MEED_UNUSED(height); // Unused parameter in web builds.
	MEED_UNUSED(title);	 // Unused parameter in web builds.

	// Window creation is not supported in web builds.
	return MEED_NULL;
}

struct MEEDWindowEvent mdWindowPollEvents(struct MEEDWindowData* pWindowData)
{
	MEED_UNUSED(pWindowData); // Unused parameter in web builds.

	struct MEEDWindowEvent event;
	event.type = MEED_WINDOW_EVENT_TYPE_NONE;
	return event;
}

void mdWindowDestroy(struct MEEDWindowData* pWindowData)
{
	MEED_UNUSED(pWindowData); // Unused parameter in web builds.
							  // No window destruction needed for web builds.
}

void mdWindowShutdown(void)
{
	// No window shutdown needed for web builds.
}

#endif // PLATFORM_IS_WEB