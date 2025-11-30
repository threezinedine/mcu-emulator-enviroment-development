#if MEED_USE_OPENGL

#include "MEEDEngine/core/core.h"
#include "MEEDEngine/modules/render/render.h"
#include "MEEDEngine/platforms/platforms.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

struct OpenGLRenderData
{
	struct MEEDWindowData* pWindowData;
};

static struct OpenGLRenderData* s_pRenderData = MEED_NULL;

void meedRenderInitialize(struct MEEDWindowData* pWindowData)
{
	MEED_ASSERT(s_pRenderData == MEED_NULL);

	s_pRenderData			   = MEED_MALLOC(struct OpenGLRenderData);
	s_pRenderData->pWindowData = pWindowData;
}

void meedRenderClearScreen(struct MEEDColor color)
{
	MEED_ASSERT(s_pRenderData != MEED_NULL);

	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void meedRenderStartFrame()
{
}

void meedRenderEndFrame()
{
}

void meedRenderDraw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
{
	MEED_UNUSED(instanceCount);
	MEED_UNUSED(firstInstance);

	MEED_ASSERT(s_pRenderData != MEED_NULL);

	glDrawArrays(GL_TRIANGLES, firstVertex, vertexCount);
}

void meedRenderPresent()
{
	MEED_ASSERT(s_pRenderData != MEED_NULL);

	GLFWwindow* pWindow = (GLFWwindow*)s_pRenderData->pWindowData->pInternal;
	glfwSwapBuffers(pWindow);
}

void meedWaitIdle()
{
}

void meedRenderShutdown()
{
	MEED_ASSERT(s_pRenderData != MEED_NULL);

	MEED_FREE(s_pRenderData, struct OpenGLRenderData);
	s_pRenderData = MEED_NULL;
	s_pRenderData = MEED_NULL;
}

#endif // MEED_USE_OPENGL