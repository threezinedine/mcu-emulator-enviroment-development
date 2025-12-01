#pragma once
#include "common.h"

#if __cplusplus
extern "C" {
#endif

#if MD_USE_VULKAN
#include <vulkan/vulkan.h>
#endif

/**
 * @file window.h
 * The utilities for managing the windowing system inside the `MEEDEngine`.
 *
 * @example
 * ```
 * struct MdWindowData* pWindowData = mdWindowCreate(800, 600, "MEED Application Window");
 *
 * // Main loop
 * while (!pWindowData->shouldClose)
 * {
 *     mdWindowPollEvents(pWindowData);
 *     // Rendering and other logic here...
 * }
 *
 * mdWindowDestroy(pWindowData);
 * ```
 */

/**
 * Enumeration of possible window event types.
 */
enum MdWindowEventType
{
	MD_WINDOW_EVENT_TYPE_NONE,	///< No event, but the polling always returns this when there are no events.
	MD_WINDOW_EVENT_TYPE_CLOSE, ///< The window close event.
	MD_WINDOW_EVENT_TYPE_COUNT,
};

/**
 * Represents a window event.
 * (Currently empty, can be expanded in the future for event handling.)
 */
struct MdWindowEvent
{
	enum MdWindowEventType type; ///< The type of the window event.
};

/**
 * Needed information for working with the windowing system.
 */
struct MD_BINDING MdWindowData
{
	void* pInternal; ///< Used for storing custom windowing system data (e.g Windows, Linux, MacOS, SDL, GLFW, etc.)

	u32			width;		 ///< The width of the window.
	u32			height;		 ///< The height of the window.
	const char* title;		 ///< The title of the window.
	b8			shouldClose; ///< Flag indicating whether the window should close.
};

/**
 * Initialize the windowing system. Must be called before using any window-related functions.
 */
void mdWindowInitialize();

/**
 * Start a new window (display) for rendering.
 *
 * @param width The width of the window in pixels.
 * @param height The height of the window in pixels.
 * @param title The title of the window as a null-terminated string.
 *
 * @return A pointer to the created window data.
 */
struct MdWindowData* mdWindowCreate(u32 width, u32 height, const char* title);

#if MD_USE_VULKAN
/**
 * Get the Vulkan surface for the specified window.
 *
 * @param pWindowData A pointer to the window data.
 * @param instance The Vulkan instance.
 * @param pSurface A pointer to store the created Vulkan surface.
 */
VkResult mdWindowCreateVulkanSurface(struct MdWindowData* pWindowData, VkInstance instance, VkSurfaceKHR* pSurface);

/**
 * Destroy the Vulkan surface associated with the specified window.
 *
 * @param pWindowData A pointer to the window data.
 * @param instance The Vulkan instance.
 * @param surface The Vulkan surface to destroy.
 */
void mdWindowDestroyVulkanSurface(struct MdWindowData* pWindowData, VkInstance instance, VkSurfaceKHR surface);
#endif

/**
 * Poll for window events (e.g., input, close events).
 *
 * @param pWindowData A pointer to the window data to poll events from.
 * @return A `MdWindowEvent` structure containing the polled event information.
 */
struct MdWindowEvent mdWindowPollEvents(struct MdWindowData* pWindowData);

/**
 * Destroy a previously created window.
 *
 * @param pWindowData A pointer to the window data to destroy.
 */
void mdWindowDestroy(struct MdWindowData* pWindowData);

/**
 * Shutdown the windowing system. Must be called after all window-related functions are done.
 */
void mdWindowShutdown();

#if __cplusplus
}
#endif