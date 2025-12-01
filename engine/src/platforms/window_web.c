#if PLATFORM_IS_WEB

#include "MEEDEngine/platforms/common.h"
#include "MEEDEngine/platforms/window.h"

void mdWindowInitialize(void)
{
	// No window initialization needed for web builds.
}

struct MdWindowData* mdWindowCreate(u32 width, u32 height, const char* title)
{
	MD_UNUSED(width);  // Unused parameter in web builds.
	MD_UNUSED(height); // Unused parameter in web builds.
	MD_UNUSED(title);  // Unused parameter in web builds.

	// Window creation is not supported in web builds.
	return MD_NULL;
}

struct MdWindowEvent mdWindowPollEvents(struct MdWindowData* pWindowData)
{
	MD_UNUSED(pWindowData); // Unused parameter in web builds.

	struct MdWindowEvent event;
	event.type = MD_WINDOW_EVENT_TYPE_NONE;
	return event;
}

void mdWindowDestroy(struct MdWindowData* pWindowData)
{
	MD_UNUSED(pWindowData); // Unused parameter in web builds.
							// No window destruction needed for web builds.
}

void mdWindowShutdown(void)
{
	// No window shutdown needed for web builds.
}

#endif // PLATFORM_IS_WEB