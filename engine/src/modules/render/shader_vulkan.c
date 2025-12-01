#if MEED_USE_VULKAN

#include "MEEDEngine/modules/render/shader.h"
#include "vulkan_common.h"

struct MdShader* mdShaderCreate(enum MdShaderType type, const char* filePath)
{
	struct MdShader* pShader = MEED_MALLOC(struct MdShader);
	MEED_ASSERT(pShader != MEED_NULL);
	pShader->type = type;

	pShader->pInternal = MEED_MALLOC(struct VulkanShader);
	MEED_ASSERT(pShader->pInternal != MEED_NULL);

	struct VulkanShader* pVulkanShader = (struct VulkanShader*)pShader->pInternal;
	mdMemorySet(pVulkanShader, 0, sizeof(struct VulkanShader));

	// Load SPIR-V binary from file
	struct MdFileData* pFile = mdFileOpen(filePath, MD_FILE_MODE_READ);
	MEED_ASSERT_MSG(pFile != MEED_NULL && pFile->isOpen, "Failed to open shader file \"%s\".", filePath);

	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(g_vulkan->device != MEED_NULL);

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType					= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize					= pFile->size;
	createInfo.pCode					= (const u32*)pFile->content;
	VK_ASSERT(vkCreateShaderModule(g_vulkan->device, &createInfo, MEED_NULL, &pVulkanShader->module));

	mdFileClose(pFile);

	return pShader;
}

void mdShaderDestroy(struct MdShader* pShader)
{
	MEED_ASSERT(pShader != MEED_NULL);
	MEED_ASSERT(g_vulkan != MEED_NULL);
	MEED_ASSERT(g_vulkan->device != MEED_NULL);

	struct VulkanShader* pVulkanShader = (struct VulkanShader*)pShader->pInternal;
	vkDestroyShaderModule(g_vulkan->device, pVulkanShader->module, MEED_NULL);

	MEED_FREE(pShader->pInternal, struct VulkanShader);
	MEED_FREE(pShader, struct MdShader);
}

#endif // MEED_USE_VULKAN