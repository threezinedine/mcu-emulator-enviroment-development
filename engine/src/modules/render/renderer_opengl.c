#if MD_USE_OPENGL

#include "MEEDEngine/modules/render/render.h"
#include "opengl_common.h"

struct OpenGLRenderData* s_pRenderData = MD_NULL;

static u32		   vbo, vao;
static GLFWwindow* s_pWindow = MD_NULL;

void mdRenderInitialize(struct MdWindowData* pWindowData)
{
	MD_ASSERT(s_pRenderData == MD_NULL);

	s_pRenderData			   = MD_MALLOC(struct OpenGLRenderData);
	s_pRenderData->pWindowData = pWindowData;

	// clang-format off
	float vertices[] = {
		// positions        // colors
		0.0f,  -0.5f, 1.0f, 0.0f, 0.0f, // bottom
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // top right
		-0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
	};
	// clang-format on

	GL_ASSERT(glGenVertexArrays(1, &vao));
	GL_ASSERT(glGenBuffers(1, &vbo));
	GL_ASSERT(glBindVertexArray(vao));
	GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_ASSERT(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GL_ASSERT(glEnableVertexAttribArray(0));

	s_pWindow = (GLFWwindow*)glfwGetCurrentContext();
}

void mdRenderClearScreen(struct MdColor color)
{
	MD_ASSERT(s_pRenderData != MD_NULL);

	GL_ASSERT(glClearColor(color.r, color.g, color.b, color.a));
	GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void mdRenderStartFrame()
{
	MD_ASSERT(s_pRenderData != MD_NULL);

	GL_ASSERT(glBindVertexArray(vao));
}

void mdRenderEndFrame()
{
}

void mdRenderDraw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
{
	MEED_UNUSED(instanceCount);
	MEED_UNUSED(firstInstance);

	MD_ASSERT(s_pRenderData != MD_NULL);

	GL_ASSERT(glDrawArrays(GL_TRIANGLES, firstVertex, vertexCount));
}

void mdRenderPresent()
{
	MD_ASSERT(s_pRenderData != MD_NULL);
	MD_ASSERT(s_pWindow != MD_NULL);

	GL_ASSERT(glfwSwapBuffers(s_pWindow));
}

void mdRenderWaitIdle()
{
}

void mdRenderShutdown()
{
	MD_ASSERT(s_pRenderData != MD_NULL);

	MD_FREE(s_pRenderData, struct OpenGLRenderData);
	s_pRenderData = MD_NULL;
	s_pRenderData = MD_NULL;
}

#endif // MD_USE_OPENGL