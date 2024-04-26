#ifndef __DATASET_PREP_H
#define __DATASET_PREP_H 1

#include<matrix.h>
#include<dataset.h>

dataset* dataset_shuffle_rows(dataset* data_set, unsigned char iterations);
dataset** dataset_split_random(dataset* data_set, uint8_t first_portion_percentage, uint8_t number_of_shuffles);

#endif