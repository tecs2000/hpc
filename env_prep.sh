#!/bin/bash

set -x

# Function to check if a command was successful and exit if not
check_command() {
  if [ $? -ne 0 ]; then
    echo "Error: $1 failed."
    exit 1
  fi
}

# Disable Turbo Boost
disable_turbo_boost() {
  echo "Disabling Turbo Boost..."

  if [ -f /sys/devices/system/cpu/intel_pstate/no_turbo ]; then
    echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo
    check_command "Disabling Intel Turbo Boost"
  elif [ -f /sys/devices/system/cpu/cpufreq/boost ]; then
    echo 0 > /sys/devices/system/cpu/cpufreq/boost
    check_command "Disabling AMD Turbo Boost"
  else
    echo "Turbo Boost control not available."
  fi
}

# Disable Internet connection
disable_internet() {
  echo "Disabling Internet connection..."

  interfaces=$(ip link show | grep -E '^[0-9]+: ' | cut -d: -f2 | awk '{print $1}')
  for interface in $interfaces; do
    sudo ip link set $interface down
    check_command "Disabling Internet on $interface"
  done
}

# Set scaling_governor to 'performance'
set_scaling_governor() {
  echo "Setting CPU scaling governor to 'performance'..."

  for governor in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor; do
    echo performance | sudo tee $governor > /dev/null
    check_command "Setting scaling governor to performance"
  done
}

# Disable Intel SpeedShift
disable_speedshift() {
  echo "Disabling Intel SpeedShift..."

  if ! command -v wrmsr &> /dev/null; then
    sudo apt-get install -y msr-tools
    check_command "Installing msr-tools"
  fi

  if ! lsmod | grep msr &> /dev/null; then
    sudo modprobe msr
    check_command "Loading msr module"
  fi

  sudo wrmsr -a 0x770 0
  check_command "Disabling Intel SpeedShift"
}

terminate_unnecessary_tasks() {
  echo "Terminating unnecessary tasks..."

  for task in $(ps -eo pid,cmd --sort=-%mem | grep -v -e '^\s*PID' | awk '{print $1}'); do
    sudo kill -9 $task
    check_command "Terminating task $task"
  done
}

# Main function
main() {
  disable_turbo_boost
  disable_internet
  set_scaling_governor
  disable_speedshift
  terminate_unnecessary_tasks

  echo "Environment preparation complete."
}

main

