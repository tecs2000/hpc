#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node
{
    int data;
    int fibdata;
    struct node *next;
};

int fib(int n)
{
    if (n <= 1)
        return n;

    int x, y;

#pragma omp task shared(x) if (n > 10)
    x = fib(n - 1);

#pragma omp task shared(y) if (n > 10)
    y = fib(n - 2);

#pragma omp taskwait
    return x + y;
}

void processwork(struct node *p)
{
    int n;
    n = p->data;
    p->fibdata = fib(n);
}

struct node *init_list(struct node *p)
{
    int i;
    struct node *head = NULL;
    struct node *temp = NULL;

    head = (struct node *)malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i = 0; i < N; i++)
    {
        temp = (struct node *)malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[])
{
    double start, end;
    struct node *p = NULL;
    struct node *temp = NULL;
    struct node *head = NULL;

    int i, count = 0;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

    start = omp_get_wtime();
    while (p != NULL)
    {
        ++count;
        p = p->next;
    }
    p = head;
    struct node *parr[count];

    for (i = 0; i < count; i++)
    {
        parr[i] = p;
        p = p->next;
    }

#pragma omp parallel
    {
#pragma omp master
        for (i = 0; i < count; i++)
            processwork(parr[i]);
    }

    end = omp_get_wtime();

    p = head;
    while (p != NULL)
    {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Compute Time: %f seconds\n", end - start);

    return 0;
}
