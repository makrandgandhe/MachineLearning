#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<matrix.h>
#include<dataset.h>

#define DEBUG 1
#define NORMAL 0
#define NONE -1
#define PRINT_MODE NORMAL

void write_error_value_in_log_file(FILE* file,unsigned int iter, double error_value);
void write_final_parameters_in_output_file(FILE* file,vec_double* vector);

int main(int count, char** args)
{
    const char* dataset_file_name;
    FILE* log_file;
    FILE* output_file;

    double learning_rate;
    long unsigned int number_of_iterations;

    dataset* data_set;

    if(count < 6)
    {
        printf("Usage: train <dataset_file> <learning_rate> <number_of_iterations> <log_file> <output_file>\n");
        return 0;
    }
    dataset_file_name = args[1];
    learning_rate = strtod(args[2],NULL);
    number_of_iterations = atoi(args[3]);
    log_file = fopen(args[4],"w");
    if(!log_file)
    {
        printf("Unable to create/open log_file: %s\n",args[4]);
        return 0;
    }
    output_file = fopen(args[5],"w");
    if(!log_file)
    {
        printf("Unable to create/open output_file: %s\n",args[5]);
        fclose(log_file);
        return 0;
    }
    data_set = mat_double_from_csv(dataset_file_name);
    if(!data_set) 
    {
        printf("Unable to load %s\n",dataset_file_name);
        return 0;
    }
}


int train(dataset* data_set, double learning_rate, long unsigned int number_of_iterations, FILE* log_file, FILE* output_file)
{
    mat_double* I;
    mat_double* IT;
    vec_double* A;
    vec_double* M;
    vec_double* P;
    vec_double* E;
    vec_double* ET;
    mat_double* EE;
    vec_double* IT_E;
    vec_double* UM;
    vec_double* LR_N__IT_E;

    double error_value, prev_error_value;
    double LR_N;
    long unsigned int i = 0;

    dimension_t dataset_row_count, dataset_column_count;
    // load 
    
    if(PRINT_MODE == DEBUG) mat_double_print(data_set,"Data Set");
    mat_double_get_dimensions(data_set,&dataset_row_count, &dataset_column_count);
    if(PRINT_MODE == NORMAL) printf("Dataset - rows = %d, column = %d\n",dataset_row_count, dataset_column_count);
    LR_N = learning_rate / dataset_row_count;
    if(PRINT_MODE == DEBUG) printf("LR_N = %20.10lf\n", LR_N);
    A = mat_double_column_to_vector(data_set,dataset_column_count - 1);
    if(!A)
    {
        mat_double_destroy(data_set);
        printf("Unable to perform Column to Vector\n");
        return 0;
    }
    I = mat_double_create_new_filled(dataset_row_count, dataset_column_count, 1);
    if(!I)
    {
        mat_double_destroy(data_set);
        vec_double_destroy(A);
        printf("Unable to create I Matrix\n");
        return 0;
    }
    mat_double_box_copy(data_set, 0, 0,dataset_row_count, dataset_column_count - 1, I, 0, 1);
    if(PRINT_MODE == DEBUG) mat_double_print(I, "I matrix");
    if(PRINT_MODE == DEBUG) vec_double_print(A,"A Vector");
    M = vec_double_create_new_column_filled(dataset_column_count, 0.0);
    if(!M)
    {
        mat_double_destroy(data_set);
        vec_double_destroy(A);
        mat_double_destroy(I);
        printf("Unable to create I Matrix\n");
        return 0;
    }
    if(PRINT_MODE == DEBUG) vec_double_print(M,"M Vector");

    IT = mat_double_transpose(I);
    if(PRINT_MODE == DEBUG) mat_double_print(IT,"IT");
    if(!IT)
    {
        mat_double_destroy(data_set);
        vec_double_destroy(A);
        mat_double_destroy(I);
        vec_double_destroy(M);
        printf("Unable to create I Matrix\n");
        return 0;
    }

    while(1)
    {
        P = vec_double_matrix_vector_multiplication(I, M);
        if(PRINT_MODE == DEBUG) vec_double_print(P,"P");
        if(!P)
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(M);
            printf("Unable to perform vec_double_matrix_vector_multiplication(I, M)\n");
            return 0;
        }
        E = vec_double_vector_substraction(P, A);
        if(PRINT_MODE == DEBUG) vec_double_print(E,"E");
        if(!E)
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(M);
            vec_double_destroy(P);
            printf("Unable to perform vec_double_vector_substraction(P, A)\n");
            return 0;
        }
        ET = vec_double_transpose(E);
        if(PRINT_MODE == DEBUG) vec_double_print(ET,"ET");
        if(!ET)
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(M);
            vec_double_destroy(P);
            vec_double_destroy(E);
            printf("Unable to perform vec_double_transpose(E)\n");
            return 0;
        }
        EE = vec_double_vector_multiplication(ET, E);
        if(PRINT_MODE == DEBUG) mat_double_print(EE,"EE");
        if(!EE)
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(M);
            vec_double_destroy(P);
            vec_double_destroy(E);
            vec_double_destroy(ET);
            printf("Unable to perform vec_double_transpose(E)\n");
            return 0;
        }
        error_value = mat_double_get(EE,0,0);
        error_value = error_value / (2 * dataset_row_count);
        if(PRINT_MODE == NORMAL) printf("%ld %lf\n",i,error_value);
        write_error_value_in_log_file(log_file, i, error_value);
        if(i > 1 && (error_value > prev_error_value || i==number_of_iterations))
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(P);
            vec_double_destroy(E);
            vec_double_destroy(ET);
            mat_double_destroy(EE);
            break;
        }
        prev_error_value = error_value;
        

        IT_E = vec_double_matrix_vector_multiplication(IT, E);
        if(PRINT_MODE == DEBUG) vec_double_print(IT_E,"IT_E");
        if(!IT_E)
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(M);
            vec_double_destroy(P);
            vec_double_destroy(E);
            vec_double_destroy(ET);
            mat_double_destroy(EE);
            printf("Unable to perform vec_double_matrix_vector_multiplication(IT, E)\n");
            return 0;
        }
        LR_N__IT_E = vec_double_scalar_multiplication(LR_N, IT_E);
        if(PRINT_MODE == DEBUG) vec_double_print(LR_N__IT_E,"LR_N__IT_E");
        if(!LR_N__IT_E)
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(M);
            vec_double_destroy(P);
            vec_double_destroy(E);
            vec_double_destroy(ET);
            mat_double_destroy(EE);
            vec_double_destroy(IT_E);
            printf("Unable to perform vec_double_scalar_multiplication(LR_N, IT_E)\n");
            return 0;
        }
        if(PRINT_MODE == DEBUG) vec_double_print(M,"M");
        UM = vec_double_vector_substraction(M, LR_N__IT_E);
        if(!UM)
        {
            mat_double_destroy(data_set);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            vec_double_destroy(M);
            vec_double_destroy(P);
            vec_double_destroy(E);
            vec_double_destroy(ET);
            mat_double_destroy(EE);
            vec_double_destroy(IT_E);
            vec_double_destroy(LR_N__IT_E);
            printf("Unable to perform vec_double_vector_substraction(M, LR_N__IT_E)\n");
            return 0;
        }
        vec_double_destroy(M);
        if(PRINT_MODE == DEBUG) vec_double_print(UM,"UM");
        M = UM;

        vec_double_destroy(P);
        vec_double_destroy(E);
        vec_double_destroy(ET);
        mat_double_destroy(EE);
        vec_double_destroy(IT_E);
        vec_double_destroy(LR_N__IT_E);
        ++i;
    }
    write_final_parameters_in_output_file(output_file, UM);
    if(PRINT_MODE == NORMAL) vec_double_print(UM,"final M");
    // vec_double_destroy(UM);
    return 0;
}


void write_error_value_in_log_file(FILE* file,unsigned int iter, double error_value)
{
    if(!file) return;
    char str[50];
    sprintf(str, "%d %20.15lf\n",iter, error_value);
    fputs(str, file);
}

void write_final_parameters_in_output_file(FILE* file,vec_double* vector)
{
    if(!file) return;
    char str[10000]; // expecting lots of features
    int n;
    dimension_t len;
    vec_double_get_length(vector,&len);
    for(index_t i = 0, n=0;i<len;++i)
    {
        n = sprintf(&str[n], "%-20.15lf\n", vec_double_get(vector,i));
    }
    fputs(str, file);
}