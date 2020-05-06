#include <stdio.h>
#include <unistd.h>

#include <ztime.h>

int main()
{
    long long t_nano, t_sec, t_day;

    t_nano = ztm_get_time(ztmNanosec, ztmReal);
    t_sec = ztm_get_time(ztmSec, ztmReal);
    t_day = ztm_get_time(ztmDay, ztmReal);

    printf("time [%lld]nanosecs\n", t_nano);
    printf("time [%lld]secs\n", t_sec);
    printf("time [%lld]days\n", t_day);

    char buff[128];
    ztm_time_to_buff(t_nano, ztmNanosec, buff, 128, "%FT%H:%M:%SZ");
    printf("converted [%lld]secs to [%lld]days ISO_8601[%s] buff[%s]\n", t_sec, ztm_convert_time(t_sec, ztmSec, ztmDay), ztm_time_to_str(t_nano, ztmNanosec, "%FT%H:%M:%SZ"), buff);

    t_nano = ztm_str_to_time(buff, "%FT%H:%M:%SZ", ztmNanosec);

    printf("time [%lld]nanosecs\n", t_nano);

    ztm_time_to_buff(t_nano, ztmNanosec, buff, 128, "%FT%H:%M:%SZ");
    printf("buff[%s]\n", buff);

    return 0;
}
