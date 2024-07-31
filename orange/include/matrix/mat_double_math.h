#ifndef __MAT_DOUBLE_MATH
#define __MAT_DOUBLE_MATH 1

#include<matrix/mat_double_matrix.h>

int orange_double_equals(double left, double right, double epsilon);

int vec_double_mean(vec_double* vector,double* mean);
int vec_double_standard_deviation(vec_double* vector, double* standard_deviation);


#endif