CC=gcc
CFLAGS=-Wall -ggdb -O2
LDFLAGS=

.PHONY : all test clean

all : mallocfail.so

mallocfail.o : mallocfail.c
	$(CC) -c $(CFLAGS) -fPIC -o $@ $<

memory_funcs.o : memory_funcs.c
	$(CC) -c $(CFLAGS) -fPIC -o $@ $<


mallocfail.so : mallocfail.o memory_funcs.o
	$(CC) -shared -o $@ $^ ${LDFLAGS} -fPIC -ldl


test : mf_test mallocfail.so
	LD_PRELOAD=./mallocfail.so ./mf_test

clean :
	-rm -f *.o mallocfail.so mallocfail_hashes.txt


