#include<dataset.h>
#include<stdlib.h>

int main(int count, char** args)
{
    if(count < 6)
    {
        printf("Usage: ./prepare_test_train_dataset <dataset_file> <randomization_factor~[1:20]> <training_data_percentage> <training_dataset_file> <testing_dataset_file>\n");
        return 0;
    }
    dataset* data_set;
    dataset* training_dataset;
    dataset* testing_dataset;
    dataset** data_set_portions;
    int randomization_factor;
    int training_data_percentage;

    data_set = mat_double_from_csv(args[1]);
    if(!data_set)
    {
        printf("Unable to load dataset from file: %s\n", args[1]);
        return 0;
    }
    randomization_factor = atoi(args[2]);
    training_data_percentage = atoi(args[3]);
    data_set_portions = dataset_split_random(data_set, training_data_percentage, randomization_factor * 10);
    if(!data_set_portions)
    {
        mat_double_destroy(data_set);
        printf("Unable to split dataset file: %s\n", args[1]);
        return 0;
    }
    training_dataset = data_set_portions[0];
    testing_dataset = data_set_portions[1];

    mat_double_to_csv(training_dataset, args[4]);
    mat_double_to_csv(testing_dataset, args[5]);

    return 0;
}