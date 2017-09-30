#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

#define TIMEOUT 5 /* seconds */

static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t s_cond_monotonic = PTHREAD_COND_INITIALIZER;

void timespec_diff(struct timespec *start,
                   struct timespec *stop,
                   struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0)
    {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    }
    else
    {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
}

void print_timedwait_result(int retcode,
                            struct timespec* start,
                            struct timespec* stop)
{
    struct timespec diff;
    timespec_diff(start, stop, &diff);

    if (retcode == ETIMEDOUT)
    {
        printf("ETIMEDOUT : %ld.%06ld (%ld.%06ld => %ld.%06ld)\n",
               diff.tv_sec, diff.tv_nsec,
               start->tv_sec, start->tv_nsec, stop->tv_sec, stop->tv_nsec);
               
    }
    else if (retcode == EINTR)
    {
        printf("EINTR     : %ld.%06ld (%ld.%06ld => %ld.%06ld)\n",
               diff.tv_sec, diff.tv_nsec,
               start->tv_sec, start->tv_nsec, stop->tv_sec, stop->tv_nsec);
    }
    else
    {
        printf("SUCCESS   : %ld.%06ld (%ld.%06ld => %ld.%06ld)\n",
               diff.tv_sec, diff.tv_nsec,
               start->tv_sec, start->tv_nsec, stop->tv_sec, stop->tv_nsec);
    }
}

void* timedwait(void* arg)
{
    pthread_mutex_lock(&s_mutex);
    {
        struct timespec start;
        struct timespec timeout;
        
        clock_gettime(CLOCK_REALTIME, &start);
        timeout.tv_sec = start.tv_sec + TIMEOUT;
        timeout.tv_nsec = start.tv_nsec;
        int ret = pthread_cond_timedwait(&s_cond, &s_mutex, &timeout);

        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        print_timedwait_result(ret, &start, &now);
    }
    pthread_mutex_unlock(&s_mutex);
}

void* timedwait_monotonic(void* arg)
{
    pthread_mutex_lock(&s_mutex);
    {
        struct timespec start;
        struct timespec timeout;
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        timeout.tv_sec = start.tv_sec + TIMEOUT;
        timeout.tv_nsec = start.tv_nsec;
        int ret = pthread_cond_timedwait(&s_cond_monotonic, &s_mutex, &timeout);

        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        print_timedwait_result(ret, &start, &now);
    }
    pthread_mutex_unlock(&s_mutex);
}

int main()
{
    pthread_t th;
    pthread_t th_monotonic;

    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    pthread_cond_init(&s_cond_monotonic, &attr);

    /*
     * do pthread_cond_signal() after 1 seconds
     */
    pthread_create(&th, NULL, timedwait, NULL);
    {
        sleep(1);
        pthread_mutex_lock(&s_mutex);    
        pthread_cond_signal(&s_cond);
        pthread_mutex_unlock(&s_mutex);
    }
    pthread_join(th, NULL);

    /*
     * do nothing - timedwait thread will be timed out
     */
    pthread_create(&th, NULL, timedwait, NULL);
    {
        ;
    }
    pthread_join(th, NULL);

    /*
     *  set the clock 1 minute  - timedwait thread will be timed out
     */
    pthread_create(&th, NULL, timedwait, NULL);
    pthread_create(&th_monotonic, NULL, timedwait_monotonic, NULL);
    {
        sleep(1);

        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        now.tv_sec += 60;
        int ret = clock_settime(CLOCK_REALTIME, &now);
        assert(ret == 0); // root privilege is necessary for clock_settime()
    }
    pthread_join(th, NULL);
    pthread_join(th_monotonic, NULL);
}
