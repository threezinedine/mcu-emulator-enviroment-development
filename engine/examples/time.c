#include "MEEDEngine/MEEDEngine.h"

int main(void)
{
	mdMemoryInitialize();

	struct MdTime current = mdGetTimeFromUNIXTimestamp(mdGetUNIXTimestamp());
	char		  timeString[20];
	mdGetTimeString(timeString, sizeof(timeString), current);

	mdFormatPrint("[TIME] Current time is: %s\n", timeString);

	mdMemoryShutdown();
	return 0;
}