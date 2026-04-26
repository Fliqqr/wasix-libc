#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *f(void *arg) {
    (void)arg;

    printf("called f()\n");
    fflush(stdout);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_nsec += 500 * 1000 * 1000; // 0.5 seconds
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec += 1;
        ts.tv_nsec -= 1000000000;
    }

    printf("Awaiting lock with timeout: 0.5s...\n");
    fflush(stdout);
    int result = pthread_mutex_timedlock(&lock, &ts);

    printf("%s\n", result == 0 ? "True" : "False");
    fflush(stdout);

    return NULL;
}

int main(void) {
    pthread_mutex_lock(&lock);

    printf("Lock acquired in main\n");
    fflush(stdout);

    pthread_t thread;
    pthread_create(&thread, NULL, f, NULL);

    pthread_join(thread, NULL);

    return 0;
}