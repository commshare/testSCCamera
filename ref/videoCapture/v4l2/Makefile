CPPFLAGS := -fPIC
LDFLAGS := -L.

#CFLAGS=-O3 -g -Wall
CFLAGS=-O0 -g -Wall

#all: ringtest mttest python
all: capturetest
#python: libring.so ring.so

#ringtest:  test.o ring.o
#        gcc -g test.o ring.o -o ringtest

capturetest : capture_test.o capture.o
		gcc -g capture_test.o capture.o -o capture_test

libcapturev4l2.so: capture.o
		$(CC) -shared -o $@ $^ $(LDFLAGS)

#ring.so: pyring.o
#        $(CC) $(LDFLAGS) -shared -o $@ $^ -lboost_python -lring

#pyring.o: ring.h

capture.o:        capture.c capture.h


#mttest:         ring.o mttest.o
#        gcc -g mttest.o ring.o -lpthread -o mttest


#clean:
#        rm -f test.o ring.o

