#!/bin/bash

create_dir() {
  local outdir=$1

  if [[ ! -d $outdir ]]; then
    mkdir $outdir
  else
    rm -r $outdir/*
  fi
}

export_debug_variables() {
  export OMP_DISPLAY_AFFINITY="TRUE"
  export OMP_DISPLAY_ENV="VERBOSE"
  export OMP_AFFINITY_FORMAT="Nesting_Lvl %0.1L, Thread %.4n: Cores %.16{thread_affinity}"
}

export_variables() {
  export OMP_PROC_BIND="close"
  export OMP_PLACES="cores"
}

start_bench() {
  local nruns=$1
  local nwarmup=$2
  local niceness=$3
  local program=$4
  shift 4
  local program_parameters=("$@")

  export_variables
#  export_debug_variables

  echo "Starting benching process..."
  for i in 1 2 4; do
    # Warm the system up
    echo "-- Warming $i core(s) up..."
    export OMP_NUM_THREADS=$i
    for j in $(seq 1 $nwarmup); do
      nice -n -$niceness $program "${program_parameters[@]}" >>$outdir/warmup_data.out
    done

    echo "-- Benching with $i CPU(s)..."
    export OMP_NUM_THREADS=$i
    outfile=bench_${i}_cpus.out
    for j in $(seq 1 $nruns); do
      nice -n -$niceness $program "${program_parameters[@]}" >>$outdir/$outfile
    done
  done
}

# Usage: ./cpu_bench.sh <nruns> <nwarmup-runs> <niceness> <program> <program-parameters>
if [ $# -lt 4 ]; then
  echo "Usage: $0 <nruns> <nwarmup-runs> <niceness> <program> <program-parameters>"
  exit 1
fi

outdir="bench_results"

nruns=$1
nwarmup_runs=$2
niceness=$3
program=$4
shift 4
program_parameters=("$@")

echo "Parameters = $nruns $nwarmup_runs $niceness $program '${program_parameters[@]}'"

create_dir $outdir
start_bench $nruns $nwarmup_runs $niceness $program "${program_parameters[@]}"
