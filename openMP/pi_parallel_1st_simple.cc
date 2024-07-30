#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

static long num_steps = 100000;
double step;

int main()
{
    double pi = 0.0;
    step = 1.0 / double(num_steps); // rectangle base

    int num_t = omp_get_max_threads();
    int steps_per_t = num_steps / num_t;

    auto start = chrono::high_resolution_clock::now();

    // computes the area under the f(x) = 4 / (1 + x²) curve, from x = 0 to x = 1
    // using openmp pragma
    #pragma omp parallel 
    {
        double sum = 0.0;
        int id = omp_get_thread_num();
        // printf("Executing thread (%d) computations...\n", id);

        for (int i = id * steps_per_t; i < ((id + 1) * steps_per_t) && i < num_steps; i++)
        {
            double x = (i + 0.5) * step; // gets the middle of the ith rectangle
            sum += 4.0 / (1.0 + (x * x));
        }

        #pragma omp atomic
        pi += sum * step;
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    printf("Area under the curve = %lf\n", pi);
    printf("Time = %.2lf ms\n", (float)duration.count());

    return 0;
}
