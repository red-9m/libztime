#include <stdio.h>
#include <unistd.h>

#include <ztime.h>

int main()
{
    unsigned long long t, t_mono;

    timez_calibration();

    while (1)
    {
        t = timez_gettime(timezMicrosec, timezReal);
        t_mono = timez_gettime(timezMicrosec, timezMono);

        printf("time [%lld] mono [%lld] diff[%lld]\n", t, t_mono, t - t_mono);
        sleep(3);
    }

    return 0;
}
