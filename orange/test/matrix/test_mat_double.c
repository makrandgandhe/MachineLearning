#include<matrix.h>

void mat_double_test1()
{
    dimension_t nr, nc, nr1, nc1;
    dimension_t l;
    index_t r, c;
    char vt;
    mat_double *matrix1, *matrix2, *matrix3, *matrix4, *matrix5, *matrix6, *matrix7, *matrix8, *matrix9, *matrix10;
    double val;
    nr = 2;
    nc = 3;
    matrix1 = mat_double_create_new(nr, nc);
    val = 1;
    for (r = 0; r < nr; ++r)
    {
        for (c = 0; c < nc; ++c)
        {
        mat_double_set(matrix1, r, c, val++);
        }
    }
    mat_double_print(matrix1, "Matrix1");
    printf("]\n\ntesting mat_double_get_dimensions\n");
    mat_double_get_dimensions(matrix1, &nr1, &nc1);
    printf("expected row count = %"
           "d"
           " , expected column count = %"
           "d"
           "\n",
           nr, nc);
    printf("actual row count = %"
           "d"
           " , actual column count = %"
           "d"
           "\n",
           nr1, nc1);
    if (nr == nr1 && nc == nc1)
        printf("TEST PASSED\n");
    else
        printf("TEST FALIED\n");
    printf("\n\ntesting  mat_double_transpose\n");
    matrix2 = mat_double_transpose(matrix1, NULL);
    mat_double_print(matrix2, "Transpose of Matrix1");
    printf("\n\ntesting  mat_double_matrix_multiplication\n");
    matrix3 = mat_double_matrix_multiplication(matrix1, matrix2, NULL);
    mat_double_print(matrix3, "Matrix1 x Transpose of Matrix1");
    printf("\n\ntesting  mat_double_scalar_multiplication\n");
    val = 5;
    matrix4 = mat_double_scalar_multiplication(val, matrix1, NULL);
    mat_double_print(matrix4, "Matrix4 = 5 x Matrix1");
    printf("\n\ntesting  mat_double_matrix_addition\n");
    matrix5 = mat_double_matrix_addition(matrix1, matrix4, NULL);
    mat_double_print(matrix5, "Matrix5 = Matrix1 + Matrix4");
    printf("\n\ntesting  mat_double_matrix_substraction\n");
    matrix6 = mat_double_matrix_substraction(matrix5, matrix4, NULL);
    mat_double_print(matrix6, "Matrix6 = Matrix5 - Matrix4");
    printf("\n\ntesting  mat_double_to_csv\n");
    mat_double_to_csv(matrix5, "matrix5.csv");
    printf("\n\ntesting  mat_double_from_csv\n");
    matrix7 = mat_double_from_csv("matrix5.csv");
    mat_double_print(matrix7, "Matrix7 loaded from matrix5.csv");

    printf("\n\ntesting  mat_double_create_new_random_filled\n");
    matrix10 = mat_double_create_new_random_filled(3, 5, 0.0, 1.0);
    mat_double_print(matrix10, "Matrix10 has random double values between 0 and 1");

    // vector tests

    vec_double *vector1, *vector2, *vector3, *vector4, *vector5, *vector6, *vector7, *vector8, *vector9, *vector10, *vector11, *vector12, *vector13, *vector14, *vector15;

    printf("\n\ntesting  vec_double_create_new_row\n");
    vector1 = vec_double_create_new_row_filled(3, 2);
    vec_double_print(vector1, "Vector1");
    printf("\n\ntesting  vec_double_create_new_row\n");
    vector2 = vec_double_create_new_column_filled(3, 2);
    vec_double_print(vector2, "Vector2");
    printf("\n\ntesting  vec_double_set\n");
    vec_double_set(vector1, 1, 7);
    vec_double_print(vector1, "Updated Vector1");
    printf("\n\ntesting  vec_double_get\n");
    val = vec_double_get(vector1, 1);
    printf("Vector1[1] = %"
           "lf"
           "\n",
           val);
    printf("\n\ntesting  vec_double_get_length\n");
    vec_double_get_length(vector1, &l);
    printf("Length of vector1 = %d\n", l);
    printf("\n\ntesting  vec_double_get_vector_type\n");
    vec_double_get_vector_type(vector1, &vt);
    printf("type of vector1 = %c\n", vt);
    printf("\n\ntesting  vec_double_transpose\n");
    vector3 = vec_double_transpose(vector1, NULL);
    vec_double_print(vector3, "Vector3 = Transpose of Vector1");
    printf("\n\ntesting  vec_double_vector_multiplication\n");
    vector12 = vec_double_create_new_row(4);
    for (int uu = 1; uu <= 3; ++uu)
    {
        vec_double_set(vector2, uu - 1, uu);
        vec_double_set(vector12, uu - 1, uu + 4);
    }
    vec_double_set(vector12, 3, 8);
    vec_double_print(vector2, "Vector2");
    vec_double_print(vector12, "Vector12");
    matrix8 = vec_double_vector_multiplication(vector2, vector12, NULL);
    mat_double_print(matrix8, "Matrix8 = Vector2 * Vector12");
    printf("\n\ntesting  vec_double_matrix_vector_multiplication\n");
    vector4 = vec_double_matrix_vector_multiplication(matrix1, vector2, NULL);
    mat_double_print(matrix1, "Matrix1");
    vec_double_print(vector2, "Vector2");
    vec_double_print(vector4, "Vector4 = Metrix1 * Vector2");

    matrix9 = mat_double_create_new(5, 2);
    for (int pp = 0, jj = 1; pp < 5; ++pp)
    {
        for (int ff = 0; ff < 2; ++ff, ++jj)
        {
            mat_double_set(matrix9, pp, ff, jj * 11);
        }
    }

    vector10 = vec_double_create_new_column(2);
    vec_double_set(vector10, 0, 6);
    vec_double_set(vector10, 1, 9);

    mat_double_print(matrix9, "Matrix9");
    vec_double_print(vector10, "Vector10");
    vector11 = vec_double_matrix_vector_multiplication(matrix9, vector10, NULL);

    vec_double_print(vector11, "Vector11 = Matrix9 * Vector10");

    printf("\n\ntesting  vec_double_scalar_multiplication\n");
    vector5 = vec_double_scalar_multiplication(3, vector4, NULL);
    vec_double_print(vector5, "Vector5 = 3 * Vector4");
    printf("\n\ntesting  vec_double_vector_addition\n");
    vector6 = vec_double_vector_addition(vector4, vector5, NULL);
    vec_double_print(vector6, "Vector6 = Vector4 + Vector5");
    printf("\n\ntesting  vec_double_vector_substraction\n");
    vector7 = vec_double_vector_substraction(vector6, vector5, NULL);
    vec_double_print(vector7, "Vector7 = Vector6 - Vector5");
    printf("\n\ntesting vec_double_to_csv\n");
    vec_double_to_csv(vector7, "Vector7.csv");
    printf("\n\ntesting vec_double_from_csv\n");
    vector8 = vec_double_from_csv("Vector7.csv");
    vec_double_print(vector8, "Vector8 from csv Vector7.csv");

    // misc function testing

    printf("\n\ntesting mat_double_box_copy\n");
    mat_double_box_copy(matrix1, 0, 1, 2, 2, matrix4, 0, 0);
    mat_double_print(matrix4, "Box Copy from Matrix1(0,1,2,2) to Matrix4(0,0,2,2) ");
    printf("\n\ntesting mat_double_column_to_vector\n");
    vector9 = mat_double_column_to_vector(matrix4, 2);
    vec_double_print(vector9, "Vector9 = 3'rd column in Matrix4");

    printf("\n\nmat_double_row_to_vector\n");
    vector13 = vec_double_create_new_row(3);
    vector14 = mat_double_row_to_vector(matrix4, 1, vector13);
    vec_double_print(vector14, "Vector14 = Matrix4[1]");

    printf("\n\nvec_double_copy\n");
    vector15 = vec_double_create_new_row(4);
    vec_double_set(vector15, 0, 1.0);
    vec_double_copy(vector14, vector15, 0, 1, 3);
    vec_double_print(vector15, "vec_double_copy(vector14, vector15, 0, 1, 3)");
}

int main()
{
    mat_double_test1();
}