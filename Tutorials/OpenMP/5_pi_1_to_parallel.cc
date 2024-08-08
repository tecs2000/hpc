#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

static long num_steps = 100000;
double step;

int main()
{
    int i;
    double pi, sum = 0.0;

    step = 1.0 / double(num_steps);

    auto start = chrono::high_resolution_clock::now();

    // computes the area under the f(x) = 4 / (1 + x²) curve, from x = 0 to x = 1
#pragma omp parallel
    {
        double x;
#pragma omp for schedule(static) reduction(+ : sum)
        for (i = 0; i < num_steps; i++)
        {
            x = (i + 0.5) * step; // gets the middle of the ith rectangle
            sum += 4.0 / (1.0 + (x * x));
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    pi = step * sum;

    printf("Area under the curve = %lf\n", pi);
    printf("Time = %.2lf ms\n", (float)duration.count());

    return 0;
}