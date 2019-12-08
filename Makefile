CC = gcc -std=gnu99
CFLAGS = -g -msse4 -O0 -pipe -fno-omit-frame-pointer -Wpsabi

all:	bench-naive bench-optimize

#naive
bench-naive: benchmark.o dgemm-naive.o
	$(CC) -o $@ benchmark.o dgemm-naive.o 

#optimize
bench-optimize: benchmark.o dgemm-optimize.o
	$(CC) -o $@ benchmark.o dgemm-optimize.o

%.o: %.c
	$(CC) -c $(CFLAGS) $<


.PHONY: clean log run
clean:
	rm -f *~ bench-naive bench-optimize *.o log

log:
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" >> log
	./bench-optimize >> log
	less log

run:
	./bench-optimize > test
