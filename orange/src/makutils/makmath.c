#include <makmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double mu_random_double(double min_val, double max_val, unsigned int seed)
{
    return 0;
}

int32_t mu_random_int(int32_t min_val, int32_t max_val, unsigned int seed)
{
    int i, random_number;
    srand(seed);
    i = rand();
    random_number = (i % (min_val - max_val + 1)) + min_val;
    printf("%d  ", random_number);
    return 0;
}

int main(int count, char **args)
{
    time_t t;
    time(&t);
    for(int i = 0;i<5000;++i)
    mu_random_int(atoi(args[1]), atoi(args[2]), (unsigned int) t);
    return 0;
}