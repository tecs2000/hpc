#include <bits/stdc++.h>
#include <omp.h>

using namespace  std;
    
#define PAD 8 // for a 64 byte L1 cache line size
#define NUM_THREADS 2

static long num_steps = 100000;
double step;

int main(){
    int i, nthreads;
    double pi;

    /**
     * A double takes 8B in mem., so 8x8 = 64B --> a whole cache line,
     * thus avoiding the false sharing performance problem 
     */
    double sum[NUM_THREADS][PAD];
    
    step = 1.0 / (double) num_steps;
    omp_set_num_threads(NUM_THREADS);

    auto start = chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        int i, id, nthrds;
        double x;

        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if(id == 0) nthreads = nthrds; // sometimes the system gives the program less threads than expected
        
        // implements a round-robin distribution of work among the threads
        for(i = id, sum[id][0] = 0.0; i < num_steps; i = i + nthrds){
            x = (i + 0.5) * step;
            sum[id][0] += 4.0 / (1.0 + x * x);
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    for(i = 0, pi = 0.0; i < nthreads; i++)
        pi += sum[i][0] * step;
    
    printf("Area under the curve = %lf\n", pi);
    printf("Time = %.2lf ms\n", (float)duration.count());

    return 0;
}