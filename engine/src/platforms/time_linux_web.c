#if PLATFORM_IS_LINUX || PLATFORM_IS_WEB

#include "MEEDEngine/platforms/time.h"
#include <time.h>

mdUNIXTime mdGetUNIXTimestamp()
{
	return (mdUNIXTime)time(NULL);
}

struct MdTime mdGetTimeFromUNIXTimestamp(mdUNIXTime timestamp)
{
	struct MdTime result;
	struct tm	  timeInfo;

	gmtime_r((const time_t*)&timestamp, &timeInfo);

	result.year	 = timeInfo.tm_year + 1900;
	result.month = timeInfo.tm_mon + 1;
	result.day	 = timeInfo.tm_mday;

	result.hours   = timeInfo.tm_hour;
	result.minutes = timeInfo.tm_min;
	result.seconds = timeInfo.tm_sec;

	return result;
}

f64 mdGetTimeDifferenceInMicroseconds(mdUNIXTime start, mdUNIXTime end)
{
	return (f64)(end - start) * 1e6;
}

#endif // PLATFORM_IS_LINUX