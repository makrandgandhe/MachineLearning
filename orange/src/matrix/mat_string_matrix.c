#include <matrix.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// matrix

typedef struct __mat_string
{
    dimension_t number_of_rows;
    dimension_t number_of_columns;
    char*** data;
} mat_string;

mat_string *mat_string_create_new(dimension_t number_of_rows, dimension_t number_of_columns)
{
    mat_string* matrix;
    char*** data;
    index_t i,j;
    if(number_of_rows <= 0 || number_of_columns <= 0) return NULL;
    data = (char***) malloc(sizeof(char**) * number_of_rows);
    if(!data) return NULL;
    for(i=0;i<number_of_columns;++i)
    {
        data[i] = (char**) malloc(sizeof(char*) * number_of_columns);
        if(!data[i])
        {
            for(j=0;j<i;++j)
            {
                free(data[j]);
            }
            free(data);
            return NULL;
        }
        for(j=0;j<number_of_columns;++j)
        {
            data[j] = NULL;
        }
    }
    matrix = (mat_string*)malloc(sizeof(mat_string));
    if(!matrix)
    {
        for(i=0;i<number_of_rows;++i)
        {
            free(data[i]);
        }
        free(data);
        return NULL;
    }
    matrix->data = data;
    matrix->number_of_rows = number_of_rows;
    matrix->number_of_columns = number_of_columns;
    return matrix;
}

// mat_string *mat_string_create_new_filled(dimension_t number_of_rows, dimension_t number_of_columns, double fill_value);
// void mat_string_destroy(mat_string *matrix);
// void mat_string_set(mat_string *matrix, index_t row_index, index_t column_index, double value);
// double mat_string_get(mat_string *matrix, index_t row_index, index_t column_index);
// void mat_string_get_dimensions(mat_string *matrix, dimension_t *number_of_rows, dimension_t *number_of_columns);
// mat_string *mat_string_transpose(mat_string *matrix);
// int mat_string_to_csv(mat_string *matrix, const char *file_name);
// mat_string *mat_string_from_csv(const char *file_name);
// void mat_string_print(mat_string *matrix, const char *title);

// vector



// set

typedef struct __set_string
{
    char **data;
    dimension_t size;
    dimension_t capacity;

} set_string;

set_string *set_string_create_new()
{
    index_t i = 0;
    set_string *set = (set_string *)malloc(sizeof(set_string));
    if (!set)
        return NULL;
    set->capacity = SET_STRING_DEFAULT_CAPACITY;
    set->size = 0;
    set->data = (char **)malloc(sizeof(char *) * set->capacity);
    if (!set->data)
    {
        free(set);
        return NULL;
    }
    for (i = 0; i < set->capacity; ++i)
    {
        set->data[i] = NULL;
    }
    return set;
}

void set_string_destroy(set_string *set)
{
    if (set)
    {
        if (set->data)
        {
            for (int i = 0; i < set->size; ++i)
            {
                if (set->data[i])
                    free(set->data[i]);
            }
            free(set->data);
        }
        free(set);
    }
}

dimension_t set_string_get_size(set_string *set)
{
    if (!set)
        return -1;
    return set->size;
}

dimension_t set_string_get_capacity(set_string *set)
{
    if (!set)
        return -1;
    return set->capacity;
}

dimension_t set_string_add(set_string *set, const char *string)
{
    dimension_t new_capacity;
    char **new_data;
    if (!set || !string)
        return -1;
    char *string_copy;
    if (!set_string_contains(set, string))
    {
        string_copy = malloc(sizeof(char) * (strlen(string) + 1));
        if (!string_copy)
            return -2;
        strcpy(string_copy, string);
        if (!set->data)
        {
            free(string_copy);
            return -1;
        }
        if (set->size == set->capacity)
        {
            printf("Expanding Capacity\n");
            new_capacity = set->capacity + SET_STRING_DEFAULT_SIZE_INCREMENT;
            new_data = realloc(set->data, sizeof(char *) * new_capacity);
            if (!new_data)
            {
                free(string_copy);
                return -3;
            }
            set->capacity = new_capacity;
            set->data = new_data;
        }
        set->data[set->size++] = string_copy;
    }
    return set->size;
}

int set_string_contains(set_string *set, const char *string)
{
    index_t i;
    if (!set || !set->data || !string)
        return MAT_FALSE;
    for (i = 0; i < set->size; ++i)
    {
        if (strcmp(set->data[i], string) == 0)
            return MAT_TRUE;
    }
    return MAT_FALSE;
}

int set_string_for_each(set_string *set, void (*func)(char *, index_t, void *), void *ptr)
{
    index_t i;
    char *string, *new_string;
    if (!set || !set->data || !func)
        return MAT_FALSE;
    for (i = 0; i < set->size; ++i)
    {
        string = set->data[i];
        new_string = (char *)malloc(sizeof(char) * (strlen(string) + 1));
        if (!new_string)
            return MAT_FALSE;
        strcpy(new_string, string);
        func(new_string, i, ptr);
    }
    return MAT_TRUE;
}

void set_string_print(set_string *set, const char *title)
{
    index_t i;
    printf("============= %s =============\n", title);
    printf("Size [%d]\n", set->size);
    for (i = 0; i < set->size; ++i)
    {
        if (i == 0)
        {
            printf("{\n");
        }
        printf("%s\n", set->data[i]);
        if (i == set->size - 1)
        {
            printf("}\n");
        }
    }
}

// void print(const char* string)
// {
//     printf("%s\n",string);
// }

// int main()
// {
//     set_string* set = set_string_create_new();
//     set_string_add(set, "Hello");
//     set_string_add(set, "Hello");
//     set_string_add(set, "World");
//     set_string_add(set, "World");
//     set_string_add(set, "Hello World");
//     set_string_add(set, "Hello");
//     set_string_add(set, "Class");
//     set_string_add(set, "Hello World");
//     index_t size;
//     dimension_t capacity;
//     size = set_string_get_size(set);
//     capacity = set_string_get_capacity(set);
//     printf("Size: %d\n",size);
//     printf("Capacity: %d\n",capacity);
//     // set_string_for_each(set, print);
//     set_string_print(set,"Set");
//     printf("Set contains 'World': %d\n",set_string_contains(set, "World"));
//     printf("Set contains 'Mack': %d\n",set_string_contains(set, "Mack"));
//     return 0;
// }