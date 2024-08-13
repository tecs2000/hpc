# HPC

Implementing some well-known algorithms using high-performance computing (HPC) techniques.

## Tips

For tips on how to benchmark a parallel program, please refer to the [benchmarking tutorial](https://github.com/tecs2000/hpc/blob/main/bench_tutorial.md). To apply all the suggestions mentioned in the tutorial in an automated way, run the `env_prep.sh` script.

**Note:** The `env_prep.sh` script will terminate all non-essential programs running on your system and log your user out. When logged in again, run the `run_bench.sh` script to perform the benchmarking.

## Automated Benchmark Preparation Script

The `env_prep.sh` script is designed to set up your environment for accurate benchmarking by applying several system optimizations. Here’s what the script does:

1. **Disable Turbo Boost:** Disables CPU turbo boost to ensure consistent performance.
2. **Disable Internet Connection:** Disables network interfaces to minimize external interference.
3. **Set CPU Governor to Performance:** Forces the CPU to run at maximum frequency.
4. **Disable Intel SpeedShift:** Disables Intel's SpeedShift to prevent dynamic frequency changes.
5. **Terminate Unnecessary Tasks:** Kills non-essential processes to free up system resources.

After running the benchmark_prep.sh script and logging back in, execute the run_bench.sh script to start the benchmarking process.

## Benchmarking Process

### Impartial Benchmarking
Execute the run_bench.sh script to start the benchmarking process:

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
./run_bench.sh 50 10 10 my_program arg1 arg2
```

This approach ensures your system is optimally configured for HPC benchmarking, leading to more accurate and reliable results.

### CPU Benchmarking

There is also the possibility to perform benchmarking based on the number of cores that can be used by the program. The `cpu_bench.sh` script allows you to do that. 

1. It binds the processes in cores close to one another, exporting the `OMP_PROC_BIND="close"` and `OMP_PLACES="cores"` variables.
2. Gives you the option to show the affinity verbosely for each iteration of the benchmarking. Uncomment the `export_debug_variables` function call. And,
3. Varies the number of CPUs from 1 to 4. This range number can be modified internally.

Similarly to the `run_bench.sh` script, it receives as input: 

**nruns:** Number of times to repeat the benchmarking.\
**nwarmup-runs:** Number of times to run the program before benchmarking.\
**niceness:** Process priority (-20 for highest priority).\
**program:** The program to benchmark.\
**program-parameters:** Parameters for the program.

And can be run as:

```bash
./cpu_bench.sh <nruns> <nwarmup-runs> <niceness> <program> <program-parameters>
```

In this case, it will perform only one warmup, and nruns iterations for each CPU configuration, storing the result of the benchmarking process in the `bench_results` folder. The filenames inside the `bench_results` folder respect the following pattern: `bench_{i}_cpus.out`, where `i` is the number of CPUs used in that experiment.

## Plotting Data

The `plot_bench_results.py` program collects the results stored in the `bench_results` folder and plots 2 bar charts: one comparing the average time of execution of each experiment, and another comparing the median time. And a box plot showing the degree of data dispersion.

This program doesn't require any parameters to run, only that the folder `bench_results` exists and contains the `bench_{i}_cpus.out` files.

PS: If you modify the number of CPUs being benchmarked, change the `num_cpus` variable, in the `plot_bench_results.py` script, accordingly.