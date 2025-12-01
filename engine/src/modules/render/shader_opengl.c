#if MD_USE_OPENGL

#include "MEEDEngine/modules/render/shader.h"
#include "opengl_common.h"

struct MdShader* mdShaderCreate(enum MdShaderType type, const char* shaderSource)
{
	struct MdShader* pShader = MD_MALLOC(struct MdShader);
	MD_ASSERT(pShader != MD_NULL);
	mdMemorySet(pShader, 0, sizeof(struct MdShader));

	pShader->type = type;

	pShader->pInternal = MD_MALLOC(struct OpenGLShader);
	MD_ASSERT(pShader->pInternal != MD_NULL);
	mdMemorySet(pShader->pInternal, 0, sizeof(struct OpenGLShader));

	struct OpenGLShader* pOpenGLShader = (struct OpenGLShader*)pShader->pInternal;

	struct MdFileData* pFile = mdFileOpen(shaderSource, MD_FILE_MODE_READ);
	MD_ASSERT_MSG(pFile != MD_NULL && pFile->isOpen, "Failed to open shader file \"%s\".", shaderSource);

	GLenum shaderType;

	switch (type)
	{
	case MD_SHADER_TYPE_VERTEX:
		shaderType = GL_VERTEX_SHADER;
		break;
	case MD_SHADER_TYPE_FRAGMENT:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case MD_SHADER_TYPE_COMPUTE:
		shaderType = GL_COMPUTE_SHADER;
		break;
	case MD_SHADER_TYPE_GEOMETRY:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	case MD_SHADER_TYPE_TESSELLATION:
		shaderType = GL_TESS_CONTROL_SHADER; // or GL_TESS_EVALUATION_SHADER based on specific use
		break;
	default:
		MD_UNTOUCHABLE();
	}

	GL_ASSERT(pOpenGLShader->shaderID = glCreateShader(shaderType));
	GL_ASSERT(glShaderSource(pOpenGLShader->shaderID, 1, (const char**)&pFile->content, (const GLint*)&pFile->size));
	GL_ASSERT(glCompileShader(pOpenGLShader->shaderID));

	glGetShaderiv(pOpenGLShader->shaderID, GL_COMPILE_STATUS, (GLint*)&shaderType);
	if (shaderType != GL_TRUE)
	{
		char infoLog[512];
		glGetShaderInfoLog(pOpenGLShader->shaderID, 512, NULL, infoLog);
		MD_ASSERT_MSG(MD_FALSE, "Shader \"%s\" compilation failed: %s", shaderSource, infoLog);
	}

	mdFileClose(pFile);

	return pShader;
}

void mdShaderDestroy(struct MdShader* pShader)
{
	MD_ASSERT(pShader != MD_NULL);

	struct OpenGLShader* pOpenGLShader = (struct OpenGLShader*)pShader->pInternal;
	GL_ASSERT(glDeleteShader(pOpenGLShader->shaderID));

	MD_FREE(pShader->pInternal, struct OpenGLShader);
	MD_FREE(pShader, struct MdShader);
	pShader = MD_NULL;
}

#endif // MD_USE_OPENGL