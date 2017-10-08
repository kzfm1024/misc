#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

#define TIMEOUT 5 /* seconds */

static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t s_cond_monotonic = PTHREAD_COND_INITIALIZER;

void timespec_diff(struct timespec *start,
                   struct timespec *end,
                   struct timespec *result)
{
    if ((end->tv_nsec - start->tv_nsec) < 0)
    {
        result->tv_sec = end->tv_sec - start->tv_sec - 1;
        result->tv_nsec = end->tv_nsec - start->tv_nsec + 1000000000;
    }
    else
    {
        result->tv_sec = end->tv_sec - start->tv_sec;
        result->tv_nsec = end->tv_nsec - start->tv_nsec;
    }
}

void print_timedwait_result(int id, int result,
                            struct timespec* start,
                            struct timespec* end)
{
    struct timespec diff;
    timespec_diff(start, end, &diff);

    if (result == ETIMEDOUT)
    {
        printf("t%d timeout: %ld.%06ld (%ld.%06ld => %ld.%06ld)\n",
               id, diff.tv_sec, diff.tv_nsec,
               start->tv_sec, start->tv_nsec, end->tv_sec, end->tv_nsec);
               
    }
    else
    {
        printf("t%d success: %ld.%06ld (%ld.%06ld => %ld.%06ld)\n",
               id, diff.tv_sec, diff.tv_nsec,
               start->tv_sec, start->tv_nsec, end->tv_sec, end->tv_nsec);
    }
}

void* timedwait(void* arg)
{
    int id = *((int*)arg);
    
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
        print_timedwait_result(id, ret, &start, &now);
    }
    pthread_mutex_unlock(&s_mutex);
}

void* timedwait_monotonic(void* arg)
{
    int id = *((int*)arg);

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
        print_timedwait_result(id, ret, &start, &now);
    }
    pthread_mutex_unlock(&s_mutex);
}

int main()
{
    /* pthread_t t1, t2, t3, t4; */
    /* int id1 = 1, id2 = 2, id3 = 3, id4 = 4; */

    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    pthread_cond_init(&s_cond_monotonic, &attr);

    /*
     * do pthread_cond_signal() after 1 seconds
     */
    /* pthread_create(&t1, NULL, timedwait, (void*)&id1); */
    /* sleep(1); */
    /* { */
    /*     pthread_mutex_lock(&s_mutex);     */
    /*     pthread_cond_signal(&s_cond); */
    /*     pthread_mutex_unlock(&s_mutex); */
    /* } */
    /* pthread_join(t1, NULL); */

    /*
     * do nothing - timedwait thread will be timed out
     */
    /* pthread_create(&t2, NULL, timedwait, (void*)&id2); */
    /* { */
    /*     ; */
    /* } */
    /* pthread_join(t2, NULL); */

    pthread_create(&t1, NULL, timedwait, (void*)&id1);
    pthread_create(&t2, NULL, timedwait_monotonic, (void*)&id2);
    sleep(1);
    {
        /*
         *  set the clock forward 1 minute  - timedwait thread will be timed out
         */
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        now.tv_sec += 60;
        int ret = clock_settime(CLOCK_REALTIME, &now);
        assert(ret == 0); // root privilege is necessary for clock_settime()
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
