#include "MEEDEngine/MEEDEngine.h"

struct MdWindowData* pWindowData = MD_NULL;
struct MdPipeline*	 pPipeline	 = MD_NULL;

void mainLoop();

int main(void)
{
	mdMemoryInitialize();
	mdWindowInitialize();

	struct MdConsoleConfig config;
	config.color = MD_CONSOLE_COLOR_GREEN;
	mdSetConsoleConfig(config);

	pWindowData = mdWindowCreate(800, 600, "MEED Application Window");
	mdRenderInitialize(pWindowData);

#if PLATFORM_IS_WEB
	pPipeline = mdPipelineCreate("shaders/triangle.vert", "shaders/triangle.frag");
#elif MD_USE_VULKAN
	pPipeline = mdPipelineCreate(MD_STRINGIFY(PROJECT_BASE_DIR) "/app/build/debug/shaders/triangle.vert.spv",
								 MD_STRINGIFY(PROJECT_BASE_DIR) "/app/build/debug/shaders/triangle.frag.spv");
#elif MD_USE_OPENGL
	pPipeline = mdPipelineCreate(MD_STRINGIFY(PROJECT_BASE_DIR) "/engine/assets/shaders/opengl/triangle.vert",
								 MD_STRINGIFY(PROJECT_BASE_DIR) "/engine/assets/shaders/opengl/triangle.frag");
#else
#error "No rendering backend selected."
#endif

#if PLATFORM_IS_WEB
	emscripten_set_main_loop(mainLoop, 0, MD_TRUE);
#else
	while (pWindowData->shouldClose == MD_FALSE)
	{
		mainLoop();
	}
#endif

	mdRenderWaitIdle();

	mdPipelineDestroy(pPipeline);
	mdRenderShutdown();
	mdWindowDestroy(pWindowData);

	mdWindowShutdown();
	mdMemoryShutdown();
	return 0;
}

void mainLoop()
{
	struct MdWindowEvent windowEvent = mdWindowPollEvents(pWindowData);

	if (windowEvent.type == MD_WINDOW_EVENT_TYPE_CLOSE)
	{
		pWindowData->shouldClose = MD_TRUE;
	}

	mdRenderClearScreen((struct MdColor){0.2f, 0.3f, 0.3f, 1.0f});

	mdRenderStartFrame();

	mdPipelineUse(pPipeline);
	mdRenderDraw(3, 1, 0, 0);

	mdRenderEndFrame();

	mdRenderPresent();
}