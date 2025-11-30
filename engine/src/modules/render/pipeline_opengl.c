#if MEED_USE_OPENGL

#include "MEEDEngine/modules/render/pipeline.h"

struct MEEDPipeline* meedPipelineCreate(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	MEED_UNUSED(vertexShaderPath);
	MEED_UNUSED(fragmentShaderPath);

	// OpenGL pipeline creation logic would go here.
	return MEED_NULL;
}

void meedPipelineUse(struct MEEDPipeline* pPipeline)
{
	MEED_UNUSED(pPipeline);

	// OpenGL pipeline binding logic would go here.
}

void meedPipelineDestroy(struct MEEDPipeline* pPipeline)
{
	MEED_UNUSED(pPipeline);

	// OpenGL pipeline destruction logic would go here.
}

#endif // MEED_USE_OPENGL