#include<dataset.h>
#include<stdlib.h>
#include<math.h>

mat_double* test_accuracy(dataset* data_set, vec_double* trained_parameters);

int main(int count, char** args)
{
    if(count < 4)
    {
        printf("Usage: ./test_accuracy <dataset_file> <trained_parameters_file> <accuracy_report_file>\n");
        return 0;
    }
    dataset* data_set;
    vec_double* trained_parameters;
    mat_double* summary;


    data_set = mat_double_from_csv(args[1]);
    if(!data_set)
    {
        printf("Unable to load dataset from file: %s\n", args[1]);
        return 0;
    }

    trained_parameters = vec_double_from_csv(args[2]);
    if(!trained_parameters)
    {
        printf("Unable to load trained parameters from file: %s\n", args[2]);
        return 0;
    }
    summary = test_accuracy(data_set, trained_parameters);
    if(summary)
    {
        mat_double_to_csv(summary, args[3]);
        mat_double_destroy(summary);
    }
    mat_double_destroy(data_set);
    vec_double_destroy(trained_parameters);
    return 0;
}

mat_double* test_accuracy(dataset* data_set, vec_double* trained_parameters)
{
    double accuracy;

    mat_double* I;
    vec_double* A;
    vec_double* P;
    vec_double* E;
    vec_double* AC;

    mat_double* S; // summary
    

    dimension_t dataset_row_count, dataset_column_count;

    mat_double_get_dimensions(data_set, &dataset_row_count, &dataset_column_count);

    I = mat_double_create_new_filled(dataset_row_count, dataset_column_count, 1);
    if(!I)
    {
        printf("Unable to create I matrix\n");
        return NULL;
    }
    mat_double_box_copy(data_set, 0, 0, dataset_row_count, dataset_column_count - 1, I, 0, 1);
    A = mat_double_column_to_vector(data_set, dataset_column_count - 1);
    if(!A)
    {
        mat_double_destroy(I);
        printf("Unable to create A vector from dataset\n");
        return NULL;
    }
    P = vec_double_matrix_vector_multiplication(I, trained_parameters);
    if(!P)
    {
        mat_double_destroy(I);
        vec_double_destroy(A);
        printf("Unable to create A vector from dataset\n");
        return NULL;
    }
    vec_double_to_csv(P, "predicted.csv");
    E = vec_double_vector_substraction(P, A);
    if(!E)
    {
        mat_double_destroy(I);
        vec_double_destroy(A);
        vec_double_destroy(P);
        printf("Unable to get E vector (E = P - A)\n");
        return NULL;
    }
    // The following code can be turned into a vector function (eg. vec_double_divide)
    index_t e;
    AC = vec_double_create_new_column(dataset_row_count);
    for(e=0;e<dataset_row_count;++e)
    {
        accuracy = 100 - ( fabs(vec_double_get(E,e)) / vec_double_get(A,e) ) * 100;
        vec_double_set(AC, e, accuracy);
    }
    // ===============================================================================

    S = mat_double_create_new(dataset_row_count, 3);
    mat_double_copy_vector_to_column(S, A, 0);
    mat_double_copy_vector_to_column(S, P, 1);
    mat_double_copy_vector_to_column(S, AC, 2);


    mat_double_destroy(I);
    vec_double_destroy(A);
    vec_double_destroy(P);
    vec_double_destroy(E);
    vec_double_destroy(AC);

    return S;
}