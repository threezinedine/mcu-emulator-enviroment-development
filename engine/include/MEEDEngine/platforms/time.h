#pragma once

#if __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * @file time.h
 * @brief Platform-independent time utilities.
 */
struct MdTime
{
	i32 year;  ///< Full year, e.g., 2024
	i32 month; ///< Month of the year [1-12]
	i32 day;   ///< Day of the month [1-31]

	i32 hours;	 ///< Hours [0-23]
	i32 minutes; ///< Minutes [0-59]
	i32 seconds; ///< Seconds [0-59]
};

typedef i64 mdUNIXTime; ///< Type representing a UNIX timestamp in seconds since the epoch (January 1, 1970).

/**
 * @brief Get the current UNIX timestamp in seconds (GMT +0).
 * @return The current UNIX timestamp from the epoch in seconds.
 * */
mdUNIXTime mdGetUNIXTimestamp();

/**
 * @brief Convert a UNIX timestamp to a MdTime structure.
 * @param timestamp The UNIX timestamp to convert.
 * @return The corresponding MdTime structure.
 */
struct MdTime mdGetTimeFromUNIXTimestamp(mdUNIXTime timestamp);

/**
 * @brief Get the name of the month corresponding to the given month number.
 * @param month The month number [1-12].
 * @return The name of the month as a string.
 */
const char* mdGetMonthName(i32 month);

/**
 * Format a MdTime structure into a human-readable string. With
 *      format "YYYY-MM-DD HH:MM:SS".
 * @param buffer The buffer to store the formatted string.
 * @param bufferSize The size of the buffer, must be at least 20 bytes or raise an exception.
 * @param time The MdTime structure to format.
 *
 * @example
 * ```cpp
 * struct MdTime time = {2024, 6, 15, 14, 30, 0};
 * char timeString[20];
 * mdGetTimeString(timeString, sizeof(timeString), time);
 * // timeString now contains "2024-06-15 14:30:00"
 * ```
 */
void mdGetTimeString(char* buffer, u32 bufferSize, struct MdTime time);

/**
 * @brief Calculate the difference between two UNIX timestamps in seconds.
 * @param start The starting UNIX timestamp.
 * @param end The ending UNIX timestamp.
 * @return The difference in seconds as a floating-point value.
 */
f64 mdGetTimeDifferenceInSeconds(mdUNIXTime start, mdUNIXTime end);

/**
 * @brief Calculate the difference between two UNIX timestamps in milliseconds.
 * @param start The starting UNIX timestamp.
 * @param end The ending UNIX timestamp.
 * @return The difference in milliseconds as a floating-point value.
 */
f64 mdGetTimeDifferenceInMilliseconds(mdUNIXTime start, mdUNIXTime end);

/**
 * @brief Calculate the difference between two UNIX timestamps in microseconds.
 * @param start The starting UNIX timestamp.
 * @param end The ending UNIX timestamp.
 * @return The difference in microseconds as a floating-point value.
 */
f64 mdGetTimeDifferenceInMicroseconds(mdUNIXTime start, mdUNIXTime end);

#if __cplusplus
}
#endif