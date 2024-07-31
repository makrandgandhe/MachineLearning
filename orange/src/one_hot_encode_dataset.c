#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <matrix.h>

int one_hot_encode(const char *input_file_path, const char *output_file_path, index_t *string_columns, int number_of_string_columns, int file_header_present, const char *info_file_path);
void generate_new_column_values(char *str, index_t i, void *ptr);
void save_encoding_info(const char *file_path, index_t *string_columns, int number_of_string_columns, set_string **sets);
void write_column_val_to_file(char *str, index_t i, void *ptr);

int main(int count, char **args)
{
    if (count != 6)
    {
        printf("Usage: ./one_hot_encode_dataset <dataset_file_path> <desired_encoded_file_path> <string_columns[comma separated list]> <has_header[Y/N]> <encoding_info_file>\n");
        printf("Example: ./one_hot_encode_dataset ./dataset.csv ./encoded_dataset.csv \"1 , 2, 3\" Y encoding_info.csv\n");
        return 0;
    }
    char *dataset_file_name, *output_file_name, *info_file_path;
    char str[513];
    index_t string_columns[1024];
    int i, j, k, len;
    char ch;
    int has_header;
    dataset_file_name = args[1];
    output_file_name = args[2];
    info_file_path = args[5];
    len = strlen(args[3]);
    for (i = 0, j = 0, k = 0; i < len; ++i)
    {
        ch = args[3][i];
        if (ch == ' ')
            continue;
        if (ch == ',' || i == len - 1)
        {
            if (i == len - 1)
            {
                str[j++] = ch;
            }
            str[j] = '\0';
            j = 0;
            if (strlen(str) == 0)
            {
                printf("Usage: ./one_hot_encode_dataset <dataset_file_path> <desired_encoded_file_path> <string_columns[comma separated list]> <has_header[Y/N]> <encoding_info_file>\n");
                printf("Example: ./one_hot_encode_dataset ./dataset.csv ./encoded_dataset.csv \"1 , 2, 3\" Y encoding_info.csv\n");
                return 0;
            }
            string_columns[k++] = atoi(str);
        }
        else
            str[j++] = ch;
    }
    has_header = strcmp(args[4], "Y") == 0 || strcmp(args[4], "y") == 0 ? 1 : 0;
    one_hot_encode(dataset_file_name, output_file_name, string_columns, k, has_header, info_file_path);
    return 0;
}

int one_hot_encode(const char *input_file_path, const char *output_file_path, index_t *string_columns, int number_of_string_columns, int file_header_present, const char *info_file_path)
{
    FILE *input_file, *output_file;
    index_t i, j, k, c, e;
    dimension_t number_of_columns_in_input_file;
    int is_current_column_of_string_type, s;
    long data_start_pivot;

    char string1[1024];
    char string2[1024];
    char *ptrs[2];

    char ch;

    set_string **unique_string_sets;

    if (!input_file_path || !strlen(input_file_path) || !output_file_path || !strlen(output_file_path) || number_of_string_columns < 0)
    {
        printf("Unable to process arguments\n");
        return -1;
    }
    input_file = fopen(input_file_path, "r");
    if (!input_file)
    {
        printf("Unable to open input file: %s \n", input_file_path);
        return -2;
    }

    // Counting columns in input file
    number_of_columns_in_input_file = 0;
    while (1)
    {
        ch = fgetc(input_file);
        if (feof(input_file) || ch == '\n')
        {
            ++number_of_columns_in_input_file;
            break;
        }
        if (ch == ',')
            ++number_of_columns_in_input_file;
    }
    if (!file_header_present)
        fseek(input_file, 0, 0);
    data_start_pivot = ftell(input_file);
    for (i = 0; i < number_of_string_columns; ++i)
    {
        if (string_columns[i] >= number_of_columns_in_input_file)
        {
            printf("Invalud column index[%d] provided, NOTE: %d columns found in input file %s\n", string_columns[i], number_of_columns_in_input_file, input_file_path);
            fclose(input_file);
            return -3;
        }
    }
    // Counting columns in input file complete

    printf("Number of columns: %d\n", number_of_columns_in_input_file);
    printf("Number of string columns: %d\n", number_of_string_columns);

    // Scanning file to get unique strings from string columns

    unique_string_sets = (set_string **)malloc(sizeof(set_string *) * number_of_string_columns);
    if (!unique_string_sets)
    {
        fclose(input_file);
        return -4;
    }
    for (i = 0; i < number_of_string_columns; ++i)
    {
        unique_string_sets[i] = set_string_create_new();
        if (!unique_string_sets[i])
        {
            for (j = 0; j < i; ++j)
            {
                set_string_destroy(unique_string_sets[j]);
            }
            free(unique_string_sets);
            return -5;
        }
    }
    i = 0;
    j = 0;
    k = 0;
    c = 0;
    while (!feof(input_file))
    {
        if (i == 0)
        {
            is_current_column_of_string_type = 0;
            for (k = 0; k < number_of_string_columns; ++k)
            {
                if (c == string_columns[k])
                    is_current_column_of_string_type = 1;
            }
        }
        ch = fgetc(input_file);
        if (feof(input_file))
        {
            ch = '\n';
        }
        if (ch == '\r')
            continue;
        if (ch == ',' || ch == '\n')
        {
            string1[i] = '\0';
            if (is_current_column_of_string_type)
            {
                if (strlen(string1) > 0)
                {
                    s = set_string_add(unique_string_sets[j], string1);
                    if (s < 0)
                    {
                        for (e = 0; e < number_of_string_columns; ++e)
                        {
                            set_string_destroy(unique_string_sets[e]);
                        }
                        free(unique_string_sets);
                        fclose(input_file);
                        return -6;
                    }
                }
                if (j == number_of_string_columns - 1)
                    j = 0;
                else
                    j++;
            }
            i = 0;
            ++c;
            if (ch == '\n')
            {
                c = 0;
            }
        }
        else if (is_current_column_of_string_type)
        {
            string1[i++] = ch;
        }
    }

    // Scanning file to get unique strings from string columns completes

    fseek(input_file, data_start_pivot, 0);

    // Encoding and Writing output file
    output_file = fopen(output_file_path, "w");
    if (!output_file)
    {
        fclose(input_file);
        for (e = 0; e < number_of_string_columns; ++e)
        {
            set_string_destroy(unique_string_sets[e]);
        }
        free(unique_string_sets);
        printf("Unable to open output file: %s\n", output_file_path);
        return -7;
    }
    i = 0;
    j = 0;
    k = 0;
    c = 0;
    while (!feof(input_file))
    {
        if (i == 0)
        {
            is_current_column_of_string_type = 0;
            for (k = 0; k < number_of_string_columns; ++k)
            {
                if (c == string_columns[k])
                    is_current_column_of_string_type = 1;
            }
        }
        ch = fgetc(input_file);
        if (feof(input_file))
        {
            ch = '\n';
        }
        if (ch == '\r')
            continue;
        if (ch == ',' || ch == '\n')
        {
            string1[i] = '\0';
            string2[0] = '\0';
            if (is_current_column_of_string_type)
            {
                ptrs[0] = string1;
                ptrs[1] = string2;
                set_string_for_each(unique_string_sets[j], generate_new_column_values, (void *)ptrs);
                string2[strlen(string2) - 1] = '\0';
                fputs(string2, output_file);
                if (j == number_of_string_columns - 1)
                    j = 0;
                else
                    j++;
            }
            else
            {
                fputs(string1, output_file);
            }
            i = 0;
            ++c;
            fputc(ch, output_file);
            if (ch == '\n')
            {
                c = 0;
            }
        }
        else
        {
            string1[i++] = ch;
        }
    }
    // Encoding and Writing output file completes
    save_encoding_info(info_file_path, string_columns, number_of_string_columns, unique_string_sets);
    // cleanup
    fclose(input_file);
    fclose(output_file);

    for (e = 0; e < number_of_string_columns; ++e)
    {
        set_string_destroy(unique_string_sets[e]);
    }
    free(unique_string_sets);
    // cleanup completes
    return 0;
}

void generate_new_column_values(char *str, index_t i, void *ptr)
{
    if (!str || !ptr)
    {
        printf("ERROR: NULL POINTER");
        return;
    }
    char **ptrs = (char **)ptr;
    char *old_column_value;
    char *new_column_values_string;
    char *p;
    old_column_value = (char *)ptrs[0];
    new_column_values_string = (char *)ptrs[1];
    p = new_column_values_string + strlen(new_column_values_string);
    if (strlen(old_column_value))
    {
        if (strcmp(old_column_value, str) == 0)
        {
            strcpy(p, "1,");
        }
        else
        {
            strcpy(p, "0,");
        }
    }
    free(str);
}

void save_encoding_info(const char *file_path, index_t *string_columns, int number_of_string_columns, set_string **sets)
{
    if (!file_path || !string_columns || !sets)
    {
        printf("NULL Pointer: Unable to save encoding info\n");
        return;
    }
    FILE *file;
    index_t i;
    char column_number[11];
    file = fopen(file_path, "w");

    if (!file)
    {
        printf("Unable to open / create file: %s\n", file_path);
        return;
    }
    for (i = 0; i < number_of_string_columns; ++i)
    {
        if (!sets[i])
        {
            fclose(file);
            printf("NULL Pointer: Unable to save encoding info\n");
            return;
        }
        sprintf(column_number, "%d,", string_columns[i]);
        fputs(column_number, file);
        set_string_for_each(sets[i], write_column_val_to_file, (void *)file);
        fseek(file,ftell(file) -1 ,0);
        fputc('\n', file);
    }
    fclose(file);
}

void write_column_val_to_file(char *str, index_t i, void *ptr)
{
    FILE *file;
    file = (FILE *)ptr;
    if (file && str)
    {
        fputs(str, file);
        fputc(',', file);
    }
}