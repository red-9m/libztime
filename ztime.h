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

/** @brief  Re-calibrate monotonic clock
 */
void ztm_adjust_mono(void);

/** @brief  Get current time
 *
 *  @param  unit           Time units
 *  @param  clock          Clock type
 *
 *  @return                zero - incorrect time unit; positive - time in 'unit' time units
 */
unsigned long long ztm_get_time(enum ztm_unit unit, enum ztm_clock clock);

/** @brief  Convert time units
 *
 *  @param  time           Time to convert
 *  @param  fromUnit       Units 'time' passed in
 *  @param  toUnit         Units to return
 *
 *  @return                zero - incorrect time unit; positive - converted time in 'toUnit' time units
 */
unsigned long long ztm_convert(unsigned long long time, enum ztm_unit fromUnit, enum ztm_unit toUnit);


#endif
