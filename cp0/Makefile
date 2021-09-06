#
CC=gcc

.PHONY: all
all: test1 loop redis

test1: adxl355_test.o adxl355.o
	$(CC) -o test1 adxl355_test.o adxl355.o -l bcm2835

loop: adxl355_loop.o adxl355.o
	$(CC) -o loop adxl355_loop.o adxl355.o -l bcm2835

redis: adxl355_redis0.o adxl355.o
	$(CC) -o redis adxl355_redis0.o adxl355.o  -l pthread -l bcm2835 -l hiredis -L/usr/local/lib

adxl355_test.o: adxl355_test.c adxl355.h
	$(CC) -c adxl355_test.c

adxl355_redis0.o: adxl355_redis0.c adxl355.h
	$(CC) -c adxl355_redis0.c -I/usr/local/include/hiredis

adxl355_loop.o: adxl355_loop.c adxl355.h
	$(CC) -c adxl355_loop.c

adxl355.o: adxl355.c adxl355.h
	$(CC) -c adxl355.c

.PHONY: clean
clean:
	rm -f loop test1 redis *.o
