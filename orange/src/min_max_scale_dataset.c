#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <matrix.h>
#include <dataset.h>

int min_max_scale(const char *input_file_path, const char *output_file_path, index_t *columns_to_scale, int number_of_columns_to_scale, const char *info_file_path);

int main(int count, char **args)
{
    if (count != 5)
    {
        printf("Usage: ./min_max_scale_dataset <dataset_file_path> <output_file_path> <columns_to_scale[comma separated list]> <scaling_info_file>\n");
        printf("Example: ./min_max_scale_dataset dataset.csv output.csv \"1,2,3\" scaling_info.csv \n");
        return 0;
    }
    char *dataset_file_name, *output_file_name, *info_file_path;
    char ch;
    index_t i, j, k;
    dimension_t len;
    char str[513];
    index_t columns_to_scale[1024];
    dataset_file_name = args[1];
    output_file_name = args[2];
    info_file_path = args[4];
    len = strlen(args[3]);
    for (i = 0, j = 0, k = 0; i < len; ++i)
    {
        ch = args[3][i];
        if (ch == ' ')
            continue;
        if (ch == ',' || i == (len - 1))
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
            columns_to_scale[k++] = atoi(str);
        }
        else
            str[j++] = ch;
    }
    min_max_scale(dataset_file_name, output_file_name, columns_to_scale, k, info_file_path);
}

int min_max_scale(const char *input_file_path, const char *output_file_path, index_t *columns_to_scale, int number_of_columns_to_scale, const char *info_file_path)
{
    index_t i, r, c;
    dataset *data_set;
    dimension_t nr, nc;
    double val;
    double mins[number_of_columns_to_scale], maxs[number_of_columns_to_scale];
    if (!input_file_path || !strlen(input_file_path) || !output_file_path || !strlen(output_file_path) || number_of_columns_to_scale < 0)
    {
        printf("Unable to process arguments\n");
        return -1;
    }
    data_set = mat_double_from_csv(input_file_path);
    if (!data_set)
    {
        printf("Unable to load dataset file: %s\n", info_file_path);
        return -2;
    }
    mat_double_get_dimensions(data_set, &nr, &nc);
    for (i = 0; i < number_of_columns_to_scale; ++i)
    {
        c = columns_to_scale[i];
        if (c < 0 || c >= nc)
        {
            mat_double_destroy(data_set);
            return -3;
        }
        mins[i] = mat_double_get(data_set, 0, columns_to_scale[i]);
        maxs[i] = mat_double_get(data_set, 0, columns_to_scale[i]);
    }
    for (r = 1; r < nr; ++r)
    {
        for (i = 0; i < number_of_columns_to_scale; ++i)
        {
            c = columns_to_scale[i];
            val = mat_double_get(data_set, r, c);
            if (val < mins[i])
                mins[i] = val;
            if (val > maxs[i])
                maxs[i] = val;
        }
    }
    for (r = 0; r < nr; ++r)
    {
        for (i = 0; i < number_of_columns_to_scale; ++i)
        {
            c = columns_to_scale[i];
            val = mat_double_get(data_set, r, c);
            val = (val - mins[i]) / (maxs[i] - mins[i]);
            mat_double_set(data_set, r, c, val);
        }
    }
    mat_double_to_csv(data_set, output_file_path);
    return 0;
}