#include <time.h>
#include <sys/time.h>

#include "ztime.h"


// === Private definitions =====================================================

static long long g_time_calibration[TIMEZ_UNIT_LAST];

static const unsigned int NANOSEC_IN_MICROSEC = 1000;
static const unsigned int NANOSEC_IN_SEC = 1000000000;
static const unsigned int MICROSEC_IN_SEC = 1000000;


// === Private functions =======================================================

static unsigned long long _gettime(enum timez_unit unit, enum timez_clock clock)
{
    unsigned long long result = 0;
    struct timespec now;
    clockid_t clock_type = CLOCK_REALTIME;
    if (clock == timezMono)
        clock_type = CLOCK_MONOTONIC;
    clock_gettime(clock_type, &now);

    switch (unit)
    {
        case timezSec: result = (unsigned long long)now.tv_sec; break;
        case timezMicrosec: result = (unsigned long long)now.tv_nsec / NANOSEC_IN_MICROSEC + (unsigned long long)now.tv_sec * MICROSEC_IN_SEC; break;
        case timezNanosec: result = (unsigned long long)now.tv_nsec + (unsigned long long)now.tv_sec * NANOSEC_IN_SEC; break;
        default: break;
    }

    return result;
}


// === Public functions ========================================================

void timez_calibration()
{
    g_time_calibration[timezNanosec] = _gettime(timezNanosec, timezReal) - _gettime(timezNanosec, timezMono);
    g_time_calibration[timezMicrosec] = g_time_calibration[timezNanosec] / NANOSEC_IN_MICROSEC;
    g_time_calibration[timezSec] = g_time_calibration[timezNanosec] / NANOSEC_IN_SEC;
}

unsigned long long timez_gettime(enum timez_unit unit, enum timez_clock clock)
{
    if (clock == timezMono)
        return _gettime(unit, timezMono) + g_time_calibration[unit];
    else
        return _gettime(unit, timezReal);
}
