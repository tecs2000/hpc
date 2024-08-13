#!/bin/bash

# Function to check if a command was successful and exit if not
check_command() {
  if [ $? -ne 0 ]; then
    echo "Error: $1 failed."
    exit 1
  fi
}

# Set process priority and perform benchmark
run_benchmark() {
  local nruns=$1
  local niceness=$2
  local program=$3
  shift 3
  local program_parameters=("$@")

  echo "Running $program for $nruns times"

  sudo perf stat -r $nruns -- sudo nice -n -$niceness $program "${program_parameters[@]}"
  check_command "Running benchmark"
}

# Usage: ./benchmark_prep.sh <nruns> <nwarmup-runs> <niceness> <program> <program-parameters>
if [ $# -lt 4 ]; then
  echo "Usage: $0 <nruns> <nwarmup-runs> <niceness> <program> <program-parameters>"
  exit 1
fi

nruns=$1
nwarmup_runs=$2
niceness=$3
program=$4
shift 4
program_parameters=("$@")

# Warm-up phase
if [ $nwarmup_runs -gt 0 ]; then
  echo "Warming the system up..."
  run_benchmark $nwarmup_runs $niceness $program $outdir "${program_parameters[@]}"
fi

# Benchmark phase
if [ $nruns -gt 0 ]; then
  echo "Started benchmarking process"
  run_benchmark $nruns $niceness $program $outdir "${program_parameters[@]}"
fi
