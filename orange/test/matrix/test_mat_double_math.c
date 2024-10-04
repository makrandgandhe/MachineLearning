#include<matrix.h>

vec_double* vector1;

void setup()
{
    vector1 = vec_double_create_new_column(5);
    vec_double_set(vector1, 0, 109.56);
    vec_double_set(vector1, 1, 99.00);
    vec_double_set(vector1, 2, 857.0008);
    vec_double_set(vector1, 3, 70.333);
    vec_double_set(vector1, 4, -80.6);
}

int test_mean()
{
    double mean, expected_mean;
    expected_mean = 211.05876;
    int s = vec_double_mean(vector1, &mean);
    if(s != 0) return -1;
    if(mean == expected_mean) return 0;
    return 1;
}

int test_standard_deviation()
{
    double sd, expected_sd, epsilon;
    expected_sd = 369.110029268357891;
    epsilon = 1e-9;
    int s = vec_double_standard_deviation(vector1, &sd);
    if(s != 0) return -1;
    if(orange_double_equals(sd, expected_sd, epsilon)) return 0;
    return 1;
}

int main()
{
    setup();
    int success;
    int all_success = 1;
    success = test_mean();
    if(success != 0)
    {
        all_success = 0;
        if(success == -1) printf("test_mean failed [Operation failed]\n");
        else printf("test_mean failed [result mis match]\n");
    }
    success = test_standard_deviation();
    if(success != 0)
    {
        all_success = 0;
        if(success == -1) printf("test_standard_deviation failed [Operation failed]\n");
        else printf("test_standard_deviation failed [result mis match]\n");
    }
    if(all_success) printf("All Test Cases PASSED\n");
    return 0;
}
