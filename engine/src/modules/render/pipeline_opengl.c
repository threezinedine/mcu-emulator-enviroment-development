#if MEED_USE_OPENGL

#include "MEEDEngine/modules/release_stack/release_stack.h"
#include "MEEDEngine/modules/render/pipeline.h"
#include "MEEDEngine/modules/render/shader.h"
#include "opengl_common.h"

void freeInternalOpenGLPipeline(void* pData)
{
	MEED_ASSERT(pData != MEED_NULL);
	struct MdPipeline* pPipeline = (struct MdPipeline*)pData;

	MEED_FREE(pPipeline->pInternal, struct OpenGLPipeline);
}

static void deleteProgram(void* pData)
{
	MEED_ASSERT(pData != MEED_NULL);

	struct MdPipeline*	   pPipeline	   = (struct MdPipeline*)pData;
	struct OpenGLPipeline* pOpenGLPipeline = (struct OpenGLPipeline*)pPipeline->pInternal;
	GL_ASSERT(glDeleteProgram(pOpenGLPipeline->shaderProgram));
}

struct MdPipeline* mdPipelineCreate(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	struct MdPipeline* pPipeline = MEED_MALLOC(struct MdPipeline);
	MEED_ASSERT(pPipeline != MEED_NULL);
	mdMemorySet(pPipeline, 0, sizeof(struct MdPipeline));

	pPipeline->pReleaseStack	  = mdReleaseStackCreate();
	pPipeline->vertexShaderPath	  = vertexShaderPath;
	pPipeline->fragmentShaderPath = fragmentShaderPath;

	pPipeline->pInternal = MEED_MALLOC(struct OpenGLPipeline);
	MEED_ASSERT(pPipeline->pInternal != MEED_NULL);
	mdMemorySet(pPipeline->pInternal, 0, sizeof(struct OpenGLPipeline));
	mdReleaseStackPush(pPipeline->pReleaseStack, pPipeline, freeInternalOpenGLPipeline);

	struct OpenGLPipeline* pOpenGLPipeline = (struct OpenGLPipeline*)pPipeline->pInternal;

	GL_ASSERT(pOpenGLPipeline->shaderProgram = glCreateProgram());

	struct MdShader* pVertexShader	 = mdShaderCreate(MD_SHADER_TYPE_VERTEX, vertexShaderPath);
	struct MdShader* pFragmentShader = mdShaderCreate(MD_SHADER_TYPE_FRAGMENT, fragmentShaderPath);

	struct OpenGLShader* pOpenGLVertexShader   = (struct OpenGLShader*)pVertexShader->pInternal;
	struct OpenGLShader* pOpenGLFragmentShader = (struct OpenGLShader*)pFragmentShader->pInternal;

	GL_ASSERT(glAttachShader(pOpenGLPipeline->shaderProgram, pOpenGLVertexShader->shaderID));
	GL_ASSERT(glAttachShader(pOpenGLPipeline->shaderProgram, pOpenGLFragmentShader->shaderID));
	GL_ASSERT(glLinkProgram(pOpenGLPipeline->shaderProgram));

	u32 linkStatus;
	glGetProgramiv(pOpenGLPipeline->shaderProgram, GL_LINK_STATUS, (GLint*)&linkStatus);
	if (linkStatus != GL_TRUE)
	{
		char infoLog[512];
		glGetProgramInfoLog(pOpenGLPipeline->shaderProgram, 512, NULL, infoLog);
		MEED_ASSERT_MSG(MEED_FALSE, "Shader program linking failed: %s", infoLog);
	}

	mdShaderDestroy(pFragmentShader);
	mdShaderDestroy(pVertexShader);

	mdReleaseStackPush(pPipeline->pReleaseStack, pPipeline, deleteProgram);

	return pPipeline;
}

void mdPipelineUse(struct MdPipeline* pPipeline)
{
	MEED_ASSERT(pPipeline != MEED_NULL);
	struct OpenGLPipeline* pOpenGLPipeline = (struct OpenGLPipeline*)pPipeline->pInternal;
	GL_ASSERT(glUseProgram(pOpenGLPipeline->shaderProgram));
}

void mdPipelineDestroy(struct MdPipeline* pPipeline)
{
	MEED_ASSERT(pPipeline != MEED_NULL);

	mdReleaseStackDestroy(pPipeline->pReleaseStack);
	MEED_FREE(pPipeline, struct MdPipeline);
	pPipeline = MEED_NULL;
}

#endif // MEED_USE_OPENGL