gcc -I ../include -c generic_matrix.c
gcc -I ../include/ generic_matrix.o mat_int8_matrix.c -o mat_int8_matrix.out -Wall
gcc -I ../include/ generic_matrix.o mat_double_matrix.c -o mat_double_matrix.out -Wall
