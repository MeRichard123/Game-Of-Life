set -xe 

#gcc main.c -o main -lraylib `pkg-config --libs --cflags raylib`
#./main


gcc ./64_bit_Conway.c -o 64_bit_Conway -lraylib `pkg-config --libs --cflags raylib`
./64_bit_Conway
