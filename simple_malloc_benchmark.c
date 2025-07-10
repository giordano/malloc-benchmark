#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>

#define ARRAY_SIZE 100
#define NUM_ITERATIONS 10000

long long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

int main() {
    printf("Simple Malloc Benchmark for %d-element double array\n", ARRAY_SIZE);
    printf("Running %d iterations...\n\n", NUM_ITERATIONS);

    long long total_time = 0;
    long long min_time = LLONG_MAX;
    long long max_time = 0;
    int failed_allocs = 0;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        long long start_time = get_time_microseconds();

        double *array = malloc(ARRAY_SIZE * sizeof(double));

        long long end_time = get_time_microseconds();

        if (array == NULL) {
            failed_allocs++;
            continue;
        }

        // Initialize to prevent optimization
        for (int j = 0; j < ARRAY_SIZE; j++) {
            array[j] = (double)j;
        }

        free(array);

        long long time_taken = end_time - start_time;
        total_time += time_taken;

        if (time_taken < min_time) min_time = time_taken;
        if (time_taken > max_time) max_time = time_taken;
    }

    int successful_allocs = NUM_ITERATIONS - failed_allocs;

    if (successful_allocs > 0) {
        double avg_time = (double)total_time / successful_allocs;

        printf("Results:\n");
        printf("  Successful allocations: %d\n", successful_allocs);
        printf("  Failed allocations: %d\n", failed_allocs);
        printf("  Average time: %.2f microseconds\n", avg_time);
        printf("  Minimum time: %lld microseconds\n", min_time);
        printf("  Maximum time: %lld microseconds\n", max_time);
        printf("  Memory per allocation: %zu bytes\n", ARRAY_SIZE * sizeof(double));
    } else {
        printf("All allocations failed!\n");
    }

    return 0;
}