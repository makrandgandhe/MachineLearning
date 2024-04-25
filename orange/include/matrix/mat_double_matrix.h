#ifndef __MAT_DOUBLE_MATRIX_H
#define __MAT_DOUBLE_MATRIX_H 1

#include<inttypes.h>

// matrix

typedef struct __mat_double mat_double; // opaque type

mat_double* mat_double_create_new(dimension_t number_of_rows, dimension_t number_of_columns);
mat_double* mat_double_create_new_filled(dimension_t number_of_rows, dimension_t number_of_columns, double fill_value);
void mat_double_destroy(mat_double* matrix);
void mat_double_set(mat_double* matrix, index_t row_index, index_t column_index, double value);
double mat_double_get(mat_double* matrix, index_t row_index, index_t column_index);
void mat_double_get_dimensions(mat_double* matrix, dimension_t* number_of_rows, dimension_t* number_of_columns);
mat_double* mat_double_transpose(mat_double* matrix);
mat_double* mat_double_matrix_multiplication(mat_double* left_matrix, mat_double* right_matrix);
mat_double* mat_double_scalar_multiplication(double value, mat_double* matrix);
mat_double* mat_double_matrix_addition(mat_double* left_matrix, mat_double* right_matrix);
mat_double* mat_double_matrix_substraction(mat_double* left_matrix, mat_double* right_matrix);
int mat_double_to_csv(mat_double* matrix, const char* file_name);
mat_double* mat_double_from_csv(const char* file_name);
void mat_double_print(mat_double* matrix, const char* title);


// vector

typedef struct __vec_double vec_double; // opaque type

vec_double* vec_double_create_new_row(dimension_t length);
vec_double* vec_double_create_new_column(dimension_t length);
vec_double* vec_double_create_new_row_filled(dimension_t length, double fill_value);
vec_double* vec_double_create_new_column_filled(dimension_t length, double fill_value);
void vec_double_destroy(vec_double* vector);
void vec_double_set(vec_double* vector, index_t index, double value);
double vec_double_get(vec_double* vector, index_t index);
void vec_double_get_length(vec_double* vector, dimension_t* length);
void vec_double_get_vector_type(vec_double* vector, char* vector_type);
vec_double* vec_double_transpose(vec_double* vector);
mat_double* vec_double_vector_multiplication(vec_double* left_vector, vec_double* right_vector);
vec_double* vec_double_matrix_vector_multiplication(mat_double* matrix, vec_double* vector);
vec_double* vec_double_scalar_multiplication(double value, vec_double* vector);
vec_double* vec_double_vector_addition(vec_double* left_vector, vec_double* right_vector);
vec_double* vec_double_vector_substraction(vec_double* left_vector, vec_double* right_vector);
int vec_double_to_csv(vec_double* vector, const char* file_name);
vec_double* vec_double_from_csv(const char* file_name);
void vec_double_print(vec_double* vector, const char* title);

// msic functions

vec_double* mat_double_column_to_vector(mat_double* matrix, index_t column_index);
int mat_double_box_copy(mat_double* source_matrix, index_t source_row_start, index_t source_column_start, dimension_t number_of_rows, dimension_t number_of_columns, mat_double* destination_matrix, index_t destination_row_start, index_t destination_column_start );

void mat_double_swap_rows(mat_double* matrix, index_t index1, index_t index2);
mat_double* mat_double_get_copy(mat_double* matrix);


// to be transfered to some other file

int random_number_in_range(int start, int end);


#endif
