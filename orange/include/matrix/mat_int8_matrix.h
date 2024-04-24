#ifndef __MAT_INT8_MATRIX_H
#define __MAT_INT8_MATRIX_H 1

#include<inttypes.h>

// matrix

typedef struct __mat_int8 mat_int8; // opaque type

mat_int8* mat_int8_create_new(dimension_t number_of_rows, dimension_t number_of_columns);
mat_int8* mat_int8_create_new_filled(dimension_t number_of_rows, dimension_t number_of_columns, int8_t fill_value);
void mat_int8_destroy(mat_int8* matrix);
void mat_int8_set(mat_int8* matrix, index_t row_index, index_t column_index, int8_t value);
int8_t mat_int8_get(mat_int8* matrix, index_t row_index, index_t column_index);
void mat_int8_get_dimensions(mat_int8* matrix, dimension_t* number_of_rows, dimension_t* number_of_columns);
mat_int8* mat_int8_transpose(mat_int8* matrix);
mat_int8* mat_int8_matrix_multiplication(mat_int8* left_matrix, mat_int8* right_matrix);
mat_int8* mat_int8_scalar_multiplication(int8_t value, mat_int8* matrix);
mat_int8* mat_int8_matrix_addition(mat_int8* left_matrix, mat_int8* right_matrix);
mat_int8* mat_int8_matrix_substraction(mat_int8* left_matrix, mat_int8* right_matrix);
int mat_int8_to_csv(mat_int8* matrix, const char* file_name);
mat_int8* mat_int8_from_csv(const char* file_name);
void mat_int8_print(mat_int8* matrix, const char* title);


// vector

typedef struct __vec_int8 vec_int8; // opaque type

vec_int8* vec_int8_create_new_row(dimension_t length);
vec_int8* vec_int8_create_new_column(dimension_t length);
vec_int8* vec_int8_create_new_row_filled(dimension_t length, int8_t fill_value);
vec_int8* vec_int8_create_new_column_filled(dimension_t length, int8_t fill_value);
void vec_int8_destroy(vec_int8* vector);
void vec_int8_set(vec_int8* vector, index_t index, int8_t value);
int8_t vec_int8_get(vec_int8* vector, index_t index);
void vec_int8_get_length(vec_int8* vector, dimension_t* length);
void vec_int8_get_vector_type(vec_int8* vector, char* vector_type);
vec_int8* vec_int8_transpose(vec_int8* vector);
mat_int8* vec_int8_vector_multiplication(vec_int8* left_vector, vec_int8* right_vector);
vec_int8* vec_int8_matrix_vector_multiplication(mat_int8* matrix, vec_int8* vector);
vec_int8* vec_int8_scalar_multiplication(int8_t value, vec_int8* vector);
vec_int8* vec_int8_vector_addition(vec_int8* left_vector, vec_int8* right_vector);
vec_int8* vec_int8_vector_substraction(vec_int8* left_vector, vec_int8* right_vector);
int vec_int8_to_csv(vec_int8* vector, const char* file_name);
vec_int8* vec_int8_from_csv(const char* file_name);
void vec_int8_print(vec_int8* vector, const char* title);

// msic functions

vec_int8* mat_int8_column_to_vector(mat_int8* matrix, index_t column_index);
int mat_int8_box_copy(mat_int8* source_matrix, index_t source_row_start, index_t source_column_start, dimension_t number_of_rows, dimension_t number_of_columns, mat_int8* destination_matrix, index_t destination_row_start, index_t destination_column_start );


#endif
