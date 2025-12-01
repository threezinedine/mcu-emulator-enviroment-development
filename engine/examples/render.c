#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdPlatformMemoryInitialize();
	mdWindowInitialize();
	struct MEEDWindowData* pWindow = mdWindowCreate(1600, 1400, "MEEDEngine Vulkan Renderer Example");

	mdRenderInitialize(pWindow);

#if MEED_USE_VULKAN
	struct MEEDPipeline* pPipeline =
		mdPipelineCreate(MEED_STRINGIFY(PROJECT_BASE_DIR) "/engine/build/debug/shaders/triangle.vert.spv",
						 MEED_STRINGIFY(PROJECT_BASE_DIR) "/engine/build/debug/shaders/triangle.frag.spv");
#elif MEED_USE_OPENGL
	struct MEEDPipeline* pPipeline =
		mdPipelineCreate(MEED_STRINGIFY(PROJECT_BASE_DIR) "/engine/assets/shaders/opengl/triangle.vert",
						 MEED_STRINGIFY(PROJECT_BASE_DIR) "/engine/assets/shaders/opengl/triangle.frag");
#else
#error "No rendering backend selected."
#endif

	while (!pWindow->shouldClose)
	{
		mdWindowPollEvents(pWindow);

		mdRenderClearScreen((struct MEEDColor){0.1f, 0.1f, 0.1f, 1.0f});

		mdRenderStartFrame();

		// Rendering commands below...
		mdPipelineUse(pPipeline);
		mdRenderDraw(3, 1, 0, 0);

		// Rendering commands above...

		mdRenderEndFrame();

		mdRenderPresent();
	}

	mdRenderWaitIdle();

	mdPipelineDestroy(pPipeline);
	mdWindowDestroy(pWindow);

	mdRenderShutdown();
	mdWindowShutdown();
	mdPlatformMemoryShutdown();
	return 0;
}