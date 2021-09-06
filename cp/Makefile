#
CC=gcc

loop: adxl355_loop.o adxl355.o
	gcc -o loop adxl355_loop.o adxl355.o
#adxl355_test.o : adxl355_test.c adxl355.h
#	gcc -c adxl355_test.c
adxl355_loop.o : adxl355_loop.c adxl355.h
	gcc -c adxl355_loop.c
adxl355.o : adxl355.c adxl355.h
	gcc -c adxl355.c
