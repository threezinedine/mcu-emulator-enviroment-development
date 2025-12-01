#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdMemoryInitialize();

	struct MdFileData* pFile = mdFileOpen("./example.txt", MD_FILE_MODE_WRITE);

	const char* content = "Hello, MEED Engine File!\n";
	mdFileWrite(pFile, content, mdGetStringLength(content));
	mdFileClose(pFile);

	struct MdFileData* pReadFile = mdFileOpen("./example.txt", MD_FILE_MODE_READ);
	mdPrint("File Content:\n");
	mdPrint(pReadFile->content);
	mdFileClose(pReadFile);

	mdMemoryShutdown();
	return 0;
}