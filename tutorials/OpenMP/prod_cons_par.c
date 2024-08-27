/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include <omp.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>

#define N 10000

#define Nthreads 2

/* Some random number constants from numerical recipies */
#define SEED 2531
#define RAND_MULT 1366
#define RAND_ADD 150889
#define RAND_MOD 714025
int randy = SEED;

/* function to fill an array with random numbers */
void fill_rand(int length, double *a)
{
    int i;
    
#pragma omp parallel for
    for (i = 0; i < length; i++)
    {
        randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;

        *(a + i) = ((double)randy) / ((double)RAND_MOD);
    }
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a)
{
    int i;
    double sum = 0.0;
#pragma omp parallel for reduction(+ : sum)
    for (i = 0; i < length; i++)
        sum += *(a + i);
    return sum;
}

int main()
{
    double *A, sum, runtime;
    int flag = 0;

    omp_set_num_threads(Nthreads);

    A = (double *)malloc(N * sizeof(double));

    runtime = omp_get_wtime();

#pragma omp parallel shared(randy)
    {
#pragma omp single
        {
            fill_rand(N, A); // Producer: fill an array of data

            sum = Sum_array(N, A); // Consumer: sum the array}
        }
    }
    runtime = omp_get_wtime() - runtime;
    printf(" In %f seconds, The sum is %f \n", runtime, sum);
}
