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

static const char* deviceExtensions[] = {
	"VK_KHR_swapchain",
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
	VkPhysicalDevice physicalDevice;
};

struct MEEDVulkan* g_vulkan = MEED_NULL; // Global Vulkan instance

static struct MEEDReleaseStack* s_releaseStack = MEED_NULL; // Global release stack instance

static void createVulkanInstance();
#if MEED_DEBUG
static void createValidationLayers();
#endif
static void choosePhysicalDevice();

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
	choosePhysicalDevice();

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
	char							 severityString[10];

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_WHITE;
		meedPlatformBufferedPrint(severityString, sizeof(severityString), "%7s", "VERBOSE");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_GREEN;
		meedPlatformBufferedPrint(severityString, sizeof(severityString), "%7s", "INFO");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_YELLOW;
		meedPlatformBufferedPrint(severityString, sizeof(severityString), "%7s", "WARNING");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		config.color = MEED_CONSOLE_COLOR_RED;
		meedPlatformBufferedPrint(severityString, sizeof(severityString), "%7s", "ERROR");
		break;
	default:
		config.color = MEED_CONSOLE_COLOR_RESET;
	}

	meedPlatformSetConsoleConfig(config);
	meedPlatformFPrint("[%7s] - [%s] - %s\n", "VULKAN", severityString, pCallbackData->pMessage);

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

static u32 isDeviceSuitable(VkPhysicalDevice device);

static void choosePhysicalDevice()
{
	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(g_vulkan->instance != MEED_NULL);
	MEED_ASSERT(s_releaseStack != MEED_NULL);

	u32 devicesCount = 0;
	VK_ASSERT(vkEnumeratePhysicalDevices(g_vulkan->instance, &devicesCount, MEED_NULL));
	MEED_ASSERT_MSG(devicesCount > 0, "Failed to find GPUs with Vulkan support.");

	VkPhysicalDevice* pDevices = MEED_MALLOC_ARRAY(VkPhysicalDevice, devicesCount);
	VK_ASSERT(vkEnumeratePhysicalDevices(g_vulkan->instance, &devicesCount, pDevices));

	u32 highestScore	= 0;
	i32 bestDeviceIndex = -1;

	for (u32 deviceIndex = 0u; deviceIndex < devicesCount; ++deviceIndex)
	{
		u32 score = isDeviceSuitable(pDevices[deviceIndex]);
		if (score > highestScore)
		{
			highestScore	= score;
			bestDeviceIndex = deviceIndex;
		}
	}

	MEED_ASSERT_MSG(bestDeviceIndex != -1, "Failed to find a suitable GPU.");
	g_vulkan->physicalDevice = pDevices[bestDeviceIndex];
	MEED_FREE_ARRAY(pDevices, VkPhysicalDevice, devicesCount);
}

static b8  checkDeviceExtensionsSupport(VkPhysicalDevice device);
static u32 rateDevice(VkPhysicalDevice device);

static u32 isDeviceSuitable(VkPhysicalDevice device)
{
	if (!checkDeviceExtensionsSupport(device))
	{
		return 0;
	}

	return rateDevice(device);
}

static b8 checkDeviceExtensionsSupport(VkPhysicalDevice device)
{
	u32 deviceExtensionsCount = 0;
	vkEnumerateDeviceExtensionProperties(device, MEED_NULL, &deviceExtensionsCount, MEED_NULL);
	MEED_ASSERT_MSG(deviceExtensionsCount > 0, "Failed to get device extension properties.");

	VkExtensionProperties* pAvailableExtensions = MEED_MALLOC_ARRAY(VkExtensionProperties, deviceExtensionsCount);
	vkEnumerateDeviceExtensionProperties(device, MEED_NULL, &deviceExtensionsCount, pAvailableExtensions);

	for (u32 requiredEXTIndex = 0u; requiredEXTIndex < MEED_ARRAY_SIZE(deviceExtensions); ++requiredEXTIndex)
	{
		b8 extensionFound = MEED_FALSE;
		for (u32 availableEXTIndex = 0u; availableEXTIndex < deviceExtensionsCount; ++availableEXTIndex)
		{
			if (meedPlatformStringCompare(deviceExtensions[requiredEXTIndex],
										  pAvailableExtensions[availableEXTIndex].extensionName) == 0)
			{
				extensionFound = MEED_TRUE;
				break;
			}
		}

		if (!extensionFound)
		{
			MEED_FREE_ARRAY(pAvailableExtensions, VkExtensionProperties, deviceExtensionsCount);
			return MEED_FALSE;
		}
	}

	MEED_FREE_ARRAY(pAvailableExtensions, VkExtensionProperties, deviceExtensionsCount);
	return MEED_TRUE;
}

static u32 rateDevice(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);

	u32 score = 0;

	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}
	else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		score += 500;
	}

	if (features.geometryShader)
	{
		score += 100;
	}

	score += properties.limits.maxImageDimension2D;

	return score;
}