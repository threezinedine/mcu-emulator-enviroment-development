#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdMemoryInitialize();

	mdPrint("Hello, MCU Emulator Environment Development!\n");

	mdMemoryShutdown();
	return 0;
}