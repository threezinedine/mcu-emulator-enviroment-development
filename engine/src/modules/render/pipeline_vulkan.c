#if MD_USE_VULKAN

#include "MEEDEngine/modules/render/pipeline.h"
#include "MEEDEngine/modules/render/shader.h"
#include "vulkan_common.h"

static void createLayout(struct MdPipeline* pPipeline);
static void createPipeline(struct MdPipeline* pPipeline);

static void freeInternalPipeline(void*);
static void deleteShaderResources(void*);

struct MdPipeline* mdPipelineCreate(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	// Implementation of pipeline creation using Vulkan
	struct MdPipeline* pPipeline = MD_MALLOC(struct MdPipeline);
	MD_ASSERT(pPipeline != MD_NULL);

	pPipeline->vertexShaderPath	  = vertexShaderPath;
	pPipeline->fragmentShaderPath = fragmentShaderPath;
	pPipeline->pReleaseStack	  = mdReleaseStackCreate();

	pPipeline->pInternal = MD_MALLOC(struct VulkanPipeline);
	MD_ASSERT(pPipeline->pInternal != MD_NULL);

	mdReleaseStackPush(pPipeline->pReleaseStack, pPipeline, freeInternalPipeline);

	struct VulkanPipeline* pVulkanPipeline = (struct VulkanPipeline*)pPipeline->pInternal;
	mdMemorySet(pVulkanPipeline, 0, sizeof(struct VulkanPipeline));

	pVulkanPipeline->pVertexShader	 = mdShaderCreate(MD_SHADER_TYPE_VERTEX, vertexShaderPath);
	pVulkanPipeline->pFragmentShader = mdShaderCreate(MD_SHADER_TYPE_FRAGMENT, fragmentShaderPath);
	mdReleaseStackPush(pPipeline->pReleaseStack, pPipeline, deleteShaderResources);

	createLayout(pPipeline);
	createPipeline(pPipeline);

	return pPipeline;
}

static void freeInternalPipeline(void* pData)
{
	MD_ASSERT(pData != MD_NULL);
	struct MdPipeline* pPipeline = (struct MdPipeline*)pData;

	MD_FREE(pPipeline->pInternal, struct VulkanPipeline);
}

static void deleteShaderResources(void* pData)
{
	MD_ASSERT(pData != MD_NULL);
	struct MdPipeline* pPipeline = (struct MdPipeline*)pData;

	struct VulkanPipeline* pVulkanPipeline = (struct VulkanPipeline*)pPipeline->pInternal;

	mdShaderDestroy(pVulkanPipeline->pVertexShader);
	mdShaderDestroy(pVulkanPipeline->pFragmentShader);
}

static void destroyLayout(void* pData);
static void createLayout(struct MdPipeline* pPipeline)
{
	MD_ASSERT(pPipeline != MD_NULL);
	MD_ASSERT(g_vulkan != MD_NULL);
	MD_ASSERT(g_vulkan->device != MD_NULL);

	struct VulkanPipeline* pVulkanPipeline = (struct VulkanPipeline*)pPipeline->pInternal;

	VkPipelineLayoutCreateInfo layoutCreateInfo = {};
	layoutCreateInfo.sType						= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutCreateInfo.pPushConstantRanges		= MD_NULL;
	layoutCreateInfo.pushConstantRangeCount		= 0;
	layoutCreateInfo.pSetLayouts				= MD_NULL;
	layoutCreateInfo.setLayoutCount				= 0;

	VK_ASSERT(vkCreatePipelineLayout(g_vulkan->device, &layoutCreateInfo, MD_NULL, &pVulkanPipeline->layout));
	mdReleaseStackPush(pPipeline->pReleaseStack, pPipeline, destroyLayout);
}

static void destroyLayout(void* pData)
{
	MD_ASSERT(g_vulkan != MD_NULL);
	MD_ASSERT(g_vulkan->device != MD_NULL);
	MD_ASSERT(pData != MD_NULL);
	struct MdPipeline*	   pPipeline	   = (struct MdPipeline*)pData;
	struct VulkanPipeline* pVulkanPipeline = (struct VulkanPipeline*)pPipeline->pInternal;

	vkDestroyPipelineLayout(g_vulkan->device, pVulkanPipeline->layout, MD_NULL);
}

static void destroyPipeline(void* pData);
static void createPipeline(struct MdPipeline* pPipeline)
{
	MD_ASSERT(pPipeline != MD_NULL);
	MD_ASSERT(g_vulkan != MD_NULL);
	MD_ASSERT(g_vulkan->device != MD_NULL);
	// MD_ASSERT(g_vulkan->renderPass != MD_NULL);

	struct VulkanPipeline* pVulkanPipeline = (struct VulkanPipeline*)pPipeline->pInternal;

	MD_ASSERT(pVulkanPipeline->pVertexShader != MD_NULL);
	MD_ASSERT(pVulkanPipeline->pFragmentShader != MD_NULL);
	MD_ASSERT(pVulkanPipeline->layout != MD_NULL);

	struct VulkanShader* pVertexVulkanShader = (struct VulkanShader*)pVulkanPipeline->pVertexShader->pInternal;
	VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
	vertexShaderStageInfo.sType							  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage							  = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.pName							  = "main";
	vertexShaderStageInfo.module						  = pVertexVulkanShader->module;

	struct VulkanShader* pFragmentVulkanShader = (struct VulkanShader*)pVulkanPipeline->pFragmentShader->pInternal;
	VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
	fragmentShaderStageInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageInfo.stage							= VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageInfo.pName							= "main";
	fragmentShaderStageInfo.module							= pFragmentVulkanShader->module;

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType								 = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount		 = 0;
	vertexInputInfo.pVertexBindingDescriptions			 = MD_NULL;
	vertexInputInfo.vertexAttributeDescriptionCount		 = 0;
	vertexInputInfo.pVertexAttributeDescriptions		 = MD_NULL;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType									 = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology								 = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable				 = VK_FALSE;

#if 0
	VkPipelineTessellationStateCreateInfo tessellationState = {};
	tessellationState.sType									= VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	tessellationState.patchControlPoints					= 0;
#endif

	VkViewport viewport = {};
	viewport.x			= 0.0f;
	viewport.y			= 0.0f;
	viewport.width		= (f32)g_vulkan->extent.width;
	viewport.height		= (f32)g_vulkan->extent.height;
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;

	VkRect2D scissor = {};
	scissor.offset	 = (VkOffset2D){0, 0};
	scissor.extent	 = g_vulkan->extent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType								= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount						= 1;
	viewportState.pViewports						= &viewport;
	viewportState.scissorCount						= 1;
	viewportState.pScissors							= &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType								  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable						  = VK_FALSE;
	rasterizer.rasterizerDiscardEnable				  = VK_FALSE;
	rasterizer.polygonMode							  = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth							  = 1.0f;
	rasterizer.cullMode								  = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace							  = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable						  = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType								   = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable				   = VK_FALSE;
	multisampling.rasterizationSamples				   = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading					   = 1.0f;
	multisampling.pSampleMask						   = MD_NULL;
	multisampling.alphaToCoverageEnable				   = VK_FALSE;
	multisampling.alphaToOneEnable					   = VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType								   = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable					   = VK_TRUE;
	depthStencil.depthWriteEnable					   = VK_TRUE;
	depthStencil.depthCompareOp						   = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable				   = VK_FALSE;
	depthStencil.stencilTestEnable					   = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType								  = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.attachmentCount					  = 1;
	colorBlending.pAttachments						  = &colorBlendAttachment;
	colorBlending.logicOpEnable						  = VK_FALSE;
	colorBlending.logicOp							  = VK_LOGIC_OP_COPY;
	colorBlending.blendConstants[0]					  = 0.0f;
	colorBlending.blendConstants[1]					  = 0.0f;
	colorBlending.blendConstants[2]					  = 0.0f;
	colorBlending.blendConstants[3]					  = 0.0f;

	VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType							  = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount				  = MD_ARRAY_SIZE(dynamicStates);
	dynamicState.pDynamicStates					  = dynamicStates;

	VkFormat colorFormats[] = {g_vulkan->surfaceFormat.format};

	VkPipelineRenderingCreateInfo pipelineRenderingInfo = {};
	pipelineRenderingInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	pipelineRenderingInfo.colorAttachmentCount			= 1;
	pipelineRenderingInfo.pColorAttachmentFormats		= colorFormats;
	pipelineRenderingInfo.depthAttachmentFormat			= VK_FORMAT_D32_SFLOAT;
	pipelineRenderingInfo.stencilAttachmentFormat		= VK_FORMAT_UNDEFINED;

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType						= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext						= &pipelineRenderingInfo;
	pipelineCreateInfo.stageCount					= MD_ARRAY_SIZE(shaderStages);
	pipelineCreateInfo.pStages						= shaderStages;
	pipelineCreateInfo.pVertexInputState			= &vertexInputInfo;
	pipelineCreateInfo.pInputAssemblyState			= &inputAssembly;
	pipelineCreateInfo.pTessellationState			= MD_NULL;
	pipelineCreateInfo.pViewportState				= &viewportState;
	pipelineCreateInfo.pRasterizationState			= &rasterizer;
	pipelineCreateInfo.pMultisampleState			= &multisampling;
	pipelineCreateInfo.pDepthStencilState			= &depthStencil;
	pipelineCreateInfo.pColorBlendState				= &colorBlending;
	pipelineCreateInfo.pDynamicState				= &dynamicState;
	pipelineCreateInfo.layout						= pVulkanPipeline->layout;
#if 0
	pipelineCreateInfo.renderPass					= g_vulkan->renderPass;
#else
	pipelineCreateInfo.renderPass = MD_NULL;
#endif
	pipelineCreateInfo.subpass = 0;

	VK_ASSERT(vkCreateGraphicsPipelines(
		g_vulkan->device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, MD_NULL, &pVulkanPipeline->pipeline));

	mdReleaseStackPush(pPipeline->pReleaseStack, pPipeline, destroyPipeline);
}

static void destroyPipeline(void* pData)
{
	MD_ASSERT(g_vulkan != MD_NULL);
	MD_ASSERT(g_vulkan->device != MD_NULL);
	MD_ASSERT(pData != MD_NULL);
	struct MdPipeline*	   pPipeline	   = (struct MdPipeline*)pData;
	struct VulkanPipeline* pVulkanPipeline = (struct VulkanPipeline*)pPipeline->pInternal;

	vkDestroyPipeline(g_vulkan->device, pVulkanPipeline->pipeline, MD_NULL);
}

void mdPipelineUse(struct MdPipeline* pPipeline)
{
	MD_ASSERT(pPipeline != MD_NULL);
	MD_ASSERT(g_vulkan != MD_NULL);
	MD_ASSERT(g_vulkan->graphicsCommandBuffers != MD_NULL);

	struct VulkanPipeline* pVulkanPipeline = (struct VulkanPipeline*)pPipeline->pInternal;

	vkCmdBindPipeline(g_vulkan->graphicsCommandBuffers[g_vulkan->currentFrame],
					  VK_PIPELINE_BIND_POINT_GRAPHICS,
					  pVulkanPipeline->pipeline);
}

void mdPipelineDestroy(struct MdPipeline* pPipeline)
{
	MD_ASSERT(pPipeline != MD_NULL);
	MD_ASSERT(pPipeline->pReleaseStack != MD_NULL);

	mdReleaseStackDestroy(pPipeline->pReleaseStack);
	MD_FREE(pPipeline, struct MdPipeline);
}

#endif // MD_USE_VULKAN