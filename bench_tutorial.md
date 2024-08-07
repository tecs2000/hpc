# Guidelines

A guide on how to prepare the Linux environment for benchmarking a program.

## Disable Turboboost

To disable Turbo Boost on Intel and AMD processors:

```
# Intel
echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo

# AMD
echo 0 > /sys/devices/system/cpu/cpufreq/boost
```

## Disable Internet Connection

Disable the internet connection for each interface:

```
sudo ip link set <if-name> down
```

## Set scaling_governor to 'performance'

If we don’t set the scaling governor policy to be performance, kernel can decide that it’s better to save power and throttle. Setting scaling_governor to ‘performance’ helps to avoid sub-nominal clocking.

Performance mode implies running the CPU at the maximum frequency.

```
for i in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
do
  echo performance > $i
done
```

## Set process priority

Increase process priority to ensure it gets more CPU time. Niceness values range from -20 (highest priority) to 19 (lowest priority).
By increasing priority, process gets more CPU time and Linux scheduler favors it more in comparison with processes with normal priority.

```
perf stat -r <nruns> -- sudo nice -n -<niceness> <program> <program-parameters>
```

# Disable Intel SpeedShift

Intel® Speed Shift Technology allows the processor to quickly reach the optimal frequency and voltage for performance and power efficiency. To disable it:

```
sudo apt-get install msr-tools
sudo modprobe msr
sudo wrmsr -a 0x770 0
```

`msr` stands for Model-Specific Register.

# Terminate Unnecessary Tasks

Finally, it is crucial to terminate all non-essential programs running on your computer, both in the foreground and background. These programs can compete with your application for limited system resources, such as I/O and CPU, potentially affecting its performance.


## References

[1] https://easyperf.net/blog/2019/08/02/Perf-measurement-environment-on-Linux

[2] https://medium.com/@The_Anshuman/how-to-change-linux-process-priority-af55d074bf3d

[3] nice man page

[4] https://www.anandtech.com/show/9751/examining-intel-skylake-speed-shift-more-responsive-processors