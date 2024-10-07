#include <matrix.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

typedef struct __mat_double
{
    dimension_t number_of_rows;
    dimension_t number_of_columns;
    double **data;
} mat_double;

mat_double *mat_double_create_new(dimension_t number_of_rows, dimension_t number_of_columns)
{
    mat_double *matrix;
    double **data;
    if (number_of_rows <= 0 || number_of_columns <= 0)
        return NULL;
    data = (double **)create_new_matrix(sizeof(double), number_of_rows, number_of_columns);
    if (!data)
        return NULL;
    matrix = (mat_double *)malloc(sizeof(mat_double));
    if (!matrix)
    {
        destroy_matrix((void **)data, number_of_rows);
        return NULL;
    }
    matrix->data = data;
    matrix->number_of_rows = number_of_rows;
    matrix->number_of_columns = number_of_columns;
    return matrix;
}

mat_double *mat_double_create_new_filled(dimension_t number_of_rows, dimension_t number_of_columns, double fill_value)
{
    mat_double *matrix;
    index_t r, c;
    double *ptr;
    matrix = mat_double_create_new(number_of_rows, number_of_columns);
    if (!matrix)
        return NULL;
    for (r = 0; r < number_of_rows; ++r)
    {
        ptr = matrix->data[r];
        for (c = 0; c < number_of_columns; ++c, ++ptr)
        {
            *ptr = fill_value;
        }
    }
    return matrix;
}

mat_double* mat_double_create_new_random_filled(dimension_t number_of_rows, dimension_t number_of_columns, double random_range_from, double random_range_to)
{
    mat_double *matrix;
    index_t r, c;
    double *ptr;
    matrix = mat_double_create_new(number_of_rows, number_of_columns);
    if (!matrix)
        return NULL;
    mu_random_set_seed();
    for (r = 0; r < number_of_rows; ++r)
    {
        ptr = matrix->data[r];
        for (c = 0; c < number_of_columns; ++c, ++ptr)
        {
            *ptr = mu_random_double(random_range_from, random_range_to);
        }
    }
    return matrix;
}

void mat_double_destroy(mat_double *matrix)
{
    if (!matrix)
        return;
    destroy_matrix((void **)matrix->data, matrix->number_of_rows);
    free(matrix);
}

void mat_double_set(mat_double *matrix, index_t row_index, index_t column_index, double value)
{
    if (!matrix || row_index >= matrix->number_of_rows || column_index >= matrix->number_of_columns)
        return;
    matrix->data[row_index][column_index] = value;
}

double mat_double_get(mat_double *matrix, index_t row_index, index_t column_index)
{
    if (!matrix || row_index >= matrix->number_of_rows || column_index >= matrix->number_of_columns)
        return 0;
    return matrix->data[row_index][column_index];
}

void mat_double_get_dimensions(mat_double *matrix, dimension_t *number_of_rows, dimension_t *number_of_columns)
{
    if (!matrix)
        return;
    if (number_of_rows)
        *number_of_rows = matrix->number_of_rows;
    if (number_of_columns)
        *number_of_columns = matrix->number_of_columns;
}

mat_double* mat_double_transpose(mat_double* matrix, mat_double* target_container)
{
    mat_double *transposed_matrix;
    dimension_t tr, tc;
    index_t r, c;
    if(target_container)
    {
        mat_double_get_dimensions(target_container, &tr, &tc);
        if(tr!=matrix->number_of_columns || tc != matrix->number_of_columns)
        {
            return NULL; // Invalid dimensions of target container
        }
        transposed_matrix = target_container;
    }
    else
    {
        transposed_matrix = mat_double_create_new(matrix->number_of_columns, matrix->number_of_rows);
    }
    if (!transposed_matrix)
        return NULL;
    for (r = 0; r < matrix->number_of_rows; ++r)
    {
        for (c = 0; c < matrix->number_of_columns; ++c)
        {
            transposed_matrix->data[c][r] = matrix->data[r][c];
        }
    }
    return transposed_matrix;
}

mat_double* mat_double_matrix_multiplication(mat_double* left_matrix, mat_double* right_matrix, mat_double* target_container)
{
    mat_double *result_matrix;
    index_t r, c, m;
    dimension_t tr, tc;
    double value;
    if (!left_matrix || !right_matrix || left_matrix->number_of_columns != right_matrix->number_of_rows)
        return NULL;
    if(target_container)
    {
        mat_double_get_dimensions(target_container, &tr, &tc);
        if(tr != left_matrix->number_of_rows || tc != right_matrix->number_of_columns)
        {
            return NULL; // Invalid dimensions of target container
        }
        result_matrix = target_container;
    }
    else
    {
        result_matrix = mat_double_create_new(left_matrix->number_of_rows, right_matrix->number_of_columns);
    }

    if (!result_matrix)
        return NULL;
    for (r = 0; r < result_matrix->number_of_rows; ++r)
    {
        for (c = 0; c < result_matrix->number_of_columns; ++c)
        {
            value = 0;
            for (m = 0; m < left_matrix->number_of_columns; ++m)
            {
                value += left_matrix->data[r][m] * right_matrix->data[m][c];
            }
            result_matrix->data[r][c] = value;
        }
    }
    return result_matrix;
}

// mat_double *mat_double_matrix_multiplication_dac(mat_double *left_matrix, mat_double *right_matrix, mat_double *result_matrix)
// {
//     index_t r, c, m;
//     double value;
//     if (!left_matrix || !right_matrix || left_matrix->number_of_columns != right_matrix->number_of_rows)
//         return NULL;
//     if (left_matrix->number_of_rows != left_matrix->number_of_columns || ceil(log2(left_matrix->number_of_rows)) != floor(log2(left_matrix->number_of_rows)))
//         return NULL;
//     if (right_matrix->number_of_rows != right_matrix->number_of_columns || ceil(log2(right_matrix->number_of_rows)) != floor(log2(right_matrix->number_of_rows)))
//         return NULL;
//     if (!result_matrix)
//     {
//         result_matrix = mat_double_create_new(left_matrix->number_of_rows, right_matrix->number_of_columns);
//     }
//     else if (result_matrix->number_of_rows != left_matrix->number_of_columns || result_matrix->number_of_columns != right_matrix->number_of_columns)
//     {
//         return NULL;
//     }
//     if (!result_matrix)
//     {
//         return NULL;
//     }
// }

// mat_double *_mat_double_matrix_multiplication_dac(mat_double *left_matrix, mat_double *right_matrix)
// {
//     index_t r, c;
//     dimension_t size, partition_size;
//     double a, b, c, d, e, f, g, h;
//     double e1, e2, e3, e4;
//     size = left_matrix->number_of_rows;
//     mat_double *result_matrix;
//     mat_double *A, *B, *C, *D, *E, *F, *G, *H;
//     mat_double *AE, *BG, *AF, *BH, *CE, *DG, *CF, *DH;
//     mat_double *E1, *E2, *E3, *E4;
//     result_matrix = mat_double_create_new(size, size);
//     if (size == 2)
//     {
//         a = left_matrix->data[0][0];
//         b = left_matrix->data[0][1];
//         c = left_matrix->data[1][0];
//         d = left_matrix->data[1][1];
//         e = right_matrix->data[0][0];
//         f = right_matrix->data[0][1];
//         g = right_matrix->data[1][0];
//         h = right_matrix->data[1][1];
//         e1 = (a * e) + (b * g);
//         e2 = (a * f) + (b * h);
//         e3 = (c * e) + (d * g);
//         e4 = (c * f) + (d * h);
//         result_matrix->data[0][0] = e1;
//         result_matrix->data[0][1] = e2;
//         result_matrix->data[1][0] = e3;
//         result_matrix->data[1][1] = e4;
//     }
//     else
//     {
//         partition_size = size / 2;
//         A = mat_double_create_new(partition_size, partition_size);
//         B = mat_double_create_new(partition_size, partition_size);
//         C = mat_double_create_new(partition_size, partition_size);
//         D = mat_double_create_new(partition_size, partition_size);
//         E = mat_double_create_new(partition_size, partition_size);
//         F = mat_double_create_new(partition_size, partition_size);
//         G = mat_double_create_new(partition_size, partition_size);
//         H = mat_double_create_new(partition_size, partition_size);
//         mat_double_box_copy(left_matrix, 0, 0, partition_size, partition_size, A, 0, 0);
//         mat_double_box_copy(left_matrix, 0, partition_size, partition_size, partition_size, B, 0, 0);
//         mat_double_box_copy(left_matrix, partition_size, 0, partition_size, partition_size, C, 0, 0);
//         mat_double_box_copy(left_matrix, partition_size, partition_size, partition_size, partition_size, D, 0, 0);
//         mat_double_box_copy(right_matrix, 0, 0, partition_size, partition_size, E, 0, 0);
//         mat_double_box_copy(right_matrix, 0, partition_size, partition_size, partition_size, F, 0, 0);
//         mat_double_box_copy(right_matrix, partition_size, 0, partition_size, partition_size, G, 0, 0);
//         mat_double_box_copy(right_matrix, partition_size, partition_size, partition_size, partition_size, H, 0, 0);
//         AE = _mat_double_matrix_multiplication_dac(A, E);
//         BG = _mat_double_matrix_multiplication_dac(B, G);
//         AF = _mat_double_matrix_multiplication_dac(A, F);
//         BH = _mat_double_matrix_multiplication_dac(B, H);
//         CE = _mat_double_matrix_multiplication_dac(C, E);
//         DG = _mat_double_matrix_multiplication_dac(D, G);
//         CF = _mat_double_matrix_multiplication_dac(C, F);
//         DH = _mat_double_matrix_multiplication_dac(D, H);
//         E1 = mat_double_matrix_addition(AE, BG);
//         E2 = mat_double_matrix_addition(AF, BH);
//         E3 = mat_double_matrix_addition(CE, DG);
//         E4 = mat_double_matrix_addition(CF, DH);
//         mat_double_box_copy(E1, 0, 0, partition_size, partition_size, result_matrix, 0, 0);
//         mat_double_box_copy(E2, 0, 0, partition_size, partition_size, result_matrix, 0, partition_size);
//         mat_double_box_copy(E3, 0, 0, partition_size, partition_size, result_matrix, partition_size, 0);
//         mat_double_box_copy(E4, 0, 0, partition_size, partition_size, result_matrix, partition_size, partition_size);

//         mat_double_destroy(AE);
//         mat_double_destroy(BG);
//         mat_double_destroy(AF);
//         mat_double_destroy(BH);
//         mat_double_destroy(CE);
//         mat_double_destroy(DG);
//         mat_double_destroy(CF);
//         mat_double_destroy(DH);
//         mat_double_destroy(E1);
//         mat_double_destroy(E2);
//         mat_double_destroy(E3);
//         mat_double_destroy(E4);
//         mat_double_destroy(A);
//         mat_double_destroy(B);
//         mat_double_destroy(C);
//         mat_double_destroy(D);
//         mat_double_destroy(E);
//         mat_double_destroy(F);
//         mat_double_destroy(G);
//         mat_double_destroy(H);
//     }
//     return result_matrix;
// }

mat_double* mat_double_scalar_multiplication(double scalar_value, mat_double* matrix, mat_double* target_container)
{
    mat_double *result_matrix;
    index_t r, c;
    dimension_t tr, tc;
    if (!matrix)
        return NULL;
    if(target_container)
    {
        mat_double_get_dimensions(target_container, &tr, &tc);
        if(tr != matrix->number_of_rows || tc != matrix->number_of_columns)
        {
            return NULL; // Invalid dimensions of target container
        }
        result_matrix = target_container;
    }
    else
    {
        result_matrix = mat_double_create_new(matrix->number_of_rows, matrix->number_of_columns);
    }

    if (!result_matrix)
        return NULL;
    for (r = 0; r < matrix->number_of_rows; ++r)
    {
        for (c = 0; c < matrix->number_of_columns; ++c)
        {
            result_matrix->data[r][c] = scalar_value * matrix->data[r][c];
        }
    }
    return result_matrix;
}

mat_double* mat_double_matrix_addition(mat_double* left_matrix, mat_double* right_matrix, mat_double* target_container)
{
    mat_double *result_matrix;
    index_t r, c;
    dimension_t tr, tc;
    if (!left_matrix || !right_matrix || left_matrix->number_of_rows != right_matrix->number_of_rows || left_matrix->number_of_columns != right_matrix->number_of_columns)
        return NULL;
    if(target_container)
    {
        mat_double_get_dimensions(target_container, &tr, &tc);
        if(tr != left_matrix->number_of_rows || tc != left_matrix->number_of_columns)
        {
            return NULL; // Invalid dimensions of target container
        }
        result_matrix = target_container;
    }
    else
    {
        result_matrix = mat_double_create_new(left_matrix->number_of_rows, left_matrix->number_of_columns);
    }
    if (!result_matrix)
        return NULL;
    for (r = 0; r < left_matrix->number_of_rows; ++r)
    {
        for (c = 0; c < left_matrix->number_of_columns; ++c)
        {
            result_matrix->data[r][c] = left_matrix->data[r][c] + right_matrix->data[r][c];
        }
    }
    return result_matrix;
}

mat_double *mat_double_matrix_substraction(mat_double *left_matrix, mat_double *right_matrix, mat_double* target_container)
{
    mat_double *result_matrix;
    index_t r, c;
    dimension_t tr, tc;
    if (!left_matrix || !right_matrix || left_matrix->number_of_rows != right_matrix->number_of_rows || left_matrix->number_of_columns != right_matrix->number_of_columns)
        return NULL;
    if(target_container)
    {
        mat_double_get_dimensions(target_container, &tr, &tc);
        if(tr != left_matrix->number_of_rows || tc != left_matrix->number_of_columns)
        {
            return NULL; // Invalid dimensions of target container
        }
        result_matrix = target_container;
    }
    else
    {
        result_matrix = mat_double_create_new(left_matrix->number_of_rows, left_matrix->number_of_columns);
    }
    if (!result_matrix)
        return NULL;
    for (r = 0; r < left_matrix->number_of_rows; ++r)
    {
        for (c = 0; c < left_matrix->number_of_columns; ++c)
        {
            result_matrix->data[r][c] = left_matrix->data[r][c] - right_matrix->data[r][c];
        }
    }
    return result_matrix;
}

int mat_double_to_csv(mat_double *matrix, const char *file_name)
{
    FILE *f;
    index_t r, c;
    dimension_t nr, nc;
    if (!matrix)
        return -1;
    nr = matrix->number_of_rows;
    nc = matrix->number_of_columns - 1; // cool trick for putting \n at the end of line
    f = fopen(file_name, "w");
    if (!f)
        return -2;
    for (r = 0; r < nr; ++r)
    {
        for (c = 0; c < nc; ++c)
        {
            fprintf(f, "%"
                       "lf"
                       ",",
                    matrix->data[r][c]);
        }
        fprintf(f, "%"
                   "lf"
                   "\n",
                matrix->data[r][c]);
    }
    fclose(f);
    return r;
}

mat_double *mat_double_from_csv(const char *file_name)
{
    FILE *f;
    int i, e, j, n, k, d;
    int lineLength = 1000;
    int chunkSize = 1000;
    int newSize;
    char ch;
    char line[lineLength];
    char **lines;
    char **tmp;

    dimension_t nr, nc;
    char val_str[20];
    double val;

    mat_double *matrix;

    if (!file_name)
        return NULL;
    f = fopen(file_name, "r");
    if (!f)
        return NULL;

    lines = (char **)malloc(sizeof(char *) * chunkSize);
    if (!lines)
    {
        fclose(f);
        return NULL;
    }
    for (e = 0; e < chunkSize; ++e)
    {
        lines[e] = (char *)malloc(sizeof(char) * lineLength);
        if (!lines[e])
        {
            for (j = 0; j < e; ++j)
                free(lines[j]);
            free(lines);
            fclose(f);
            return NULL;
        }
    }
    n = 0;
    while (1)
    {
        i = 0;
        // reading a line
        while (1)
        {
            ch = fgetc(f);
            if (feof(f))
                break;
            if (ch == '\n')
            {
                line[i++] = ch;
                break;
            }
            line[i++] = ch;
        }
        if (i)
        {
            if (n && n % chunkSize == 0)
            {
                tmp = lines;
                newSize = n + chunkSize;
                lines = (char **)malloc(sizeof(char *) * newSize);
                if (!lines)
                {
                    for (e = 0; e < n; ++e)
                        free(tmp[e]);
                    free(tmp);
                    fclose(f);
                    return NULL;
                }
                for (e = 0; e < n; ++e)
                {
                    lines[e] = tmp[e];
                }
                free(tmp);
                for (e = n; e < newSize; ++e)
                {
                    lines[e] = (char *)malloc(sizeof(char) * lineLength);
                    if (!lines[e])
                    {
                        for (j = 0; j < e; ++j)
                            free(lines[j]);
                        free(lines);
                        fclose(f);
                        return NULL;
                    }
                }
            }
            strcpy(lines[n++], line);
        }
        if (feof(f))
        {
            fclose(f);
            break;
        }
    }

    // code to create and populate matrix
    matrix = NULL;
    if (n)
    {
        nr = n;
        // logic to get no of columns
        nc = 1;
        for (e = 0; e < lineLength; ++e)
        {
            ch = lines[0][e];
            if (ch == ',')
                ++nc;
        }
        matrix = mat_double_create_new(nr, nc);
        if (!matrix)
        {
            for (e = 0; e < n; ++e)
                free(lines[e]);
            free(lines);
            return NULL;
        }
        for (i = 0; i < nr; ++i)
        {
            k = 0;
            for (j = 0; j < nc; ++j)
            {
                strcpy(val_str, "");
                if (j == 0)
                {
                    e = 0;
                }
                else
                {
                    e = k + 1;
                }
                for (k = e; lines[i][k] != ',' && lines[i][k] != '\n'; ++k)
                    ;
                d = k - e;
                if (d)
                    strncpy(val_str, lines[i] + e, d);
                val_str[d] = '\0';
                val = strtod(val_str, NULL);
                mat_double_set(matrix, i, j, val);
            }
        }
    }
    for (e = 0; e < n; ++e)
        free(lines[e]);
    free(lines);
    return matrix;
}

void mat_double_print(mat_double *matrix, const char *title)
{
    index_t r, c;
    printf("============= %s =============\n", title);
    printf("Dimensions [%" PRId32 " X %" PRId32 "]\n", matrix->number_of_rows, matrix->number_of_columns);
    if (!matrix)
    {
        printf("NULL");
        return;
    }
    for (r = 0; r < matrix->number_of_rows; ++r)
    {
        for (c = 0; c < matrix->number_of_columns; ++c)
        {
            printf("%10"
                   "lf"
                   "",
                   matrix->data[r][c]);
        }
        printf("\n");
    }
}

// vector

typedef struct __vec_double
{
    dimension_t length;
    double *data;
    char vector_type;
} vec_double;

vec_double* vec_double_create_new(dimension_t length)
{
    return vec_double_create_new_row(length);
}

vec_double *vec_double_create_new_row(dimension_t length)
{
    vec_double *vector;
    double *data;
    if (!length)
        return NULL;
    data = (double *)create_new_vector(sizeof(double), length);
    if (!data)
        return NULL;
    vector = (vec_double *)malloc(sizeof(vec_double));
    if (!vector)
    {
        destroy_vector(data);
        return NULL;
    }
    vector->data = data;
    vector->length = length;
    vector->vector_type = 'r';
    return vector;
}

vec_double *vec_double_create_new_column(dimension_t length)
{
    vec_double *vector = vec_double_create_new_row(length);
    if (!vector)
        return NULL;
    vector->vector_type = 'c';
    return vector;
}

vec_double *vec_double_create_new_row_filled(dimension_t length, double fill_value)
{
    int e;
    vec_double *vector = vec_double_create_new_row(length);
    if (!vector)
        return NULL;
    for (e = 0; e < length; ++e)
        vector->data[e] = fill_value;
    return vector;
}

vec_double *vec_double_create_new_column_filled(dimension_t length, double fill_value)
{
    int e;
    vec_double *vector = vec_double_create_new_column(length);
    if (!vector)
        return NULL;
    for (e = 0; e < length; ++e)
        vector->data[e] = fill_value;
    return vector;
}

void vec_double_destroy(vec_double *vector)
{
    if (!vector)
        return;
    destroy_vector(vector->data);
    free(vector);
}

void vec_double_set(vec_double *vector, index_t index, double value)
{
    if (vector && index < vector->length)
        vector->data[index] = value;
}

double vec_double_get(vec_double *vector, index_t index)
{
    if (!vector || index >= vector->length)
        return 0;
    return vector->data[index];
}

void vec_double_get_length(vec_double *vector, dimension_t *length)
{
    if (vector && length)
        *length = vector->length;
}

void vec_double_get_vector_type(vec_double *vector, char *vector_type)
{
    if (vector && vector_type)
        *vector_type = vector->vector_type;
}

vec_double *vec_double_transpose(vec_double *vector, vec_double* target_container)
{
    vec_double *transposed_vector;
    if (!vector)
        return NULL;
    if(target_container)
    {
        if(target_container->length != vector->length)
        {
            return NULL; // Invalid target container dimensions
        }
        transposed_vector = target_container;
        if(vector->vector_type == 'c') target_container->vector_type = 'c';
        else target_container->vector_type = 'c';
    }
    else
    {
        if (vector->vector_type == 'c')
            transposed_vector = vec_double_create_new_row(vector->length);
        else
            transposed_vector = vec_double_create_new_column(vector->length);
    }
    if (!transposed_vector)
        return NULL;
    copy_vector(transposed_vector->data, vector->data, sizeof(double), vector->length);
    return transposed_vector;
}

mat_double *vec_double_vector_multiplication(vec_double *left_vector, vec_double *right_vector, mat_double* target_container)
{
    mat_double *matrix;
    dimension_t nr, nc;
    dimension_t left_vector_nr, left_vector_nc, right_vector_nc;
    index_t r, c, m;
    double value;
    if (!left_vector || !right_vector)
        return NULL;
    if (left_vector->vector_type == right_vector->vector_type)
        return NULL;
    left_vector_nr = left_vector->vector_type == 'c' ? left_vector->length : 1;
    left_vector_nc = left_vector->vector_type == 'r' ? left_vector->length : 1;
    // right_vector_nr = right_vector->vector_type == 'c' ? right_vector->length : 1;
    right_vector_nc = right_vector->vector_type == 'r' ? right_vector->length : 1;

    nr = left_vector_nr;
    nc = right_vector_nc;
    if(target_container)
    {
        if(target_container->number_of_rows != nr || target_container->number_of_columns != nc)
        {
            return NULL; // Invalid target container dimensions
        }
        matrix = target_container;
    }
    else
    {
        matrix = mat_double_create_new(nr, nc);
    }
    if (!matrix)
        return NULL;

    for (r = 0; r < matrix->number_of_rows; ++r)
    {
        for (c = 0; c < matrix->number_of_columns; ++c)
        {
            value = 0;
            for (m = 0; m < left_vector_nc; ++m)
            {
                value += (*(left_vector->data + r + m)) * (*(right_vector->data + m + c));
            }
            matrix->data[r][c] = value;
        }
    }
    return matrix;
}

vec_double *vec_double_matrix_vector_multiplication(mat_double *matrix, vec_double *vector, vec_double* target_container)
{
    index_t r, c;
    double value;
    vec_double *result_vector;
    if (!matrix || !vector)
        return NULL;
    if (vector->vector_type != 'c' || matrix->number_of_columns != vector->length)
        return NULL;
    if(target_container)
    {
        if(target_container->length != matrix->number_of_rows)
        {
            return NULL; // Invalid target_container dimensions
        }
        result_vector = target_container;
    }
    else
    {
        result_vector = vec_double_create_new_column(matrix->number_of_rows);
    }
    if (!result_vector)
        return NULL;
    for (r = 0; r < result_vector->length; ++r)
    {
        value = 0;
        for (c = 0; c < matrix->number_of_columns; ++c)
        {
            value += matrix->data[r][c] * vector->data[c];
        }
        result_vector->data[r] = value;
    }
    return result_vector;
}

vec_double *vec_double_scalar_multiplication(double value, vec_double *vector, vec_double* target_container)
{
    vec_double *result_vector;
    index_t e;
    if (!vector)
        return NULL;
    if(target_container)
    {
        if(target_container->length != vector->length)
        {
            return NULL; // Invalid target_container dimensions
        }
        result_vector = target_container;
    }
    else
    {
        result_vector = vector->vector_type == 'r' ? vec_double_create_new_row(vector->length) : vec_double_create_new_column(vector->length);
    }
    if (!result_vector)
        return NULL;
    for (e = 0; e < vector->length; ++e)
    {
        result_vector->data[e] = vector->data[e] * value;
    }
    return result_vector;
}

vec_double *vec_double_vector_addition(vec_double *left_vector, vec_double *right_vector, vec_double* target_container)
{
    vec_double *result_vector;
    index_t e;
    if (!left_vector || !right_vector)
        return NULL;
    if (left_vector->length != right_vector->length || left_vector->vector_type != right_vector->vector_type)
        return NULL;
    if(target_container)
    {
        if(target_container->length != left_vector->length)
        {
            return NULL; // Invalid target_container dimensions
        }
        result_vector = target_container;
    }
    else
    {
        result_vector = left_vector->vector_type == 'r' ? vec_double_create_new_row(left_vector->length) : vec_double_create_new_column(left_vector->length);
    }
    if (!result_vector)
        return NULL;
    for (e = 0; e < left_vector->length; ++e)
    {
        result_vector->data[e] = left_vector->data[e] + right_vector->data[e];
    }
    return result_vector;
}

vec_double *vec_double_vector_substraction(vec_double *left_vector, vec_double *right_vector, vec_double* target_container)
{
    vec_double *result_vector;
    index_t e;
    if (!left_vector || !right_vector)
        return NULL;
    if (left_vector->length != right_vector->length || left_vector->vector_type != right_vector->vector_type)
        return NULL;

    if(target_container)
    {
        if(target_container->length != left_vector->length)
        {
            return NULL; // Invalid target_container dimensions
        }
        result_vector = target_container;
    }
    else
    {
        result_vector = left_vector->vector_type == 'r' ? vec_double_create_new_row(left_vector->length) : vec_double_create_new_column(left_vector->length);
    }

    if (!result_vector)
        return NULL;
    for (e = 0; e < left_vector->length; ++e)
    {
        result_vector->data[e] = left_vector->data[e] - right_vector->data[e];
    }
    return result_vector;
}

int vec_double_to_csv(vec_double *vector, const char *file_name)
{
    FILE *f;
    index_t r, c;
    dimension_t nr, nc;
    if (!vector)
        return -4;
    nr = vector->vector_type == 'c' ? vector->length : 1;
    nc = vector->vector_type == 'r' ? vector->length : 1;
    f = fopen(file_name, "w");
    if (!f)
        return -2;
    for (r = 0; r < nr; ++r)
    {
        for (c = 0; c < nc - 1; ++c)
        {
            fprintf(f, "%"
                       "lf"
                       ",",
                    *(vector->data + r + c));
        }
        fprintf(f, "%"
                   "lf"
                   "\n",
                *(vector->data + r + c));
    }
    fclose(f);
    return r;
}

vec_double *vec_double_from_csv(const char *file_name)
{
    mat_double *matrix;
    vec_double *vector;
    dimension_t nr, nc;
    index_t r, c;
    if (!file_name)
        return NULL;
    matrix = mat_double_from_csv(file_name);
    if (!matrix)
        return NULL;
    if (matrix->number_of_rows != 1 && matrix->number_of_columns != 1)
    {
        mat_double_destroy(matrix);
        return NULL;
    }
    vector = matrix->number_of_rows == 1 ? vec_double_create_new_row(matrix->number_of_columns) : vec_double_create_new_column(matrix->number_of_rows);
    if (!vector)
    {
        mat_double_destroy(matrix);
        return NULL;
    }
    nr = vector->vector_type == 'c' ? vector->length : 1;
    nc = vector->vector_type == 'r' ? vector->length : 1;
    for (r = 0; r < nr; ++r)
    {
        for (c = 0; c < nc; ++c)
        {
            *(vector->data + r + c) = matrix->data[r][c];
        }
    }
    return vector;
}

void vec_double_print(vec_double *vector, const char *title)
{
    index_t r;
    if (!vector)
    {
        printf("NULL\n");
        return;
    }
    printf("============= %s =============\n", title);
    printf("Length = %d , Type = %c\n", vector->length, vector->vector_type);
    for (r = 0; r < vector->length; ++r)
    {
        printf("%10"
               "lf"
               "",
               vector->data[r]);
    }
    printf("\n");
}

vec_double *mat_double_column_to_vector(mat_double *matrix, index_t column_index)
{
    index_t r;
    if (!matrix || column_index >= matrix->number_of_columns)
        return NULL;
    vec_double *vector = vec_double_create_new_column(matrix->number_of_rows);
    if (!vector)
        return NULL;
    for (r = 0; r < matrix->number_of_rows; ++r)
    {
        vector->data[r] = matrix->data[r][column_index];
    }
    return vector;
}

vec_double* mat_double_row_to_vector(mat_double* matrix, index_t row_index, vec_double* target_container)
{
    index_t c;
    vec_double* vector;
    double* row;
    if (!matrix || row_index >= matrix->number_of_rows)
        return NULL;
    if(target_container)
    {
        if(target_container->length != matrix->number_of_columns)
        {
            printf("Comparision failed %d != %d",target_container->length, matrix->number_of_columns);
            return NULL; // Invalid target container dimensions
        }
        vector = target_container;
        vector->vector_type = 'r';
    }
    else
    {
        vector = vec_double_create_new_row(matrix->number_of_columns);
    }
    if(!vector) return NULL; // Insufficient memory
    row = matrix->data[row_index];
    for(c=0;c<matrix->number_of_columns;++c)
    {
        vector->data[c] = row[c];
    }
    return vector;
}

int mat_double_box_copy(mat_double *source_matrix, index_t source_row_start, index_t source_column_start, dimension_t number_of_rows, dimension_t number_of_columns, mat_double *destination_matrix, index_t destination_row_start, index_t destination_column_start)
{
    index_t rs, cs, rd, cd;
    if (!source_matrix || !destination_matrix || source_row_start > source_matrix->number_of_rows || source_row_start + number_of_rows > source_matrix->number_of_rows || source_column_start >= source_matrix->number_of_columns || source_column_start + number_of_columns > source_matrix->number_of_columns || destination_row_start >= destination_matrix->number_of_rows || destination_row_start + number_of_rows > destination_matrix->number_of_rows || destination_column_start >= destination_matrix->number_of_columns || destination_column_start + number_of_columns > destination_matrix->number_of_columns)
    {
        return -1;
    }
    for (rd = destination_row_start, rs = source_row_start; rd < destination_row_start + number_of_rows; ++rd, ++rs)
    {
        for (cd = destination_column_start, cs = source_column_start; cd < destination_column_start + number_of_columns; ++cd, ++cs)
        {
            destination_matrix->data[rd][cd] = source_matrix->data[rs][cs];
        }
    }
    return 0;
}

void mat_double_swap_rows(mat_double *matrix, index_t index1, index_t index2)
{
    if (!matrix || index1 >= matrix->number_of_rows || index2 >= matrix->number_of_rows)
        return;
    double *tmp_row;
    tmp_row = matrix->data[index1];
    matrix->data[index1] = matrix->data[index2];
    matrix->data[index2] = tmp_row;
}

mat_double *mat_double_get_copy(mat_double *matrix)
{
    mat_double *new_matrix;
    dimension_t nr, nc;
    if (!matrix)
        return NULL;
    nr = matrix->number_of_rows;
    nc = matrix->number_of_columns;
    new_matrix = mat_double_create_new(nr, nc);
    if (!new_matrix)
        return NULL;
    copy_matrix((void **)new_matrix->data, (void **)matrix->data, sizeof(double), nr, nc);
    return new_matrix;
}

int mat_double_copy_vector_to_column(mat_double *matrix, vec_double *vector, index_t column_index)
{
    index_t e;
    if (!matrix || !vector || column_index >= matrix->number_of_columns || matrix->number_of_rows != vector->length || vector->vector_type != 'c')
    {
        return -1;
    }
    for (e = 0; e < matrix->number_of_rows; ++e)
    {
        matrix->data[e][column_index] = vector->data[e];
    }
    return 0;
}


void vec_double_copy(vec_double* src, vec_double* dest, index_t src_start_index, index_t dest_start_index, dimension_t number_of_elements)
{
    if(!src || !dest)
    {
        return;
    }
    if(src_start_index + number_of_elements > src->length ||  dest_start_index + number_of_elements > dest->length)
    {
        return;
    }
    copy_vector(dest->data + dest_start_index, src->data + src_start_index, sizeof(double), number_of_elements);
}


index_t vec_double_exists(vec_double* vector, double value)
{
    index_t i;
    if(!vector)
    {
        return NULL;
    }
    for(i=0;i<vector->length;++i)
    {
        if(orange_double_equals(vector->data[i], value))
        {
            return i;
        }
    }
    return -1;
}


mat_double* vec_double_one_hot_encode(vec_double* vector)
{
    mat_double* matrix;
    index_t i, index;
    dimension_t r, c, k;
    if(!vector)
    {
        return NULL;
    }
    k = 0;
    for(i=0;i<vector->length;++i)
    {
        if()
    }
}


// this function will be moved to some other location
int random_number_in_range(int start, int end)
{
    int random_number;
    srand(time(0));
    random_number = rand();
    random_number = (random_number % (start - end + 1)) + start;
    return random_number;
}

// void mat_double_test2()
// {
//     mat_double *A, *B;
//     mat_double_from_csv("A.csv");
//     mat_double_from_csv("B.csv");
//     _mat_double_matrix_multiplication_dac(A, B);
// }

