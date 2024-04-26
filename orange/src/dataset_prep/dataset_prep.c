#include<dataset.h>
#include<stdlib.h>
dataset* dataset_shuffle_rows(dataset* data_set, unsigned char iterations)
{
    int e;
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

dataset** dataset_split_random(dataset* data_set, uint8_t first_portion_percentage, uint8_t number_of_shuffles)
{
    dimension_t nr, nc, nr1, nr2, nc1, nc2;
    dataset *data_set1, *data_set2;
    dataset *shuffled_dataset;
    dataset** portions;
    if(!data_set || first_portion_percentage >= 100) return NULL;
    shuffled_dataset = dataset_shuffle_rows(data_set, number_of_shuffles);
    if(!shuffled_dataset) return NULL;
    mat_double_get_dimensions(data_set, &nr, &nc);
    nr1 = (dimension_t)( (double)nr * ((double)first_portion_percentage / 100) );
    nr2 = nr - nr1;
    nc1 = nc2 = nc;
    portions = (dataset**) malloc(sizeof(dataset*) * 2);
    if(!portions)
    {
        mat_double_destroy(shuffled_dataset);
        return NULL;
    }
    data_set1 = mat_double_create_new(nr1,nc1);
    if(!data_set1)
    {
        mat_double_destroy(shuffled_dataset);
        return NULL;
    }
    data_set2 = mat_double_create_new(nr2,nc2);
    if(!data_set2)
    {
        mat_double_destroy(shuffled_dataset);
        return NULL;
    }
    mat_double_box_copy(shuffled_dataset, 0, 0, nr1, nc, data_set1, 0, 0);
    mat_double_box_copy(shuffled_dataset, nr1, 0, nr2, nc, data_set2, 0, 0);
    mat_double_destroy(shuffled_dataset);
    portions[0] = data_set1;
    portions[1] = data_set2;
    return portions;
}
