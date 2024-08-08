#include <bits/stdc++.h>
#include <omp.h>

using namespace  std;
    
#define NUM_THREADS 2

static long num_steps = 100000;
double step;

int main(){
    int i, nthreads;
    double pi = 0;

    step = 1.0 / (double) num_steps;
    omp_set_num_threads(NUM_THREADS);

    auto start = chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        int i, id, nthrds;
        double x, sum = 0;

        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if(id == 0) nthreads = nthrds; // sometimes the system gives the program less threads than expected
        
        // implements a round-robin distribution of work among the threads
        for(i = id; i < num_steps; i = i + nthrds){
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
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