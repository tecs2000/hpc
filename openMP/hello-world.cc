#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int main()
{

#pragma omp parallel
    {
        int id = 0;
        printf("Hello(%d)", id);
        printf(" world(%d) \n", id);
    }

    return 0;
}