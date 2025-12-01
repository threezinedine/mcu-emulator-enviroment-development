#pragma once

#if __cplusplus
extern "C" {
#endif

/**
 * @file release_stack.h
 * Used for working with some resources which is need to be released in a certain order.
 */

#include "MEEDEngine/core/containers/stack.h"
#include "MEEDEngine/platforms/common.h"

/**
 * Structure representing a release stack.
 * Used to manage resources that need to be released in a specific order.
 */
struct MdReleaseStack
{
	struct MdStack* pStack;
};

/**
 * User must pass this function pointer to free the attached instance.
 * @param pData Pointer to the data to be released.
 */
typedef void (*MdReleaseFunc)(void*);

/**
 * Structure representing an item in the release stack.
 * Contains the data pointer and the associated release function.
 */
struct MdReleaseStackItem
{
	void*		  pData;		///< Pointer to the data associated with the resource.
	MdReleaseFunc pReleaseFunc; ///< Function pointer to release the resource.
};

/**
 * Initialize an instance of MdReleaseStack.
 * @return Pointer to the created MdReleaseStack instance.
 */
struct MdReleaseStack* mdReleaseStackCreate();

/**
 * Push a release function and its associated data onto the release stack.
 * @param pReleaseStack Pointer to the MdReleaseStack instance.
 * @param pData Pointer to the data to be passed to the release function.
 * @param pReleaseFunc Pointer to the function to be called for releasing the resource. CANNOT be NULL.
 */
void mdReleaseStackPush(struct MdReleaseStack* pReleaseStack, void* pData, MdReleaseFunc pReleaseFunc);

/**
 * Free the MdReleaseStack instance and release all resources in the stack.
 * @param pReleaseStack Pointer to the MdReleaseStack instance to be destroyed. if NULL, raise an exception.
 *
 * @note All resources and the callbacks will be released in the reverse order they were added (LIFO order).
 */
void mdReleaseStackDestroy(struct MdReleaseStack* pReleaseStack);

#if __cplusplus
}
#endif