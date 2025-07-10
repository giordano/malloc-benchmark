#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>

#define ARRAY_SIZE 100
#define NUM_ITERATIONS 100000
#define NUM_TESTS 10

// Function to get current time in microseconds
long long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

// Function to benchmark single malloc operation
long long benchmark_single_malloc() {
    long long start_time = get_time_microseconds();

    double *array = (double *)malloc(ARRAY_SIZE * sizeof(double));

    long long end_time = get_time_microseconds();

    if (array == NULL) {
        fprintf(stderr, "malloc failed!\n");
        return -1;
    }

    // Initialize array to avoid compiler optimization
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = (double)i;
    }

    free(array);

    return end_time - start_time;
}

// Function to benchmark multiple malloc operations
void benchmark_multiple_mallocs() {
    printf("Benchmarking %d malloc operations for %d-element double array...\n",
           NUM_ITERATIONS, ARRAY_SIZE);

    long long total_time = 0;
    long long min_time = LLONG_MAX;
    long long max_time = 0;
    int failed_allocs = 0;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        long long time_taken = benchmark_single_malloc();

        if (time_taken < 0) {
            failed_allocs++;
            continue;
        }

        total_time += time_taken;

        if (time_taken < min_time) {
            min_time = time_taken;
        }

        if (time_taken > max_time) {
            max_time = time_taken;
        }
    }

    int successful_allocs = NUM_ITERATIONS - failed_allocs;

    if (successful_allocs > 0) {
        double avg_time = (double)total_time / successful_allocs;

        printf("\nResults:\n");
        printf("  Successful allocations: %d\n", successful_allocs);
        printf("  Failed allocations: %d\n", failed_allocs);
        printf("  Total time: %lld microseconds\n", total_time);
        printf("  Average time per malloc: %.2f microseconds\n", avg_time);
        printf("  Minimum time: %lld microseconds\n", min_time);
        printf("  Maximum time: %lld microseconds\n", max_time);
        printf("  Memory allocated per malloc: %zu bytes\n", ARRAY_SIZE * sizeof(double));
    } else {
        printf("All malloc operations failed!\n");
    }
}

// Function to run multiple test runs and calculate statistics
void run_multiple_tests() {
    printf("Running %d test runs with %d iterations each...\n\n", NUM_TESTS, NUM_ITERATIONS);

    double avg_times[NUM_TESTS];

    for (int test = 0; test < NUM_TESTS; test++) {
        printf("Test run %d/%d: ", test + 1, NUM_TESTS);

        long long total_time = 0;
        int successful_allocs = 0;

        for (int i = 0; i < NUM_ITERATIONS; i++) {
            long long time_taken = benchmark_single_malloc();

            if (time_taken >= 0) {
                total_time += time_taken;
                successful_allocs++;
            }
        }

        if (successful_allocs > 0) {
            avg_times[test] = (double)total_time / successful_allocs;
            printf("Average: %.2f microseconds\n", avg_times[test]);
        } else {
            avg_times[test] = -1;
            printf("Failed\n");
        }
    }

    // Calculate overall statistics
    double overall_avg = 0;
    double min_avg = 1e9;
    double max_avg = 0;
    int valid_tests = 0;

    for (int i = 0; i < NUM_TESTS; i++) {
        if (avg_times[i] >= 0) {
            overall_avg += avg_times[i];
            if (avg_times[i] < min_avg) min_avg = avg_times[i];
            if (avg_times[i] > max_avg) max_avg = avg_times[i];
            valid_tests++;
        }
    }

    if (valid_tests > 0) {
        overall_avg /= valid_tests;

        printf("\n=== OVERALL STATISTICS ===\n");
        printf("Valid test runs: %d/%d\n", valid_tests, NUM_TESTS);
        printf("Overall average time: %.2f microseconds\n", overall_avg);
        printf("Best test run average: %.2f microseconds\n", min_avg);
        printf("Worst test run average: %.2f microseconds\n", max_avg);
        printf("Memory size per allocation: %zu bytes (%d doubles)\n",
               ARRAY_SIZE * sizeof(double), ARRAY_SIZE);
    }
}

int main() {
    printf("=== Malloc Benchmark for 100-element Double Array ===\n");
    printf("System: Linux\n");
    printf("Array size: %d doubles (%zu bytes)\n", ARRAY_SIZE, ARRAY_SIZE * sizeof(double));
    printf("Iterations per test: %d\n", NUM_ITERATIONS);
    printf("Number of test runs: %d\n\n", NUM_TESTS);

    // Warm up the system
    printf("Warming up the system...\n");
    for (int i = 0; i < 1000; i++) {
        double *temp = malloc(ARRAY_SIZE * sizeof(double));
        if (temp) {
            free(temp);
        }
    }
    printf("Warm-up complete.\n\n");

    // Run the benchmark
    run_multiple_tests();

    printf("\nBenchmark completed!\n");
    return 0;
}
