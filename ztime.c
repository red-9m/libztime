#include <sys/time.h>
#include <errno.h>

#include "ztime.h"


// === Private definitions =====================================================

#define STR_TIME_BUFF_SIZE 512

static char g_str_time[STR_TIME_BUFF_SIZE];
static long long g_time_calibration[(int)ZTM_UNIT_LAST];

static const long long NANOSEC_IN_MICROSEC = 1000;
static const long long NANOSEC_IN_SEC      = NANOSEC_IN_MICROSEC * 1000000;
static const long long NANOSEC_IN_MIN      = NANOSEC_IN_SEC      * 60;
static const long long NANOSEC_IN_HOUR     = NANOSEC_IN_MIN      * 60;
static const long long NANOSEC_IN_DAY      = NANOSEC_IN_HOUR     * 24;

long long unit_to_nano[(int)ZTM_UNIT_LAST] =
{
    NANOSEC_IN_DAY, NANOSEC_IN_HOUR, NANOSEC_IN_MIN, NANOSEC_IN_SEC, NANOSEC_IN_MICROSEC, 1
};


// === Private functions =======================================================

static struct timespec _get_time(enum ztm_clock clock)
{
    struct timespec now;
    clockid_t clock_type = CLOCK_REALTIME;

    if (clock == ztmMono)
        clock_type = CLOCK_MONOTONIC;
    else if (clock == ztmCpu)
        clock_type = CLOCK_PROCESS_CPUTIME_ID;

    // If clock_gettime() returned an error then -1 returned and errno is set
    if (clock_gettime(clock_type, &now) == -1)
        now.tv_sec  = -1;

    return now;
}

static long long _get_time_nano(struct timespec time)
{
    return (long long)time.tv_nsec + (long long)time.tv_sec * NANOSEC_IN_SEC;
}

static long long _convert_to_from(long long time, int unit, int direction)
{
    long long result = 0;

    if ((unit >= 0) && (unit < (int)ZTM_UNIT_LAST))
    {
        if (direction)
            result = time * unit_to_nano[unit];
        else
            result = time / unit_to_nano[unit];
    } else
        errno = EINVAL;

    return result;
}

static long long _convert_to_nanosec(long long time, enum ztm_unit fromUnit)
{
    const int direction_to = 1;
    return _convert_to_from(time, (int)fromUnit, direction_to);
}

static long long _convert_from_nanosec(long long time, enum ztm_unit toUnit)
{
    const int direction_from = 0;
    return _convert_to_from(time, (int)toUnit, direction_from);
}


// === Public functions ========================================================

int ztm_calibrate()
{
    int i, result;
    long long nanosec_calibration;
    struct timespec time_real, time_mono;

    result = -1;
    tzset();
    time_real = _get_time(ztmReal);
    if (time_real.tv_sec != -1)
    {
        time_mono = _get_time(ztmMono);

        if (time_mono.tv_sec != -1)
        {
            nanosec_calibration = _get_time_nano(time_real) - _get_time_nano(time_mono);
            for (i = 0; i < (int)ZTM_UNIT_LAST; i++)
                g_time_calibration[i] = _convert_from_nanosec(nanosec_calibration, (enum ztm_unit)i);
        }
    }

    return result;
}

long long ztm_get_time(enum ztm_unit unit, enum ztm_clock clock)
{
    long long result = -1;
    struct timespec now = _get_time(clock);

    if (now.tv_sec != -1)
    {
        result = _convert_from_nanosec(_get_time_nano(now), unit);

        if (clock == ztmMono)
            result += g_time_calibration[unit];
    }

    return result;
}

struct tm* ztm_get_time_brokendown(enum ztm_clock clock)
{
    struct tm* result = NULL;
    struct timespec now = _get_time(clock);

    if (now.tv_sec != -1)
        result = localtime(&now.tv_sec);

    return result;
}

struct tm* ztm_make_time_brokendown(long long time, enum ztm_unit unit)
{
    struct tm* result;
    long long secs = _convert_from_nanosec(_convert_to_nanosec(time, unit), ztmSec);
    time_t now = (time_t)secs;
    result = localtime(&now);

    return result;
}

long long ztm_convert_time(long long time, enum ztm_unit fromUnit, enum ztm_unit toUnit)
{
    return _convert_from_nanosec(_convert_to_nanosec(time, fromUnit), toUnit);
}

const char* ztm_time_to_str(long long time, enum ztm_unit unit, const char *format)
{
    ztm_time_to_buff(time, unit, g_str_time, STR_TIME_BUFF_SIZE, format);

    return g_str_time;
}

char* ztm_time_to_buff(long long time, enum ztm_unit unit, char* buff, size_t buffSize, const char *format)
{
    time_t rawtime = (time_t)_convert_from_nanosec(_convert_to_nanosec(time, unit), ztmSec);
    struct tm *brokendown_time = localtime(&rawtime);

    strftime(buff, buffSize, format, brokendown_time);

    return buff;
}

long long ztm_str_to_time(const char *timeStr, const char *format, enum ztm_unit toUnit)
{
    struct tm bd_time;
    time_t time_sec = 0;
    bd_time.tm_isdst = daylight;

    if (strptime(timeStr, format, &bd_time) != NULL)
        time_sec = mktime(&bd_time);
    else
        errno = EINVAL;

    return ztm_convert_time(time_sec, ztmSec, toUnit);
}
