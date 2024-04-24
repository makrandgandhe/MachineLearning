#ifndef __GENERIC_MATRIX_H
#define __GENERIC_MATRIX_H 1

#include<stdio.h>


void** create_new_matrix(size_t element_size, dimension_t number_of_rows, dimension_t number_of_columns);
void destroy_matrix(void** matrix, dimension_t number_of_rows);
void* create_new_vector(size_t element_size, dimension_t length);
void destroy_vector(void* vector);

int copy_vector(void* dest, void* src, size_t element_size, dimension_t length);

#endif