# HPC

Implementing some well-known algorithms using high-performance computing (HPC) techniques.

## Tips

For tips on how to benchmark a parallel program, please refer to the [benchmarking tutorial](https://github.com/tecs2000/hpc/blob/main/benchmarking.md). To apply all the suggestions mentioned in the tutorial in an automated way, run the `env_prep.sh` script.

**Note:** The `env_prep.sh` script will terminate all non-essential programs running on your system and log your user out. When logged in again, run the `run_bench.sh` script to perform the benchmarking.

## Automated Benchmark Preparation Script

The `env_prep.sh` script is designed to set up your environment for accurate benchmarking by applying several system optimizations. Here’s what the script does:

1. **Disable Turbo Boost:** Disables CPU turbo boost to ensure consistent performance.
2. **Disable Internet Connection:** Disables network interfaces to minimize external interference.
3. **Set CPU Governor to Performance:** Forces the CPU to run at maximum frequency.
4. **Disable Intel SpeedShift:** Disables Intel's SpeedShift to prevent dynamic frequency changes.
5. **Terminate Unnecessary Tasks:** Kills non-essential processes to free up system resources.

After running the benchmark_prep.sh script and logging back in, execute the run_benchmark.sh script to start the benchmarking process.

## Benchmarking Process

Execute the run_benchmark.sh script to start the benchmarking process:

```bash
./run_bench.sh <nruns> <nwarmup-runs> <niceness> <program> <program-parameters>
```

Where,

**nruns:** Number of times to repeat the benchmarking.\
**nwarmup-runs:** Number of times to run the program before benchmarking.\
**niceness:** Process priority (-20 for highest priority).\
**program:** The program to benchmark.\
**program-parameters:** Parameters for the program.

Example:

```bash
./run_bench.sh 10 -10 my_program arg1 arg2
```

This approach ensures your system is optimally configured for HPC benchmarking, leading to more accurate and reliable results.
