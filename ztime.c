#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include "ztime.h"


// === Private definitions =====================================================

static long long g_time_calibration[(int)ZTM_UNIT_LAST];

static const unsigned long long NANOSEC_IN_MICROSEC = 1000;
static const unsigned long long MICROSEC_IN_SEC     = 1000000;
static const unsigned long long NANOSEC_IN_SEC      = MICROSEC_IN_SEC * NANOSEC_IN_MICROSEC;
static const unsigned long long NANOSEC_IN_MIN      = NANOSEC_IN_SEC * 60;
static const unsigned long long NANOSEC_IN_HOUR     = NANOSEC_IN_MIN * 60;
static const unsigned long long NANOSEC_IN_DAY      = NANOSEC_IN_HOUR * 24;

unsigned long long unit_to_nano[(int)ZTM_UNIT_LAST] =
{
    NANOSEC_IN_DAY,
    NANOSEC_IN_HOUR,
    NANOSEC_IN_MIN,
    NANOSEC_IN_SEC,
    NANOSEC_IN_MICROSEC,
    1
};


// === Private functions =======================================================

static unsigned long long _get_time(enum ztm_clock clock)
{
    struct timespec now;
    unsigned long long result = 0;
    clockid_t clock_type = CLOCK_REALTIME;

    if (clock == ztmMono)
        clock_type = CLOCK_MONOTONIC;
    else if (clock == ztmCpu)
        clock_type = CLOCK_PROCESS_CPUTIME_ID;

    if (clock_gettime(clock_type, &now) == 0)
        result = (unsigned long long)now.tv_nsec + (unsigned long long)now.tv_sec * NANOSEC_IN_SEC;

    // If clock_gettime() returned an error then 0 returned and errno set to EINVAL
    return result;
}

static unsigned long long _convert_to_from(unsigned long long time, int unit, int direction)
{
    unsigned long long result = 0;

    if ((unit >= 0) && (unit < ZTM_UNIT_LAST))
    {
        if (direction)
            result = time * unit_to_nano[unit];
        else
            result = time / unit_to_nano[unit];
    }

    return result;    
}

static unsigned long long _convert_to_nanosec(unsigned long long time, enum ztm_unit fromUnit)
{
    return _convert_to_from(time, (int)fromUnit, 1);
}

static unsigned long long _convert_from_nanosec(unsigned long long time, enum ztm_unit toUnit)
{
    return _convert_to_from(time, (int)toUnit, 0);
}


// === Public functions ========================================================

void ztm_adjust_mono()
{
    int i;
    unsigned long long nanosec_calibration = _get_time(ztmReal) - _get_time(ztmMono);

    for (i = 0; i < (int)ZTM_UNIT_LAST; i++)
        g_time_calibration[i] = _convert_from_nanosec(nanosec_calibration, (enum ztm_unit)i);
}

unsigned long long ztm_get_time(enum ztm_unit unit, enum ztm_clock clock)
{
    unsigned long long result = _convert_from_nanosec(_get_time(clock), unit);

    if (clock == ztmMono)
        result += g_time_calibration[unit];

    return result;
}

unsigned long long ztm_convert(unsigned long long time, enum ztm_unit fromUnit, enum ztm_unit toUnit)
{
    return _convert_from_nanosec(_convert_to_nanosec(time, fromUnit), toUnit);
}
