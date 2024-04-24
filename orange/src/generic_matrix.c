#include<matrix.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void** create_new_matrix(size_t element_size, dimension_t number_of_rows, dimension_t number_of_columns)
{
    void** matrix;
    dimension_t r;
    if(number_of_rows == 0 || number_of_columns == 0) return NULL;
    matrix = (void**) malloc(sizeof(void*) * number_of_rows);
    if(!matrix) return NULL;
    for(r=0;r<number_of_rows;++r)
    {
        matrix[r] = (void*) malloc(element_size * number_of_columns);
        if(!matrix[r])
        {
            for(--r;r>=0;--r) free(matrix[r]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}


void destroy_matrix(void** matrix, dimension_t number_of_rows)
{
    dimension_t r;
    if(!matrix) return;
    for(r=0;r<number_of_rows;++r)
    {
        free(matrix[r]);
    }
    free(matrix);
}

void* create_new_vector(size_t element_size, dimension_t length)
{
    void* vector;
    dimension_t l;
    if(length == 0) return NULL;
    vector = (void*) malloc(element_size * length);
    if(!vector) return NULL;
    return vector;
}

void destroy_vector(void* vector)
{
    if(vector) free(vector);
}

int copy_vector(void* dest, void* src, size_t element_size, dimension_t length)
{
    if(!dest || !src) return -1;
    memcpy(dest,src,element_size*length);
}