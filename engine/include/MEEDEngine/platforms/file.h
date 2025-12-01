#pragma once

#if __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * @file file.h
 * The utilities for managing file operations inside the `MEEDEngine`.
 */

enum MEED_BINDING MdFileMode
{
	MD_FILE_MODE_READ,	///< Open the file in read mode.
	MD_FILE_MODE_WRITE, ///< Open the file in write mode.
	MD_FILE_MODE_APPEND ///< Open the file in append mode.
};

/**
 * Needed information for working with files.
 */
struct MEED_BINDING MdFileData
{
	void* pInternal MEED_HIDDEN; ///< Used for storing custom file system data (e.g., file handles).

	b8				isOpen;	  ///< Flag indicating whether the file is currently open.
	const char*		filePath; ///< The path of the file.
	u32				size;	  ///< The size of the file in bytes.
	char*			content;  ///< Pointer to the file content in memory.
	enum MdFileMode mode;	  ///< The mode in which the file was opened.
};

/**
 * Opens a file at the specified path with the given mode.
 * @param filePath The path of the file to open.
 * @param mode The mode in which to open the file.
 * @return Pointer to the MdFileData representing the opened file.
 */
struct MdFileData* mdFileOpen(const char* filePath, enum MdFileMode mode);

/**
 * Checks if the specified file is open.
 * @param pFileData Pointer to the MdFileData representing the file.
 * @return MEED_TRUE if the file is open, MEED_FALSE otherwise.
 */
b8 mdFileIsOpen(struct MdFileData* pFileData);

/**
 * Writes data to the specified file.
 * @param pFileData Pointer to the MdFileData representing the file.
 * @param data Pointer to the data to write.
 * @param size The size of the data to write in bytes.
 */
void mdFileWrite(struct MdFileData* pFileData, const char* data, mdSize size);

/**
 * Closes the specified file.
 * @param pFileData Pointer to the MdFileData representing the file to close.
 */
void mdFileClose(struct MdFileData* pFileData);

#if __cplusplus
}
#endif