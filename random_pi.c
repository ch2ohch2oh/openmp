/*
 * I write this little program to verify that parallel is faster than serial.
 * The CPU of my computer is dual-core so the parallel version is two times faster.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "omp.h"

double random_pi_parallel(int total_trials)
{
    int total_hits = 0;
#pragma omp parallel
{
    int workers = omp_get_num_threads();
    int id = omp_get_thread_num();
    int start = total_trials/workers*id;
    int end;
    if(id == workers-1)
        end = total_trials;
    else
        end = start + total_trials/workers;
    int hits = 0;
    for(int i=start; i<end; i++)
    {
        double x = (double)rand()/RAND_MAX;
        double y = (double)rand()/RAND_MAX;
        if(x*x + y*y <= 1.0)
            hits++;
    }
#pragma omp atomic
    total_hits += hits;
} // end of parallel section

    return (double)total_hits / total_trials * 4;
}

double random_pi_serial(int total_trials)
{
    int total_hits = 0;
    for(int i=0; i<total_trials; i++)
    {
        double x = (double)rand()/RAND_MAX;
        double y = (double)rand()/RAND_MAX;
        if(x*x + y*y <= 1.0)
            total_hits ++;
    }
    return (double)total_hits / total_trials * 4;
}

int main()
{
    double pi;

    printf("serial:\n");
    clock_t start = clock();
    pi = random_pi_serial(1000000000);
    clock_t end = clock();
    printf("pi = %f\n", pi);
    printf("time = %f\n", (double)(end-start)/CLOCKS_PER_SEC);

    printf("parallel:\n");
    start = clock();
    pi = random_pi_parallel(1000000000);
    end = clock();
    printf("pi = %f\n", pi);
    printf("time = %f\n", (double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}
