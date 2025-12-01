#if MD_USE_VULKAN

#include "MEEDEngine/modules/render/shader.h"
#include "vulkan_common.h"

struct MdShader* mdShaderCreate(enum MdShaderType type, const char* filePath)
{
	struct MdShader* pShader = MD_MALLOC(struct MdShader);
	MD_ASSERT(pShader != MD_NULL);
	pShader->type = type;

	pShader->pInternal = MD_MALLOC(struct VulkanShader);
	MD_ASSERT(pShader->pInternal != MD_NULL);

	struct VulkanShader* pVulkanShader = (struct VulkanShader*)pShader->pInternal;
	mdMemorySet(pVulkanShader, 0, sizeof(struct VulkanShader));

	// Load SPIR-V binary from file
	struct MdFileData* pFile = mdFileOpen(filePath, MD_FILE_MODE_READ);
	MD_ASSERT_MSG(pFile != MD_NULL && pFile->isOpen, "Failed to open shader file \"%s\".", filePath);

	MD_ASSERT(g_vulkan != MD_NULL);
	MD_ASSERT(g_vulkan->device != MD_NULL);

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType					= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize					= pFile->size;
	createInfo.pCode					= (const u32*)pFile->content;
	VK_ASSERT(vkCreateShaderModule(g_vulkan->device, &createInfo, MD_NULL, &pVulkanShader->module));

	mdFileClose(pFile);

	return pShader;
}

void mdShaderDestroy(struct MdShader* pShader)
{
	MD_ASSERT(pShader != MD_NULL);
	MD_ASSERT(g_vulkan != MD_NULL);
	MD_ASSERT(g_vulkan->device != MD_NULL);

	struct VulkanShader* pVulkanShader = (struct VulkanShader*)pShader->pInternal;
	vkDestroyShaderModule(g_vulkan->device, pVulkanShader->module, MD_NULL);

	MD_FREE(pShader->pInternal, struct VulkanShader);
	MD_FREE(pShader, struct MdShader);
}

#endif // MD_USE_VULKAN