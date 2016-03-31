/*
 * How to use #pragma omp atomic to avoid racing condition.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "omp.h"

int race()
{
    int x = 0;
    omp_set_num_threads(4);
    /*
    Given four threads running at the same time,
    we expect x is increased by 4 after the parallel section.
    However, this is not the whole story because x+=1 is not an
    atomic operation.
    */
#pragma omp parallel
{
//    #pragma omp atomic
    x += 1;
}
    return x;
}

int main()
{
    int num = 1000;
    int stat[5] = {0};

    for(int i=0; i<num; i++)
    {
        stat[race()]++;
    }
    for(int i=0; i<5; i++)
        printf("sum = %d for %d times\n", i, stat[i]);

    return 0;
}

