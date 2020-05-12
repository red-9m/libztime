#include <stdio.h>
#include <unistd.h>

#include <ztime.h>

int main()
{
    long long t_nano, t_sec, t_day;
    struct tm *bd_time, *bd_time_conv;
    char buff[128];

    ztm_calibrate();
    printf("[%s][%s][%ld] daylight[%d]\n", tzname[0], tzname[1], timezone, daylight);
    t_nano = ztm_get_time(ztmNanosec, ztmReal);
    t_sec = ztm_get_time(ztmSec, ztmReal);
    t_day = ztm_get_time(ztmDay, ztmReal);

    printf("ztm_get_time [%lld]nanosecs\n", t_nano);
    printf("ztm_get_time [%lld]secs\n", t_sec);
    printf("ztm_get_time [%lld]days\n", t_day);

    printf("converted secs to [%lld]days and nano to str[%s] buff[%s]\n",
        ztm_convert_time(t_sec, ztmSec, ztmDay),
        ztm_time_to_str(t_nano, ztmNanosec, "%FT%H:%M:%SZ"),
        ztm_time_to_buff(t_nano, ztmNanosec, buff, 128, "%FT%H:%M:%SZ")
    );

    t_nano = ztm_str_to_time(buff, "%FT%H:%M:%SZ", ztmNanosec);
    printf("Buff converted back to [%lld]nanosecs\n", t_nano);


    bd_time = ztm_get_time_brokendown(ztmReal);
    printf("ztm_get_bd_time [%02d:%02d:%02d]\n",
        bd_time->tm_hour, bd_time->tm_min, bd_time->tm_sec
    );
    bd_time_conv = ztm_make_time_brokendown(t_nano, ztmNanosec);

    printf("ztm_make_bd_time[%02d:%02d:%02d]\n",
        bd_time_conv->tm_hour, bd_time_conv->tm_min, bd_time_conv->tm_sec
    );
    printf("\n");

    return 0;
}
