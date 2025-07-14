#!/usr/bin/env julia

using Statistics

const DEFAULT_ARRAY_SIZE = 100
const NUM_ITERATIONS = 100_000
const NUM_TESTS = 10

"""
    get_time_microseconds()

Get current time in microseconds using high precision timing.
"""
function get_time_microseconds()
    return round(Int, time() * 1_000_000)
end

"""
    benchmark_single_malloc(array_size)

Benchmark a single malloc operation for the given array size.
Returns the time taken in microseconds, or -1 if malloc failed.
"""
function benchmark_single_malloc(array_size)
    start_time = get_time_microseconds()

    # Allocate memory using Libc.malloc
    ptr = Libc.malloc(array_size * sizeof(Float64))

    end_time = get_time_microseconds()

    if ptr == C_NULL
        @error "malloc failed!"
        return -1
    end

    # Prevent compiler optimization
    Core.donotdelete(ptr)

    # Free the memory
    Libc.free(ptr)

    return end_time - start_time
end

"""
    run_multiple_tests(array_size)

Run multiple test runs and calculate comprehensive statistics.
"""
function run_multiple_tests(array_size)
    println("Running $NUM_TESTS test runs with $NUM_ITERATIONS iterations each...\n")

    avg_times = Vector{Float64}(undef, NUM_TESTS)

    for test in 1:NUM_TESTS
        print("Test run $test/$NUM_TESTS: ")

        total_time = 0
        successful_allocs = 0

        for i in 1:NUM_ITERATIONS
            time_taken = benchmark_single_malloc(array_size)

            if time_taken >= 0
                total_time += time_taken
                successful_allocs += 1
            end
        end

        if successful_allocs > 0
            avg_times[test] = total_time / successful_allocs
            println("Average: $(round(avg_times[test], digits=2)) microseconds")
        else
            avg_times[test] = -1
            println("Failed")
        end
    end

    # Calculate overall statistics
    valid_tests = count(x -> x >= 0, avg_times)

    if valid_tests > 0
        valid_avg_times = filter(x -> x >= 0, avg_times)
        overall_avg = mean(valid_avg_times)
        min_avg = minimum(valid_avg_times)
        max_avg = maximum(valid_avg_times)

        println("\n=== OVERALL STATISTICS ===")
        println("Valid test runs: $valid_tests/$NUM_TESTS")
        println("Overall average time: $(round(overall_avg, digits=2)) microseconds")
        println("Best test run average: $(round(min_avg, digits=2)) microseconds")
        println("Worst test run average: $(round(max_avg, digits=2)) microseconds")
        println("Memory size per allocation: $(array_size * sizeof(Float64)) bytes ($array_size doubles)")
    end
end

"""
    print_usage(program_name)

Print usage information for the program.
"""
function print_usage(program_name)
    println("Usage: $program_name [array_size]")
    println("  array_size: Number of doubles to allocate (default: $DEFAULT_ARRAY_SIZE)")
    println("  Example: $program_name 1000    # Benchmark 1000-element array")
    println("  Example: $program_name         # Benchmark $DEFAULT_ARRAY_SIZE-element array (default)")
end

"""
    main()

Main function that parses arguments and runs the benchmark.
"""
function main()
    # Parse command line arguments
    if length(ARGS) > 1
        @error "Too many arguments"
        print_usage(PROGRAM_FILE)
        exit(1)
    end

    array_size = DEFAULT_ARRAY_SIZE

    if length(ARGS) == 1
        if ARGS[1] == "-h" || ARGS[1] == "--help"
            print_usage(PROGRAM_FILE)
            exit(0)
        end

        try
            array_size = parse(Int, ARGS[1])
            if array_size <= 0
                throw(ArgumentError("Array size must be positive"))
            end
        catch e
            @error "Invalid array size '$(ARGS[1])'. Must be a positive integer."
            print_usage(PROGRAM_FILE)
            exit(1)
        end
    end

    println("=== Malloc Benchmark for $(array_size)-element Double Array ===")
    println("System: $(Sys.KERNEL)")
    println("Array size: $array_size doubles ($(array_size * sizeof(Float64)) bytes)")
    println("Iterations per test: $NUM_ITERATIONS")
    println("Number of test runs: $NUM_TESTS\n")

    # Warm up the system
    println("Warming up the system...")
    for i in 1:1000
        ptr = Libc.malloc(array_size * sizeof(Float64))
        if ptr != C_NULL
            Libc.free(ptr)
        end
    end
    println("Warm-up complete.\n")

    # Run the benchmark
    run_multiple_tests(array_size)

    println("\nBenchmark completed!")
end

# Run the main function if this script is executed directly
if abspath(PROGRAM_FILE) == @__FILE__
    main()
end
