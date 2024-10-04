#include <makutils.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int32_t mu_random_int(int32_t min_val, int32_t max_val)
{
    int random_number;
    random_number = rand() % (max_val - min_val + 1) + min_val;
    return random_number;
}

double mu_random_double(double min_val, double max_val)
{
    // Scale the output of rand() to a range between 0 and 1, then map it to the desired range
    double scale = rand() / (double) RAND_MAX; // rand() / RAND_MAX gives a float between 0 and 1
    double random_double = min_val + scale * (max_val - min_val); // Scale it to the desired range
    return random_double;
}

void mu_random_set_seed(void)
{
    time_t t;
    time(&t); // Get the current time
    srand((unsigned int)t); // Seed the random number generator once with the current time
}