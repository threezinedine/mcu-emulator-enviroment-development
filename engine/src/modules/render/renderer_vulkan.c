#include "MEEDEngine/modules/release_stack/release_stack.h"
#include "MEEDEngine/modules/render/renderer.h"
#include "MEEDEngine/platforms/platforms.h"
#include "vulkan_common.h"
#include <vulkan/vulkan.h>

b8 s_isInitialized = MEED_FALSE;

static const char* instanceExtensions[] = {
	"VK_KHR_surface",
	"VK_KHR_xcb_surface",
	"VK_EXT_debug_utils",
};

static const char* layers[] = {
#if MEED_DEBUG
	"VK_LAYER_KHRONOS_validation",
#endif
};

/// @brief The context of the Vulkan renderer.
struct MEEDVulkan
{
	VkInstance instance;
#if MEED_DEBUG
	VkDebugUtilsMessengerEXT debugMessenger;
#endif
};

struct MEEDVulkan* g_vulkan = MEED_NULL; // Global Vulkan instance

static struct MEEDReleaseStack* s_releaseStack = MEED_NULL; // Global release stack instance

static void createVulkanInstance();
#if MEED_DEBUG
static void createValidationLayers();
#endif

static void deleteGlobalVulkanInstance(void*);

void meedRenderInitialize()
{
	MEED_ASSERT_MSG(!s_isInitialized, "Rendering module is already initialized.");
	MEED_ASSERT(g_vulkan == MEED_NULL);
	MEED_ASSERT(s_releaseStack == MEED_NULL);
	// Implementation of rendering module initialization
	meedWindowInitialize();
	s_releaseStack = meedReleaseStackCreate();

	g_vulkan = MEED_MALLOC(struct MEEDVulkan);
	meedReleaseStackPush(s_releaseStack, MEED_NULL, deleteGlobalVulkanInstance);

	createVulkanInstance();
#if MEED_DEBUG
	createValidationLayers();
#endif

	s_isInitialized = MEED_TRUE;
}

static void deleteGlobalVulkanInstance(void* pData)
{
	MEED_UNUSED(pData);

	MEED_ASSERT(g_vulkan != MEED_NULL);

	MEED_FREE(g_vulkan, struct MEEDVulkan);
}

void meedRenderShutdown()
{
	MEED_ASSERT_MSG(s_isInitialized, "Rendering module is not initialized.");
	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(s_releaseStack != MEED_NULL);
	// Implementation of rendering module shutdown

	meedReleaseStackDestroy(s_releaseStack);
	meedWindowShutdown();
	s_isInitialized = MEED_FALSE;
}

static void deleteVulkanInstance(void*);

static void createVulkanInstance()
{
	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(s_releaseStack != MEED_NULL);

	VkApplicationInfo appInfo  = {};
	appInfo.sType			   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName   = "MEED Engine Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName		   = "MEED Engine";
	appInfo.engineVersion	   = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion		   = VK_API_VERSION_1_3;

	VkInstanceCreateInfo instanceCreateInfo	   = {};
	instanceCreateInfo.sType				   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo		   = &appInfo;
	instanceCreateInfo.enabledExtensionCount   = MEED_ARRAY_SIZE(instanceExtensions);
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions;
	instanceCreateInfo.enabledLayerCount	   = MEED_ARRAY_SIZE(layers);
	instanceCreateInfo.ppEnabledLayerNames	   = layers;

	VK_ASSERT(vkCreateInstance(&instanceCreateInfo, MEED_NULL, &g_vulkan->instance));
	meedReleaseStackPush(s_releaseStack, MEED_NULL, deleteVulkanInstance);
}

static void deleteVulkanInstance(void* pData)
{
	MEED_UNUSED(pData);

	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(g_vulkan->instance != MEED_NULL);

	vkDestroyInstance(g_vulkan->instance, MEED_NULL);
}

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT	   messageSeverity,
												   VkDebugUtilsMessageTypeFlagsEXT			   messageType,
												   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
												   void*									   pUserData)
{
	struct MEEDPlatformConsoleConfig config;

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_WHITE;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_GREEN;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_YELLOW;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_RED;
		break;
	default:
		config.color = MEED_CONSOLE_COLOR_RESET;
	}

	meedPlatformSetConsoleConfig(config);
	meedPlatformFPrint("VULKAN VALIDATION LAYER: %s", pCallbackData->pMessage);

	return VK_FALSE;
}

#if MEED_DEBUG
static void deleteDebugMessenger(void*);

static void createValidationLayers()
{
	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(g_vulkan->instance != MEED_NULL);
	MEED_ASSERT(s_releaseStack != MEED_NULL);

	VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};

	messengerCreateInfo.sType			= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
										  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
										  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	messengerCreateInfo.pfnUserCallback = vulkanDebugCallback;

	PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerEXT =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_vulkan->instance, "vkCreateDebugUtilsMessengerEXT");

	MEED_ASSERT(createDebugUtilsMessengerEXT != MEED_NULL);
	VK_ASSERT(
		createDebugUtilsMessengerEXT(g_vulkan->instance, &messengerCreateInfo, MEED_NULL, &g_vulkan->debugMessenger));
	meedReleaseStackPush(s_releaseStack, MEED_NULL, deleteDebugMessenger);
}

static void deleteDebugMessenger(void* pData)
{
	MEED_UNUSED(pData);

	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(g_vulkan->instance != MEED_NULL);
	MEED_ASSERT(g_vulkan->debugMessenger != MEED_NULL);

	PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerEXT =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_vulkan->instance,
																   "vkDestroyDebugUtilsMessengerEXT");

	MEED_ASSERT(destroyDebugUtilsMessengerEXT != MEED_NULL);
	destroyDebugUtilsMessengerEXT(g_vulkan->instance, g_vulkan->debugMessenger, MEED_NULL);
}
#endif