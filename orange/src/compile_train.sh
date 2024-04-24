gcc -I ../include -c generic_matrix.c
gcc -I ../include/ generic_matrix.o -c mat_double_matrix.c -Wall
gcc -I ../include *.o train.c -o train.out
