#if PLATFORM_IS_LINUX && !MD_USE_OPENGL
#include "MEEDEngine/platforms/window.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#if MD_USE_VULKAN
#include <vulkan/vulkan_xlib.h>
#endif

/**
 * Internal structure to hold Linux-specific window data.
 */
struct LinuxWindowData
{
	Display* pDisplay;
	i32		 screen;
	Window	 window;
};

static i32 ignore_x_errors(Display* display, XErrorEvent* event)
{
	return 0; // Ignore all X errors
}

static XEvent event; ///< used for event polling, declared static to avoid reallocation on each poll

static b8 s_isInitialized = MD_FALSE; ///< Track if the windowing system is initialized

#define MEED_WINDOW_UTILS_ASSERTION()                                                                                  \
	do                                                                                                                 \
	{                                                                                                                  \
		MD_ASSERT_MSG(                                                                                                 \
			s_isInitialized == MD_TRUE,                                                                                \
			"Windowing system is not initialized. Call mdWindowInitialize() before using window functions.");          \
		MD_ASSERT_MSG(pWindowData != MD_NULL, "Provided MdWindowData pointer is NULL.");                               \
		MD_ASSERT_MSG(pWindowData->pInternal != MD_NULL, "Internal window data pointer is NULL.");                     \
	} while (0)

void mdWindowInitialize()
{
	MD_ASSERT(s_isInitialized == MD_FALSE);

	// Set X11 error handler to ignore errors
	XSetErrorHandler(ignore_x_errors);

	// Initialization complete.
	s_isInitialized = MD_TRUE;
}

struct MdWindowData* mdWindowCreate(u32 width, u32 height, const char* title)
{
	MD_ASSERT(s_isInitialized == MD_TRUE);

	struct MdWindowData* pWindowData = MD_MALLOC(struct MdWindowData);
	MD_ASSERT(pWindowData != MD_NULL);
	pWindowData->width	= width;
	pWindowData->height = height;
	pWindowData->title	= title;

	pWindowData->pInternal = MD_MALLOC(struct LinuxWindowData);
	MD_ASSERT(pWindowData->pInternal != MD_NULL);

	struct LinuxWindowData* pLinuxData = (struct LinuxWindowData*)pWindowData->pInternal;
	pLinuxData->pDisplay			   = XOpenDisplay(NULL);
	MD_ASSERT(pLinuxData->pDisplay != MD_NULL);
	pLinuxData->screen = DefaultScreen(pLinuxData->pDisplay);

	pLinuxData->window = XCreateSimpleWindow(pLinuxData->pDisplay,
											 RootWindow(pLinuxData->pDisplay, pLinuxData->screen),
											 0,
											 0,
											 width,
											 height,
											 1,
											 BlackPixel(pLinuxData->pDisplay, pLinuxData->screen),
											 WhitePixel(pLinuxData->pDisplay, pLinuxData->screen));

	XSelectInput(pLinuxData->pDisplay, pLinuxData->window, ExposureMask | KeyPressMask);
	XMapWindow(pLinuxData->pDisplay, pLinuxData->window);
	XStoreName(pLinuxData->pDisplay, pLinuxData->window, title);

	XSizeHints sizeHints;
	mdMemorySet(&sizeHints, 0, sizeof(XSizeHints));
	sizeHints.flags		 = PMinSize | PMaxSize;
	sizeHints.min_width	 = width;
	sizeHints.min_height = height;
	sizeHints.max_width	 = width;
	sizeHints.max_height = height;
	XSetWMNormalHints(pLinuxData->pDisplay, pLinuxData->window, &sizeHints);

	Atom wmDeleteMessage = XInternAtom(pLinuxData->pDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(pLinuxData->pDisplay, pLinuxData->window, &wmDeleteMessage, 1);

	return pWindowData;
}

struct MdWindowEvent mdWindowPollEvents(struct MdWindowData* pWindowData)
{
	MEED_WINDOW_UTILS_ASSERTION();

	struct LinuxWindowData* pLinuxData = (struct LinuxWindowData*)pWindowData->pInternal;

	XNextEvent(pLinuxData->pDisplay, &event);
	struct MdWindowEvent windowEvent;
	mdMemorySet(&windowEvent, 0, sizeof(struct MdWindowEvent));
	windowEvent.type = MD_WINDOW_EVENT_TYPE_NONE;

	if (event.type == ClientMessage)
	{
		if (event.xclient.data.l[0] == XInternAtom(pLinuxData->pDisplay, "WM_DELETE_WINDOW", False))
		{
			pWindowData->shouldClose = MD_TRUE;
			windowEvent.type		 = MD_WINDOW_EVENT_TYPE_CLOSE;
		}
	}

	return windowEvent;
}

#if MD_USE_VULKAN
VkResult mdWindowCreateVulkanSurface(struct MdWindowData* pWindowData, VkInstance instance, VkSurfaceKHR* pSurface)
{
	MEED_WINDOW_UTILS_ASSERTION();

	struct LinuxWindowData* pLinuxData = (struct LinuxWindowData*)pWindowData->pInternal;

	VkSurfaceKHR			   surface;
	VkXlibSurfaceCreateInfoKHR createInfo = {};
	createInfo.sType					  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	createInfo.dpy						  = pLinuxData->pDisplay;
	createInfo.window					  = pLinuxData->window;

	return vkCreateXlibSurfaceKHR(instance, &createInfo, MD_NULL, pSurface);
}

void mdWindowDestroyVulkanSurface(struct MdWindowData* pWindowData, VkInstance instance, VkSurfaceKHR surface)
{
	MEED_WINDOW_UTILS_ASSERTION();

	vkDestroySurfaceKHR(instance, surface, MD_NULL);
}

#endif

void mdWindowDestroy(struct MdWindowData* pWindowData)
{
	MEED_WINDOW_UTILS_ASSERTION();

	struct LinuxWindowData* pLinuxData = (struct LinuxWindowData*)pWindowData->pInternal;

	XDestroyWindow(pLinuxData->pDisplay, pLinuxData->window);
	XFlush(pLinuxData->pDisplay);
	XCloseDisplay(pLinuxData->pDisplay);

	MD_FREE(pWindowData->pInternal, struct LinuxWindowData);
	MD_FREE(pWindowData, struct MdWindowData);
}

void mdWindowShutdown()
{
	MD_ASSERT(s_isInitialized == MD_TRUE);

	// Shutdown complete.
	s_isInitialized = MD_FALSE;
}

#endif // PLATFORM_IS_LINUX && !MD_USE_OPENGL