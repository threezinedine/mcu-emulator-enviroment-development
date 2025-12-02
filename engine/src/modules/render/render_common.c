#include "render_common.h"

u32 mdGetVertexAttributeTypeSize(enum MdVertexBufferAttributeType type)
{
	switch (type)
	{
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_FLOAT:
		return sizeof(float);
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_UNSIGNED_INT:
		return sizeof(u32);
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_FLOAT2:
		return sizeof(float) * 2;
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_UNSIGNED_INT2:
		return sizeof(u32) * 2;
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_FLOAT3:
		return sizeof(float) * 3;
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_UNSIGNED_INT3:
		return sizeof(u32) * 3;
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_FLOAT4:
		return sizeof(float) * 4;
	case MD_VERTEX_BUFFER_ATTRIBUTE_TYPE_UNSIGNED_INT4:
		return sizeof(u32) * 4;
	default:
		MD_UNTOUCHABLE();
		return 0;
	}
}