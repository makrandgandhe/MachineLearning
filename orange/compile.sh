gcc -I include -c src/matrix/generic_matrix.c -Wall
gcc -I include -c src/matrix/mat_double_matrix.c -Wall
gcc -I include -c src/matrix/mat_double_math.c -Wall
gcc -I include -c src/matrix/mat_string_matrix.c -Wall

ar rcs lib/libmatrix.a *.o
rm -f *.o

gcc -I include -c src/dataset_prep/dataset_prep.c -Wall
ar rcs lib/libdataset.a *.o
rm -f *.o


gcc -I include src/train.c  -Llib -lmatrix -Llib -ldataset  -o train -Wall
gcc -I include src/prepare_test_train_dataset.c  -Llib -lmatrix -Llib -ldataset  -o prepare_test_train_dataset -Wall
gcc -I include src/test_accuracy.c -Llib -lmatrix -Llib -ldataset  -o test_accuracy -Wall
gcc -I include test/matrix/test_mat_double_math.c -Llib -lmatrix -lm -o test_matrix -Wall
