#include<matrix.h>

#define DEBUG 1
#define NORMAL 0
#define PRINT_MODE DEBUG

#define ITERATIONS 1000

double learning_rate = 0.001;
const char* dataSetFileName = "../resources/Ice.csv";

int main()
{
    mat_double* dataset;
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
    dataset = mat_double_from_csv(dataSetFileName);
    if(!dataset) 
    {
        printf("Unable to load %s\n",dataSetFileName);
        return 0;
    }
    if(PRINT_MODE == DEBUG) mat_double_print(dataset,"Data Set");
    mat_double_get_dimensions(dataset,&dataset_row_count, &dataset_column_count);
    printf("Dataset - rows = %d, column = %d\n",dataset_row_count, dataset_column_count);
    LR_N = learning_rate / dataset_row_count;
    printf("LR_N = %20.10lf\n", LR_N);
    A = mat_double_column_to_vector(dataset,dataset_column_count - 1);
    if(!A)
    {
        mat_double_destroy(dataset);
        printf("Unable to perform Column to Vector\n");
        return 0;
    }
    I = mat_double_create_new_filled(dataset_row_count, dataset_column_count, 1);
    if(!I)
    {
        mat_double_destroy(dataset);
        vec_double_destroy(A);
        printf("Unable to create I Matrix\n");
        return 0;
    }
    mat_double_box_copy(dataset, 0, 0,dataset_row_count, dataset_column_count - 1, I, 0, 1);
    if(PRINT_MODE == DEBUG) mat_double_print(I, "I matrix");
    if(PRINT_MODE == DEBUG) vec_double_print(A,"A Vector");
    M = vec_double_create_new_column_filled(dataset_column_count, 0.0);
    if(!M)
    {
        mat_double_destroy(dataset);
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
        mat_double_destroy(dataset);
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
            mat_double_destroy(dataset);
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
            mat_double_destroy(dataset);
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
            mat_double_destroy(dataset);
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
            mat_double_destroy(dataset);
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
        printf("%ld %lf\n",i,error_value);
        if(i > 0 && error_value > prev_error_value)
        {
            mat_double_destroy(dataset);
            vec_double_destroy(A);
            mat_double_destroy(I);
            mat_double_destroy(IT);
            // vec_double_destroy(M);
            vec_double_destroy(P);
            vec_double_destroy(E);
            vec_double_destroy(ET);
            mat_double_destroy(EE);
            printf("BREAKING LOOP\n\n");
            printf("error_value = %20.10lf , prev_error_value = %20.10lf\n",error_value, prev_error_value);
            break;
        }
        prev_error_value = error_value;
        

        IT_E = vec_double_matrix_vector_multiplication(IT, E);
        if(PRINT_MODE == DEBUG) vec_double_print(IT_E,"IT_E");
        if(!IT_E)
        {
            mat_double_destroy(dataset);
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
        vec_double_print(LR_N__IT_E,"LR_N__IT_E");
        if(!LR_N__IT_E)
        {
            mat_double_destroy(dataset);
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
        UM = vec_double_vector_substraction(M, LR_N__IT_E);
        if(!UM)
        {
            mat_double_destroy(dataset);
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
        M = UM;

        vec_double_destroy(P);
        vec_double_destroy(E);
        vec_double_destroy(ET);
        mat_double_destroy(EE);
        vec_double_destroy(IT_E);
        vec_double_destroy(LR_N__IT_E);
        ++i;
    }

    return 0;
}