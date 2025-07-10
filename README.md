# Malloc Benchmark for 100-element Double Array

This C program benchmarks the performance of `malloc()` for allocating a 100-element array of `double` values.

## Features

- **Precise timing**: Uses `gettimeofday()` for microsecond precision
- **Multiple test runs**: Runs 10 separate test runs for statistical reliability
- **Comprehensive statistics**: Reports min, max, and average times
- **Memory size**: Allocates exactly 800 bytes (100 doubles × 8 bytes each)
- **System warm-up**: Performs initial allocations to stabilize performance
- **Error handling**: Tracks failed allocations

## Compilation

```bash
make
```

Or manually:
```bash
gcc -Wall -Wextra -O2 -std=c99 -o malloc_benchmark malloc_benchmark.c
```

## Usage

Run the benchmark:
```bash
make run
```

Or directly:
```bash
./malloc_benchmark
```

## Output

The program will output:
- System information and test parameters
- Progress for each test run
- Overall statistics including:
  - Average time per malloc operation
  - Best and worst test run averages
  - Memory size allocated per operation

## Configuration

You can modify the following constants in `malloc_benchmark.c`:

- `ARRAY_SIZE`: Number of doubles to allocate (default: 100)
- `NUM_ITERATIONS`: Number of malloc operations per test run (default: 10,000)
- `NUM_TESTS`: Number of separate test runs (default: 10)

## Example Output

```
=== Malloc Benchmark for 100-element Double Array ===
System: Linux
Array size: 100 doubles (800 bytes)
Iterations per test: 10000
Number of test runs: 10

Warming up the system...
Warm-up complete.

Running 10 test runs with 10000 iterations each...

Test run 1/10: Average: 0.15 microseconds
Test run 2/10: Average: 0.14 microseconds
...
Test run 10/10: Average: 0.16 microseconds

=== OVERALL STATISTICS ===
Valid test runs: 10/10
Overall average time: 0.15 microseconds
Best test run average: 0.14 microseconds
Worst test run average: 0.17 microseconds
Memory size per allocation: 800 bytes (100 doubles)

Benchmark completed!
```

## Cleanup

Remove compiled files:
```bash
make clean
``` 