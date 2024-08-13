import re
import os
import numpy as np
import matplotlib.pyplot as plt

def get_times(filename):
    """Extracts the times from the given file and returns them as a list of floats."""
    times = []
    with open(filename, 'r') as file:
        for line in file:
            match = re.search(r'Time taken:\s+([\d.]+)\s+seconds', line)
            if match:
                times.append(float(match.group(1)))
    return times

def main():
    num_cpus = [1, 2, 4]
    all_times = []
    averages = []
    medians = []
    dir = 'bench_results/'

    for i in num_cpus:
        filename = f'{dir}bench_{i}_cpus.out'
        if os.path.exists(filename):
            times = get_times(filename)
            all_times.append(times)
            avg_time = np.mean(times) if times else 0
            median_time = np.median(times) if times else 0
            averages.append(avg_time)
            medians.append(median_time)
        else:
            print(f"File {filename} does not exist.")
            all_times.append([])
            averages.append(0)
            medians.append(0)

    print(f"Median Time w/ 4 CPU(s) / Median Time w/ 1 CPU(s) = {medians[2] / medians[0]}" )
    # Plotting Average Times Bar Chart
    plt.figure(figsize=(10, 5))
    plt.bar(range(len(num_cpus)), averages, tick_label=[f'{i} CPU(s)' for i in num_cpus])
    plt.xlabel('Number of CPUs')
    plt.ylabel('Average Time Taken (seconds)')
    plt.title('Average Time Taken for Different CPU Configurations')
    plt.show()

    # Plotting Box Plot
    plt.figure(figsize=(10, 5))
    plt.boxplot(all_times, labels=[f'{i} CPU(s)' for i in num_cpus], showmeans=True)
    plt.xlabel('Number of CPUs')
    plt.ylabel('Time Taken (seconds)')
    plt.title('Distribution of Time Taken for Different CPU Configurations')
    plt.show()

    # Plotting Median Times Bar Chart
    plt.figure(figsize=(10, 5))
    plt.bar(range(len(num_cpus)), medians, tick_label=[f'{i} CPU(s)' for i in num_cpus], color='orange')
    plt.xlabel('Number of CPUs')
    plt.ylabel('Median Time Taken (seconds)')
    plt.title('Median Time Taken for Different CPU Configurations')
    plt.show()

if __name__ == "__main__":
    main()
