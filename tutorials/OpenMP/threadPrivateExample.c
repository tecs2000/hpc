#include <omp.h>
#include <stdio.h>

int counter;  // Global variable

#pragma omp threadprivate(counter)

int main()
{
    counter = 0;  // Initialize the global variable

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        // Each thread will have its own copy of counter
        counter = thread_id;  // Set the counter to the thread ID

        printf("Thread %d: counter = %d\n", thread_id, counter);
    }

    // Another parallel region where threadprivate retains its value
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        // The counter should retain its value from the previous parallel region
        printf("Thread %d (after): counter = %d\n", thread_id, counter);
    }

    return 0;
}
