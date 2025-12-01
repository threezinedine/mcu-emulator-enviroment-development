#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdPlatformMemoryInitialize();

	struct MEEDFileData* pFile = mdPlatformOpenFile("./example.txt", MEED_FILE_MODE_WRITE);

	const char* content = "Hello, MEED Engine File!\n";
	mdPlatformWrite(pFile, content, mdPlatformGetStringLength(content));
	mdPlatformCloseFile(pFile);

	struct MEEDFileData* pReadFile = mdPlatformOpenFile("./example.txt", MEED_FILE_MODE_READ);
	mdPlatformPrint("File Content:\n");
	mdPlatformPrint(pReadFile->content);
	mdPlatformCloseFile(pReadFile);

	mdPlatformMemoryShutdown();
	return 0;
}