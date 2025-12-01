#if PLATFORM_IS_WEB
#include "MEEDEngine/platforms/file.h"
#include <fcntl.h>
#include <unistd.h>

struct LinuxFileData
{
	i32 fd;
};

struct MdFileData* mdFileOpen(const char* filePath, enum MdFileMode mode)
{
	struct MdFileData* pFileData = MD_MALLOC(struct MdFileData);
	mdMemorySet(pFileData, 0, sizeof(struct MdFileData));

	pFileData->isOpen	= MD_FALSE;
	pFileData->filePath = filePath;
	pFileData->mode		= mode;

	pFileData->pInternal = MD_MALLOC(struct LinuxFileData);
	MD_ASSERT(pFileData->pInternal != MD_NULL);

	struct LinuxFileData* pLinuxData = (struct LinuxFileData*)pFileData->pInternal;

	switch (mode)
	{
	case MD_FILE_MODE_READ:
		pLinuxData->fd = open(filePath, O_RDONLY);
		break;
	case MD_FILE_MODE_WRITE:
		pLinuxData->fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		break;
	case MD_FILE_MODE_APPEND:
		pLinuxData->fd = open(filePath, O_WRONLY | O_CREAT | O_APPEND, 066);
		break;

	default:
		MD_UNTOUCHABLE();
	}

	if (pLinuxData->fd != -1)
	{
		pFileData->isOpen = MD_TRUE;

		if (mode == MD_FILE_MODE_READ)
		{
			// Get file size
			off_t currentPos = lseek(pLinuxData->fd, 0, SEEK_CUR);
			off_t fileSize	 = lseek(pLinuxData->fd, 0, SEEK_END);
			lseek(pLinuxData->fd, currentPos, SEEK_SET);

			pFileData->size = (u32)fileSize;

			// Read file content into memory
			pFileData->content = MD_MALLOC_ARRAY(char, pFileData->size + 1);
			MD_ASSERT(pFileData->content != MD_NULL);
			ssize_t bytesRead = read(pLinuxData->fd, pFileData->content, pFileData->size);
			MD_ASSERT_MSG(bytesRead == (ssize_t)pFileData->size,
						  "Failed to read entire file: Expected %u bytes, but read %zd bytes.",
						  pFileData->size,
						  bytesRead);
			pFileData->content[pFileData->size] = '\0'; // Null-terminate the content
		}
	}

	return pFileData;
}

b8 mdFileIsOpen(struct MdFileData* pFileData)
{
	MD_ASSERT(pFileData != MD_NULL);
	return pFileData->isOpen;
}

void mdFileWrite(struct MdFileData* pFileData, const char* data, mdSize size)
{
	MD_ASSERT(pFileData != MD_NULL);
	MD_ASSERT(pFileData->isOpen == MD_TRUE);

	struct LinuxFileData* pLinuxData = (struct LinuxFileData*)pFileData->pInternal;
	MD_ASSERT(pLinuxData != MD_NULL);

	ssize_t bytesWritten = write(pLinuxData->fd, data, size);
	MD_ASSERT_MSG(bytesWritten == (ssize_t)size,
				  "Failed to write all data to file: Expected to write %zu bytes, but wrote %zd bytes.",
				  size,
				  bytesWritten);
}

void mdFileClose(struct MdFileData* pFileData)
{
	MD_ASSERT(pFileData != MD_NULL);

	struct LinuxFileData* pLinuxData = (struct LinuxFileData*)pFileData->pInternal;
	MD_ASSERT(pLinuxData != MD_NULL);

	if (pFileData->isOpen)
	{
		close(pLinuxData->fd);

		if (pFileData->mode == MD_FILE_MODE_READ && pFileData->content != MD_NULL)
		{
			MD_FREE_ARRAY(pFileData->content, char, pFileData->size + 1);
		}
	}

	MD_FREE(pLinuxData, struct LinuxFileData);
	MD_FREE(pFileData, struct MdFileData);
}

#endif // PLATFORM_IS_WEB
