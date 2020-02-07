#ifndef _LIB_ZTIME_H_
#define _LIB_ZTIME_H_

/** @file ztime.h
 *
 *  @brief libZTime public API
 */


// === Public types ============================================================

/**
 *  Time unit
 */
enum timez_unit
{
    timezSec = 0,        /**< Time unit - seconds */
    timezMicrosec = 1,   /**< Time unit - microseconds */
    timezNanosec = 2,    /**< Time unit - nanoseconds */

    TIMEZ_UNIT_LAST = 3  /**< Last item */
};

/**
 *  Clock type
 */
enum timez_clock
{
    timezReal = 0,  /**< Real non-monotonic clock (could be affected by NTP and time set) */
    timezMono = 1   /**< Monotonic clock (require calibration) */
};

// === Public API ==============================================================

/** @brief  Re-calibrate monotonic clock
 */
void timez_calibration(void);

/** @brief  Get current time
 *
 *  @param  unit           Time unit (seconds, microseconds, nanoseconds)
 *  @param  clock          Clock type (Realtime, monotonic)
 *
 *  @return                zero - incorrect time unit; positive - time in `unit` time units
 */
unsigned long long timez_gettime(enum timez_unit unit, enum timez_clock clock);


#endif
