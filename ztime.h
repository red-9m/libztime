#ifndef _LIB_ZTIME_H_
#define _LIB_ZTIME_H_

/** @file ztime.h
 *
 *  @brief libZTime public API
 */

#include <stddef.h>
#include <time.h>

// === Public types ============================================================

/**
 *  Time unit
 */
enum ztm_unit
{
    ztmDay = 0,    /**< Days */
    ztmHour,       /**< Hours */
    ztmMin,        /**< Minutes */
    ztmSec,        /**< Seconds */
    ztmMicrosec,   /**< Microseconds */
    ztmNanosec,    /**< Nanoseconds */

    ZTM_UNIT_LAST  /**< Last item */
};

/**
 *  Clock type
 */
enum ztm_clock
{
    ztmReal,  /**< Real non-monotonic clock (could be affected by NTP and time set) */
    ztmMono,  /**< Monotonic clock (require calibration) */
    ztmCpu    /**< Process CPU time */
};


// === Public API ==============================================================

/** @brief  Set time zone and re-calibrate monotonic clock
 *
 *  @return                '0' - success, '-1' - fail and errno is set
 */
int ztm_calibrate(void);


/** @brief  Get current time
 *
 *  @param  unit           Time units for returned time
 *  @param  clock          Clock type to get
 *
 *  @return                Non-negative - time in 'unit'; '-1' - fail and errno is set
 */
long long ztm_get_time(enum ztm_unit unit, enum ztm_clock clock);


/** @brief  Get current broken-down time
 *
 *  @param  clock          Clock type to get
 *
 *  @return                Broken-down time; NULL - fail and errno is set
 */
struct tm* ztm_get_time_brokendown(enum ztm_clock clock);


/** @brief  Make broken-down time
 *
 *  @param  time           Time to convert
 *  @param  unit           Unit 'time' passed in
 *
 *  @return                Broken-down time; NULL - fail and errno is set
 */
struct tm* ztm_make_time_brokendown(long long time, enum ztm_unit unit);


/** @brief  Convert between time units
 *
 *  @param  time           Time to convert
 *  @param  fromUnit       Unit 'time' passed in
 *  @param  toUnit         Unit to return time in
 *
 *  @return                Converted time in 'toUnit'; EINVAL in errno - incorrect time unit
 */
long long ztm_convert_time(long long time, enum ztm_unit fromUnit, enum ztm_unit toUnit);


/** @brief  Format time to static string
 *
 *  @param  time           Time
 *  @param  unit           Unit 'time' passed in
 *  @param  format         Output format template (see strftime())
 *
 *  @return                Formatted time string
 */
const char* ztm_time_to_str(long long time, enum ztm_unit unit, const char *format);


/** @brief  Format time to string buffer
 *
 *  @param  time           Time
 *  @param  unit           Unit 'time' passed in
 *  @param  buff           Buffer to write to
 *  @param  buffSize       Size of buffer 'buff'
 *  @param  format         Output format template (see strftime())
 *
 *  @return                Returned 'buff'
 */
char* ztm_time_to_buff(long long time, enum ztm_unit unit, char* buff, size_t buffSize, const char *format);


/** @brief  Convert time string to time unit
 *
 *  @param  timeStr        Time string
 *  @param  format         Input format template (see strftime())
 *  @param  toUnit         Unit to return time in
 *
 *  @return                Time in 'unit'; EINVAL in errno - incorrect time string or format
 */
long long ztm_str_to_time(const char *timeStr, const char *format, enum ztm_unit toUnit);


#endif
