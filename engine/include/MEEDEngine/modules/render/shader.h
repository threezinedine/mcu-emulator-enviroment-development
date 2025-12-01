#pragma once

#if __cplusplus
extern "C" {
#endif

#include "MEEDEngine/platforms/platforms.h"

/**
 * @file shader.h
 * The utilities for managing shaders inside the `MEEDEngine` rendering module.
 */

enum MdShaderType
{
	MD_SHADER_TYPE_VERTEX = 0,	 ///< Vertex shader type.
	MD_SHADER_TYPE_FRAGMENT,	 ///< Fragment shader type.
	MD_SHADER_TYPE_COMPUTE,		 ///< Compute shader type.
	MD_SHADER_TYPE_GEOMETRY,	 ///< Geometry shader type.
	MD_SHADER_TYPE_TESSELLATION, ///< Tessellation shader type.
	MD_SHADER_TYPE_COUNT
};

/**
 * Represents a shader used in the rendering pipeline.
 */
struct MdShader
{
	void*			  pInternal; ///< Pointer to internal shader data (implementation-specific).
	enum MdShaderType type;		 ///< The type of the shader (e.g., vertex, fragment).
};

/**
 * Creates a shader from the specified file.
 * @param type The type of the shader to create.
 * @param filePath The file path to the shader source code.
 * @return A pointer to the created `MdShader` structure.
 */
struct MdShader* mdShaderCreate(enum MdShaderType type, const char* filePath);

/**
 * Destroys a previously created shader.
 * @param pShader A pointer to the `MdShader` structure to destroy.
 */
void mdShaderDestroy(struct MdShader* pShader);

#if __cplusplus
}
#endif