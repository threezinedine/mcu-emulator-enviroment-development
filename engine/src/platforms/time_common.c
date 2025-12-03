#include "MEEDEngine/platforms/console.h"
#include "MEEDEngine/platforms/time.h"

const char* mdGetMonthName(i32 month)
{
	switch (month)
	{
	case 1:
		return "January";
	case 2:
		return "February";
	case 3:
		return "March";
	case 4:
		return "April";
	case 5:
		return "May";
	case 6:
		return "June";
	case 7:
		return "July";
	case 8:
		return "August";
	case 9:
		return "September";
	case 10:
		return "October";
	case 11:
		return "November";
	case 12:
		return "December";
	default:
		MD_UNTOUCHABLE();
		return "Invalid Month";
	}
}

void mdGetTimeString(char* buffer, u32 bufferSize, struct MdTime time)
{
	MD_ASSERT(buffer != MD_NULL);
	MD_ASSERT(bufferSize >= 20); // "YYYY-MM-DD HH:MM:SS" is 19 characters + null terminator

	mdFormatString(buffer,
				   bufferSize,
				   "%04d-%02d-%02d %02d:%02d:%02d",
				   time.year,
				   time.month,
				   time.day,
				   time.hours,
				   time.minutes,
				   time.seconds);
}

f64 mdGetTimeDifferenceInMilliseconds(mdUNIXTime start, mdUNIXTime end)
{
	return mdGetTimeDifferenceInMicroseconds(start, end) * 1e-3;
}

f64 mdGetTimeDifferenceInSeconds(mdUNIXTime start, mdUNIXTime end)
{
	return mdGetTimeDifferenceInMicroseconds(start, end) * 1e-6;
}