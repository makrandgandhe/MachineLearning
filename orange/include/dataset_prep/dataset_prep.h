#ifndef __DATASET_PREP_H
#define __DATASET_PREP_H 1

#include<matrix.h>
#include<dataset.h>
#include<dataset_prep/dataset_prep_options.h>

dataset* dataset_shuffle_rows(dataset* data_set, unsigned char iterations);
dataset** dataset_split_random(dataset* data_set, uint8_t first_portion_percentage, uint8_t number_of_shuffles);
dataset* dataset_encode(dataset* data_set, ORANGE_DATASET_ENCODING_TYPE encoding_type);
dataset** dataset_remove_columns(dataset* data_set, char** columns);



#endif