#include<dataset_prep/dataset_prep.h>

dataset* dataset_shuffle_rows(dataset* data_set, unsigned char iterations)
{
    int e, f;
    dimension_t nr, nc;
    index_t random_index;
    mat_double* new_matrix;
    if(!data_set) return NULL;
    mat_double_get_dimensions(data_set, &nr, &nc);
    new_matrix = mat_double_get_copy(data_set);
    
    while(iterations)
    {
        for(e=0;e<nr-3;++e)
        {
            random_index = random_number_in_range(e+1,nr-1);
            mat_double_swap_rows(new_matrix, e, random_index);
        }
        --iterations;
    }
    return new_matrix;
}

dataset** mat_split_random(dataset* data_set, uint8_t first_portion_percentage, uint8_t number_of_shuffles)
{
    dimension_t nr, nc, nr1, nr2, nc1, nc2;
    mat_double *matrix1, *matrix2;
    mat_double *shuffled_matrix;
    mat_double** portions;
    if(!data_set || first_portion_percentage >= 100) return NULL;
    shuffled_matrix = mat_double_shuffle_rows(data_set, number_of_shuffles);
    if(!shuffled_matrix) return NULL;
    mat_double_get_dimensions(data_set, &nr, &nc);
    nr1 = nr * (first_portion_percentage / 100);
    nr2 = nr - nr1;
    nc1 = nc2 = nc;
    portions = (mat_double*) malloc(sizeof(mat_double*) * 2);
    if(!portions)
    {
        mat_double_destroy(shuffled_matrix);
        return NULL;
    }
    matrix1 = mat_double_create_new(nr1,nc1);
    if(!matrix1)
    {
        mat_double_destroy(shuffled_matrix);
        return NULL;
    }
    matrix2 = mat_double_create_new(nr2,nc2);
    if(!matrix2)
    {
        mat_double_destroy(shuffled_matrix);
        return NULL;
    }
    mat_double_box_copy(shuffled_matrix, 0, 0, nr1, nc, matrix1, 0, 0);
    mat_double_box_copy(shuffled_matrix, nr1, 0, nr2, nc, matrix2, 0, 0);
    mat_double_destroy(shuffled_matrix);
    portions[0] = matrix1;
    portions[2] = matrix2;
    return portions;
}
