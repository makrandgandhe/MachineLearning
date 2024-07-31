#include<matrix.h>
#include<stdlib.h>
#include<math.h>

int orange_double_equals(double left, double right, double epsilon)
{
    return fabs(left-right) < epsilon;
}


int vec_double_mean(vec_double* vector,double* mean)
{
    double m = 0.0;
    dimension_t len;
    index_t i;
    if(!vector || !mean) return -1;
    vec_double_get_length(vector,&len);
    for(i=0;i<len;++i)
    {
        m += vec_double_get(vector, i) / len;
    }
    *mean = m;
    return 0;
}

int vec_double_standard_deviation(vec_double* vector, double* standard_deviation)
{
    double mean;
    double sd = 0.0;
    double tmp;
    dimension_t len;
    index_t i;
    if(!vector || !standard_deviation) return -1;
    vec_double_mean(vector, &mean);
    vec_double_get_length(vector,&len);
    for(i=0;i<len;++i)
    {
        tmp = vec_double_get(vector, i) - mean;
        sd += (tmp * tmp) / (len - 1);
    }
    sd = sqrt(sd);
    *standard_deviation = sd;
    return 0;
}

