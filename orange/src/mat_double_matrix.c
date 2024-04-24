#include<matrix.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>

typedef struct __mat_double
{
    dimension_t number_of_rows;
    dimension_t number_of_columns;
    double** data;
} mat_double;

mat_double* mat_double_create_new(dimension_t number_of_rows, dimension_t number_of_columns)
{
    mat_double* matrix;
    double** data;
    if(number_of_rows == 0 || number_of_columns == 0) return NULL;
    data = (double**) create_new_matrix(sizeof(double),number_of_rows,number_of_columns);
    if(!data) return NULL;
    matrix = (mat_double*)malloc(sizeof(mat_double));
    if(!matrix)
    {
        destroy_matrix((void**)data,number_of_rows);
        return NULL;
    }
    matrix->data = data;
    matrix->number_of_rows = number_of_rows;
    matrix->number_of_columns = number_of_columns;
    return matrix;
}

mat_double* mat_double_create_new_filled(dimension_t number_of_rows, dimension_t number_of_columns, double fill_value)
{
    mat_double* matrix;
    index_t r,c;
    double* ptr;
    matrix = mat_double_create_new(number_of_rows,number_of_columns);
    if(!matrix) return NULL;
    for(r=0;r < number_of_rows;++r)
    {
        ptr = matrix->data[r];
        for(c=0;c<number_of_columns;++c,++ptr)
        {
            *ptr = fill_value;
        }
    }
    return matrix;
}

void mat_double_destroy(mat_double* matrix)
{
    if(!matrix) return;
    destroy_matrix((void**) matrix->data, matrix->number_of_rows);
    free(matrix);
}

void mat_double_set(mat_double* matrix, index_t row_index, index_t column_index, double value)
{
    if(!matrix || row_index >= matrix->number_of_rows || column_index >= matrix->number_of_columns ) return;
    matrix->data[row_index][column_index] = value;
}

double mat_double_get(mat_double* matrix, index_t row_index, index_t column_index)
{
    if(!matrix || row_index >= matrix->number_of_rows || column_index >= matrix->number_of_columns ) return 0;
    return matrix->data[row_index][column_index];
}

void mat_double_get_dimensions(mat_double* matrix, dimension_t* number_of_rows, dimension_t* number_of_columns)
{
    if(!matrix) return;
    if(number_of_rows) *number_of_rows = matrix->number_of_rows;
    if(number_of_columns) *number_of_columns = matrix->number_of_columns;
}

mat_double* mat_double_transpose(mat_double* matrix)
{
    mat_double* transposed_matrix;
    index_t r,c;
    transposed_matrix = mat_double_create_new(matrix->number_of_columns,matrix->number_of_rows);
    if(!transposed_matrix) return NULL;
    for(r=0;r<matrix->number_of_rows;++r)
    {
        for(c=0;c<matrix->number_of_columns;++c)
        {
            transposed_matrix->data[c][r] = matrix->data[r][c];
        }
    }
    return transposed_matrix;
}

mat_double* mat_double_matrix_multiplication(mat_double* left_matrix, mat_double* right_matrix)
{
    mat_double* result_matrix;
    index_t r,c, m;
    double value;
    if(!left_matrix || !right_matrix || left_matrix->number_of_columns != right_matrix->number_of_rows) return NULL;
    result_matrix = mat_double_create_new(left_matrix->number_of_rows,right_matrix->number_of_columns);
    if(!result_matrix) return NULL;
    for(r=0;r<result_matrix->number_of_rows;++r)
    {
        for(c=0;c<result_matrix->number_of_columns;++c)
        {
            value = 0;
            for(m=0;m<left_matrix->number_of_columns;++m)
            {
                value += left_matrix->data[r][m] * right_matrix->data[m][c];
            }
            result_matrix->data[r][c] = value;
        }
    }
    return result_matrix;
}


mat_double* mat_double_scalar_multiplication(double scalar_value, mat_double* matrix)
{
    mat_double* result_matrix;
    index_t r,c;
    if(!matrix) return NULL;
    result_matrix = mat_double_create_new(matrix->number_of_rows,matrix->number_of_columns);
    if(!result_matrix) return NULL;
    for(r=0;r<matrix->number_of_rows;++r)
    {
        for(c=0;c<matrix->number_of_columns;++c)
        {
            result_matrix->data[r][c] = scalar_value * matrix->data[r][c];
        }
    }
    return result_matrix;
}


mat_double* mat_double_matrix_addition(mat_double* left_matrix, mat_double* right_matrix)
{
    mat_double* result_matrix;
    index_t r,c;
    if(!left_matrix || !right_matrix || left_matrix->number_of_rows !=  right_matrix->number_of_rows 
                    || left_matrix->number_of_columns != right_matrix->number_of_columns) return NULL;
    result_matrix = mat_double_create_new(left_matrix->number_of_rows,left_matrix->number_of_columns);
    if(!result_matrix) return NULL;
    for(r=0;r<left_matrix->number_of_rows;++r)
    {
        for(c=0;c<left_matrix->number_of_columns;++c)
        {
            result_matrix->data[r][c]= left_matrix->data[r][c] + right_matrix->data[r][c];
        }
    }
    return result_matrix;
}




mat_double* mat_double_matrix_substraction(mat_double* left_matrix, mat_double* right_matrix)
{
    mat_double* result_matrix;
    index_t r,c;
    if(!left_matrix || !right_matrix || left_matrix->number_of_rows !=  right_matrix->number_of_rows 
                    || left_matrix->number_of_columns != right_matrix->number_of_columns) return NULL;
    result_matrix = mat_double_create_new(left_matrix->number_of_rows,left_matrix->number_of_columns);
    if(!result_matrix) return NULL;
    for(r=0;r<left_matrix->number_of_rows;++r)
    {
        for(c=0;c<left_matrix->number_of_columns;++c)
        {
            result_matrix->data[r][c]= left_matrix->data[r][c] - right_matrix->data[r][c];
        }
    }
    return result_matrix;
}

int mat_double_to_csv(mat_double* matrix, const char* file_name)
{
    FILE* f;
    index_t r,c;
    dimension_t nr ,nc;
    if(!matrix) return -1;
    nr = matrix->number_of_rows;
    nc = matrix->number_of_columns - 1; // cool trick for putting \n at the end of line
    f = fopen(file_name,"w");
    if(!f) return -2;
    for(r=0;r<nr;++r)
    {
        for(c=0;c<nc;++c)
        {
            fprintf(f,"%" "lf" ",", matrix->data[r][c]);
        }
        fprintf(f,"%" "lf" "\n", matrix->data[r][c]);
    }
    fclose(f);
    return r;
}

mat_double* mat_double_from_csv(const char* file_name)
{
    FILE* f;
    int i, e, j, n, k, d;
    int lineLength = 1000; 
    int chunkSize = 1000;
    int newSize;
    char ch;
    char line[lineLength];
    char** lines;
    char** tmp;

    dimension_t nr, nc;
    char val_str[20];
    double val;

    mat_double* matrix;

    if(!file_name) return NULL;
    f = fopen(file_name,"r");
    if(!f) return NULL;

    lines = (char**) malloc(sizeof(char*) * chunkSize);
    if(!lines) 
    {
        fclose(f);
        return NULL;
    }
    for(e=0;e<chunkSize;++e)
    {
        lines[e] = (char*) malloc(sizeof(char) * lineLength);
        if(!lines[e])
        {
            for(j=0;j<e;++j) free(lines[j]);
            free(lines);
            fclose(f);
            return NULL;
        }
    }
    n=0;
    while(1)
    {
        i = 0;
        // reading a line
        while(1)
        {
            ch = fgetc(f);
            if(feof(f)) break;
            if(ch == '\n')
            {
                line[i++] = ch;
                break;
            }
            line[i++] = ch;
        }
        if(i)
        {
            if(n && n % chunkSize == 0)
            {
                tmp = lines;
                newSize = n + chunkSize;
                lines = (char**) malloc(sizeof(char*) * newSize);
                if(!lines) 
                {
                    for(e = 0;e<n;++e) free(tmp[e]);
                    free(tmp);
                    fclose(f);
                    return NULL;
                }
                for(e=0;e<n;++e)
                {
                    lines[e] = tmp[e];
                }
                free(tmp);
                for(e=n;e<newSize;++e)
                {
                    lines[e] = (char*) malloc(sizeof(char) * lineLength);
                    if(!lines[e])
                    {
                        for(j=0;j<e;++j) free(lines[j]);
                        free(lines);
                        fclose(f);
                        return NULL;
                    }
                }
            }
            strcpy(lines[n++], line);
        }
        if(feof(f))
        {
            fclose(f);
            break;
        }
    }

    // code to create and populate matrix
    matrix = NULL;
    if(n)
    {
        nr = n;
        // logic to get no of columns
        nc = 1;
        for(e=0;e<lineLength;++e)
        {
            ch = lines[0][e];
            if(ch == ',') ++nc;
        }
        matrix = mat_double_create_new(nr,nc);
        if(!matrix)
        {
            for(e=0;e<n;++e) free(lines[e]);
            free(lines);
            return NULL;
        }
        for(i=0;i<nr;++i)
        {
            k = 0;
            for(j=0;j<nc;++j)
            {
                strcpy(val_str,"");
                if(j==0)
                {
                    e = 0;
                }
                else
                {
                    e = k + 1;
                }
                for(k = e;lines[i][k] != ',' && lines[i][k] != '\n';++k);
                d = k - e;
                if(d) strncpy(val_str,lines[i] + e, d);
                val_str[d] = '\0';
                val = strtod(val_str, NULL);
                mat_double_set(matrix,i,j,val);
            }
        }
    }
    for(e=0;e<n;++e) free(lines[e]);
    free(lines);
    return matrix;
}

void mat_double_print(mat_double* matrix, const char* title)
{
    index_t r,c;
    printf("============= %s =============\n",title);
    printf("Dimensions [%" PRId32 " X %" PRId32 "]\n", matrix->number_of_rows,matrix->number_of_columns);
    if(!matrix)
    {
        printf("NULL");
        return;
    }
    for(r=0;r<matrix->number_of_rows;++r)
    {
        for(c=0;c<matrix->number_of_columns;++c)
        {
            printf("%10" "lf" "", matrix->data[r][c]);
        }
        printf("\n");
    }
}

// vector


typedef struct __vec_double
{
    dimension_t length;
    double* data;
    char vector_type;
} vec_double;

vec_double* vec_double_create_new_row(dimension_t length)
{
    vec_double* vector;
    double* data;
    if(!length) return NULL;
    data = (double*) create_new_vector(sizeof(double), length);
    if(!data) return NULL;
    vector = (vec_double*) malloc(sizeof(vec_double));
    if(!vector)
    {
        destroy_vector(data);
        return NULL;
    }
    vector->data = data;
    vector->length = length;
    vector->vector_type = 'r';
    return vector;
}


vec_double* vec_double_create_new_column(dimension_t length)
{
    vec_double* vector =  vec_double_create_new_row(length);
    if(!vector) return NULL;
    vector->vector_type = 'c';
    return vector;
}


vec_double* vec_double_create_new_row_filled(dimension_t length, double fill_value)
{
    int e;
    vec_double* vector =  vec_double_create_new_row(length);
    if(!vector) return NULL;
    for(e=0;e<length;++e) vector->data[e] = fill_value;
    return vector;
}


vec_double* vec_double_create_new_column_filled(dimension_t length, double fill_value)
{
    int e;
    vec_double* vector =  vec_double_create_new_column(length);
    if(!vector) return NULL;
    for(e=0;e<length;++e) vector->data[e] = fill_value;
    return vector;
}


void vec_double_destroy(vec_double* vector)
{
    if(!vector) return;
    destroy_vector(vector->data);
    free(vector);
}


void vec_double_set(vec_double* vector, index_t index, double value)
{
    if(vector && index < vector->length) vector->data[index] = value;
}


double vec_double_get(vec_double* vector, index_t index)
{
    if(!vector || index >= vector->length) return 0;
    return vector->data[index];

}


void vec_double_get_length(vec_double* vector, dimension_t* length)
{
    if(vector && length) *length = vector->length;
}


void vec_double_get_vector_type(vec_double* vector, char* vector_type)
{
    if(vector && vector_type) *vector_type = vector->vector_type;
}


vec_double* vec_double_transpose(vec_double* vector)
{
    vec_double* transposed_vector;
    if(!vector) return NULL;
    if(vector->vector_type == 'c') transposed_vector = vec_double_create_new_row(vector->length);
    else transposed_vector = vec_double_create_new_column(vector->length);
    if(!transposed_vector) return NULL;
    copy_vector(transposed_vector->data, vector->data, sizeof(double), vector->length);
    return transposed_vector;
}


mat_double* vec_double_vector_multiplication(vec_double* left_vector, vec_double* right_vector)
{
    mat_double* matrix;
    dimension_t nr, nc;
    dimension_t left_vector_nr, left_vector_nc, right_vector_nc;
    index_t r,c,m;
    double value;
    if(!left_vector || !right_vector) return NULL;
    if(left_vector->vector_type == right_vector->vector_type) return NULL;
    if(left_vector->length != right_vector->length )
    {
        if((left_vector->vector_type == 'c' && right_vector->length == 1) || (right_vector->vector_type == 'r' && left_vector->length == 1))
        {
            // ok case
        }
        else 
        {
            return NULL;
        }
    }
    left_vector_nr = left_vector->vector_type == 'c' ? left_vector->length : 1;
    left_vector_nc = left_vector->vector_type == 'r' ? left_vector->length : 1;
    // right_vector_nr = right_vector->vector_type == 'c' ? right_vector->length : 1;
    right_vector_nc = right_vector->vector_type == 'r' ? right_vector->length : 1;

    nr = left_vector_nr;
    nc = right_vector_nc;
    matrix = mat_double_create_new(nr ,nc);
    if(!matrix) return NULL;

    for(r=0;r<matrix->number_of_rows;++r)
    {
        for(c=0;c<matrix->number_of_columns;++c)
        {
            value = 0;
            for(m=0;m<left_vector_nc;++m)
            {
                value += ( *(left_vector->data+r+m) ) * (*(right_vector->data+m+c));
            }
            matrix->data[r][c] = value;
        }
    }
    return matrix;
}

vec_double* vec_double_matrix_vector_multiplication(mat_double* matrix, vec_double* vector)
{
    index_t r,c;
    double value;
    vec_double* result_vector;
    if(!matrix || !vector) return NULL;
    if(vector->vector_type != 'c' || matrix->number_of_columns != vector->length) return NULL;
    result_vector = vec_double_create_new_column(matrix->number_of_rows);
    if(!result_vector) return NULL;
    for(r=0;r<result_vector->length;++r)
    {
        value = 0;
        for(c=0;c<matrix->number_of_columns;++c)
        {
            value += matrix->data[r][c] * vector->data[r];
        }
        result_vector->data[r] = value;
    }
    return result_vector;
}


vec_double* vec_double_scalar_multiplication(double value, vec_double* vector)
{
    vec_double* result_vector;
    index_t e;
    if(!vector) return NULL;
    result_vector = vector->vector_type == 'r'? vec_double_create_new_row(vector->length) : vec_double_create_new_column(vector->length);
    if(!result_vector) return NULL;
    for(e=0;e<vector->length;++e)
    {
        result_vector->data[e] = vector->data[e] * value;
    }
    return result_vector;
}


vec_double* vec_double_vector_addition(vec_double* left_vector, vec_double* right_vector)
{
    vec_double* result_vector;
    index_t e;
    if(!left_vector || !right_vector) return NULL;
    if(left_vector->length != right_vector->length || left_vector->vector_type != right_vector->vector_type) return NULL;
    result_vector = left_vector->vector_type == 'r'? vec_double_create_new_row(left_vector->length) : vec_double_create_new_column(left_vector->length);
    if(!result_vector) return NULL;
    for(e=0;e<left_vector->length;++e)
    {
        result_vector->data[e] = left_vector->data[e] + right_vector->data[e];
    }
    return result_vector;
}


vec_double* vec_double_vector_substraction(vec_double* left_vector, vec_double* right_vector)
{
    vec_double* result_vector;
    index_t e;
    if(!left_vector || !right_vector) return NULL;
    if(left_vector->length != right_vector->length || left_vector->vector_type != right_vector->vector_type) return NULL;
    result_vector = left_vector->vector_type == 'r'? vec_double_create_new_row(left_vector->length) : vec_double_create_new_column(left_vector->length);
    if(!result_vector) return NULL;
    for(e=0;e<left_vector->length;++e)
    {
        result_vector->data[e] = left_vector->data[e] - right_vector->data[e];
    }
    return result_vector;
}


int vec_double_to_csv(vec_double* vector, const char* file_name)
{
    FILE* f;
    index_t r, c;
    dimension_t nr, nc;
    if(!vector) return -4;
    nr = vector->vector_type == 'c' ? vector->length : 1;
    nc = vector->vector_type == 'r' ? vector->length : 1;
    f = fopen(file_name,"w");
    if(!f) return -2;
    for(r=0;r<nr;++r)
    {
        for(c=0;c<nc-1;++c)
        {
            fprintf(f,"%" "lf" ",", *(vector->data+r+c));
        }
        fprintf(f,"%" "lf" "\n", *(vector->data+r+c));
    }
    fclose(f);
    return r;
}


vec_double* vec_double_from_csv(const char* file_name)
{
    mat_double* matrix;
    vec_double* vector;
    dimension_t nr, nc;
    index_t r, c;
    if(!file_name) return NULL;
    matrix =  mat_double_from_csv(file_name);
    if(!matrix) return NULL;
    if(matrix->number_of_rows!=1 && matrix->number_of_columns != 1)
    {
        mat_double_destroy(matrix);
        return NULL;
    }
    vector =  matrix->number_of_rows == 1 ? vec_double_create_new_row(matrix->number_of_columns) : vec_double_create_new_column(matrix->number_of_rows);
    if(!vector) 
    {
        mat_double_destroy(matrix);
        return NULL;
    }
    nr = vector->vector_type == 'c' ? vector->length : 1;
    nc = vector->vector_type == 'r' ? vector->length : 1;
    for(r=0;r<nr;++r)
    {
        for(c=0;c<nc;++c)
        {
            *(vector->data+r+c) = matrix->data[r][c];
        }
    }
    return vector;
}

void vec_double_print(vec_double* vector, const char* title)
{
    index_t r;
    if(!vector)
    {
        printf("NULL\n");
        return;
    }
    printf("============= %s =============\n",title);
    printf("Length = %d , Type = %c\n", vector->length, vector->vector_type);
    for(r=0;r<vector->length;++r)
    {
        printf("%10" "lf" "", vector->data[r]);
    }
    printf("\n");
}

vec_double* mat_double_column_to_vector(mat_double* matrix, index_t column_index)
{
    index_t r;
    if(!matrix || column_index >= matrix->number_of_columns) return NULL;
    vec_double* vector = vec_double_create_new_column(matrix->number_of_rows);
    if(!vector) return NULL;
    for(r=0;r<matrix->number_of_rows;++r)
    {
        vector->data[r] = matrix->data[r][column_index];
    }
    return vector;
}


int mat_double_box_copy(mat_double* source_matrix, index_t source_row_start, index_t source_column_start, dimension_t number_of_rows, dimension_t number_of_columns, mat_double* destination_matrix, index_t destination_row_start, index_t destination_column_start )
{
    index_t rs,cs,rd,cd;
    if(!source_matrix || !destination_matrix || source_row_start > source_matrix->number_of_rows || source_row_start + number_of_rows > source_matrix->number_of_rows
        || source_column_start >= source_matrix->number_of_columns || source_column_start + number_of_columns > source_matrix->number_of_columns
        || destination_row_start >= destination_matrix->number_of_rows || destination_row_start + number_of_rows > destination_matrix->number_of_rows
        || destination_column_start >= destination_matrix->number_of_columns || destination_column_start + number_of_columns > destination_matrix->number_of_columns)
    {
        return -1;
    }
    for(rd = destination_row_start, rs = source_row_start;rd < destination_row_start + number_of_rows; ++rd, ++rs)
    {
        for(cd = destination_column_start, cs = source_column_start;cd < destination_column_start + number_of_columns; ++cd, ++cs)
        {
            destination_matrix->data[rd][cd] = source_matrix->data[rs][cs];
        }
    }
    return 0;
}


void mat_double_test()
{
    dimension_t nr,nc,nr1,nc1;
    dimension_t l;
    index_t r,c;
    char vt;
    mat_double *matrix1, *matrix2, *matrix3, *matrix4, *matrix5, *matrix6, *matrix7, *matrix8;
    double val;
    nr = 2;
    nc = 3;
    matrix1 = mat_double_create_new(nr,nc);
    val = 1;
    for(r=0;r<nr;++r)
    {
        for(c=0;c<nc;++c)
        {
            matrix1->data[r][c] = val++;
        }
    }
    mat_double_print(matrix1,"Matrix1");
    printf("]\n\ntesting mat_double_get_dimensions\n");
    mat_double_get_dimensions(matrix1,&nr1,&nc1);
    printf("expected row count = %" "d" " , expected column count = %" "d" "\n",nr,nc);
    printf("actual row count = %" "d" " , actual column count = %" "d" "\n",nr1,nc1);
    if(nr == nr1 && nc == nc1) printf("TEST PASSED\n");
    else printf("TEST FALIED\n");
    printf("\n\ntesting  mat_double_transpose\n");
    matrix2 = mat_double_transpose(matrix1);
    mat_double_print(matrix2, "Transpose of Matrix1");
    printf("\n\ntesting  mat_double_matrix_multiplication\n");
    matrix3 = mat_double_matrix_multiplication(matrix1,matrix2);
    mat_double_print(matrix3,"Matrix1 x Transpose of Matrix1");
    printf("\n\ntesting  mat_double_scalar_multiplication\n");
    val = 5;
    matrix4 = mat_double_scalar_multiplication(val,matrix1);
    mat_double_print(matrix4,"Matrix4 = 5 x Matrix1");
    printf("\n\ntesting  mat_double_matrix_addition\n");
    matrix5 = mat_double_matrix_addition(matrix1, matrix4);
    mat_double_print(matrix5,"Matrix5 = Matrix1 + Matrix4");
    printf("\n\ntesting  mat_double_matrix_substraction\n");
    matrix6 = mat_double_matrix_substraction(matrix5, matrix4);
    mat_double_print(matrix6,"Matrix6 = Matrix5 - Matrix4");
    printf("\n\ntesting  mat_double_to_csv\n");
    mat_double_to_csv(matrix5,"matrix5.csv");
    printf("\n\ntesting  mat_double_from_csv\n");
    matrix7 = mat_double_from_csv("matrix5.csv");
    mat_double_print(matrix7,"Matrix7 loaded from matrix5.csv");

    // vector tests

    vec_double *vector1, *vector2, *vector3, *vector4, *vector5, *vector6, *vector7, *vector8, *vector9;

    printf("\n\ntesting  vec_double_create_new_row\n");
    vector1 = vec_double_create_new_row_filled(3,2);
    vec_double_print(vector1,"Vector1");
    printf("\n\ntesting  vec_double_create_new_row\n");
    vector2 = vec_double_create_new_column_filled(3, 2);
    vec_double_print(vector2,"Vector2");
    printf("\n\ntesting  vec_double_set\n");
    vec_double_set(vector1, 1, 7);
    vec_double_print(vector1,"Updated Vector1");
    printf("\n\ntesting  vec_double_get\n");
    val = vec_double_get(vector1, 1);
    printf("Vector1[1] = %" "lf" "\n", val);
    printf("\n\ntesting  vec_double_get_length\n");
    vec_double_get_length(vector1, &l);
    printf("Length of vector1 = %d\n", l);
    printf("\n\ntesting  vec_double_get_vector_type\n");
    vec_double_get_vector_type(vector1, &vt);
    printf("type of vector1 = %c\n", vt);
    printf("\n\ntesting  vec_double_transpose\n");
    vector3 = vec_double_transpose(vector1);
    vec_double_print(vector3, "Vector3 = Transpose of Vector1");
    printf("\n\ntesting  vec_double_vector_multiplication\n");
    matrix8 =  vec_double_vector_multiplication(vector1, vector2);
    mat_double_print(matrix8, "Matrix8 = Vector1 * Vector2");
    printf("\n\ntesting  vec_double_matrix_vector_multiplication\n");
    vector4 = vec_double_matrix_vector_multiplication(matrix1, vector2);
    vec_double_print(vector4, "Vector4 = Metrix1 * Vector2");
    printf("\n\ntesting  vec_double_scalar_multiplication\n");
    vector5 = vec_double_scalar_multiplication(3, vector4);
    vec_double_print(vector5, "Vector5 = 3 * Vector4");
    printf("\n\ntesting  vec_double_vector_addition\n");
    vector6 = vec_double_vector_addition(vector4, vector5);
    vec_double_print(vector6, "Vector6 = Vector4 + Vector5");
    printf("\n\ntesting  vec_double_vector_substraction\n");
    vector7 = vec_double_vector_substraction(vector6, vector5);
    vec_double_print(vector7, "Vector7 = Vector6 - Vector5");
    printf("\n\ntesting vec_double_to_csv\n");
    vec_double_to_csv(vector7, "Vector7.csv");
    printf("\n\ntesting vec_double_from_csv\n");
    vector8 = vec_double_from_csv("Vector7.csv");
    vec_double_print(vector8,"Vector8 from csv Vector7.csv");

    // misc function testing

    printf("\n\ntesting mat_double_box_copy\n");
    mat_double_box_copy(matrix1,0,1,2,2,matrix4,0,0);
    mat_double_print(matrix4, "Box Copy from Matrix1(0,1,2,2) to Matrix4(0,0,2,2) ");
    printf("\n\ntesting mat_double_column_to_vector\n");
    vector9 = mat_double_column_to_vector(matrix4, 2);
    vec_double_print(vector9, "Vector9 = 3'rd column in Matrix4");


}

int main1()
{
    mat_double_test();
    return 0;
}
