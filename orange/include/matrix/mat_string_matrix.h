#ifndef __MAT_STRING_MATRIX_H
#define __MAT_STRING_MATRIX_H 1

#include <inttypes.h>
#include <matrix.h>

#define SET_STRING_DEFAULT_CAPACITY 1024
#define SET_STRING_DEFAULT_SIZE_INCREMENT 100

// matrix

typedef struct __mat_string mat_string; // opaque type

mat_string *mat_string_create_new(dimension_t number_of_rows, dimension_t number_of_columns);
mat_string *mat_string_create_new_filled(dimension_t number_of_rows, dimension_t number_of_columns, double fill_value);
void mat_string_destroy(mat_string *matrix);
void mat_string_set(mat_string *matrix, index_t row_index, index_t column_index, double value);
double mat_string_get(mat_string *matrix, index_t row_index, index_t column_index);
void mat_string_get_dimensions(mat_string *matrix, dimension_t *number_of_rows, dimension_t *number_of_columns);
mat_string *mat_string_transpose(mat_string *matrix);
int mat_string_to_csv(mat_string *matrix, const char *file_name);
mat_string *mat_string_from_csv(const char *file_name);
void mat_string_print(mat_string *matrix, const char *title);

// set

typedef struct __set_string set_string; // opaque type

set_string *set_string_create_new();
void set_string_destroy(set_string *set);
dimension_t set_string_get_size(set_string *set);
dimension_t set_string_get_capacity(set_string *set);
dimension_t set_string_add(set_string *set, const char *string);
int set_string_contains(set_string *set, const char *string);
int set_string_for_each(set_string *set, void (*func)(char *, index_t, void *), void *ptr);
void set_string_print(set_string *set, const char *title);
set_string *set_string_from_csv(const char *file_name);
int set_string_to_csv(set_string* set, const char *file_name);

#endif

