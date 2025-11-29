#pragma once
#include "MEEDEngine/platforms/common.h"

/**
 * @file linked_list.h
 *
 * Self implemented general linked list container.
 */

/**
 * @struct MEEDLinkedListNode
 * @brief A node in a linked list.
 *
 * This structure represents a single node in a linked list, containing
 * a pointer to the data and a pointer to the next node in the list.
 */
struct MEEDLinkedListNode
{
	void* pData; ///< Pointer to the data stored in the node.

	struct MEEDLinkedListNode* pNext; ///< Pointer to the next node in the linked list.
};

struct MEEDLinkedList
{
	meedSize				   size;  ///< The number of nodes in the linked list.
	struct MEEDLinkedListNode* pHead; ///< Pointer to the head node of the linked list.
	struct MEEDLinkedListNode* pTail; ///< Pointer to the tail node of the linked list.
};

/**
 * @brief Creates and initializes a new linked list.
 *
 * This function allocates memory for a new linked list structure,
 * initializes its members, and returns a pointer to the newly created
 * linked list.
 *
 * @return Pointer to the newly created MEEDLinkedList.
 */
struct MEEDLinkedList* meedLinkedListCreate();

/**
 * Append the data to the end of the linked list.
 *
 * @param pList Pointer to the MEEDLinkedList. If NULL, raises an assertion.
 * @param pData Pointer to the data to be added to the list.
 */
void meedLinkedListPush(struct MEEDLinkedList* pList, void* pData);

/**
 * @brief Destroys a linked list and frees its memory.
 *
 * This function deallocates the memory used by the linked list and
 * all its nodes. It does not free the memory of the data stored in
 * the nodes; that is the responsibility of the caller.
 *
 * @param pList Pointer to the MEEDLinkedList to be destroyed. If NULL, raises an assertion.
 */
void meedLinkedListDestroy(struct MEEDLinkedList* pList);