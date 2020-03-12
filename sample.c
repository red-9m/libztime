#include <stdio.h>
#include <unistd.h>

#include <ztime.h>

int main()
{
    unsigned long long t_nano, t_sec, t_day;

    t_nano = ztm_get_time(ztmNanosec, ztmReal);
    t_sec = ztm_get_time(ztmSec, ztmReal);
    t_day = ztm_get_time(ztmDay, ztmReal);

    printf("time [%lld]nanosecs\n", t_nano);
    printf("time [%lld]secs\n", t_sec);
    printf("time [%lld]days\n", t_day);

    printf("converted [%lld]secs to [%lld]days\n", t_sec, ztm_convert(t_sec, ztmSec, ztmDay));

    return 0;
}
