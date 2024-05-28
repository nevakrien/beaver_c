# Makefile to compile user_gene.c into user_gene.o
# and compile and link test_turing.c into an executable with debugging information

# Compiler to use
CC=gcc
CXX=g++-14

# Base Compiler flags for object files
CFLAGS=-c -g2 -Wall
CXXFLAGS=-c -g2 -static -Wall #libalgos.a

# Static library creation flags
AR=ar
ARFLAGS=rcs

# Compiler flags for executables, includes debugging
LDFLAGS=-g2

# Default target
all: user_gene.o single_threaded test_turing

# Compile user_gene.c to user_gene.o
user_gene.o: user_gene.c
	$(CC) $(CFLAGS) user_gene.c -o user_gene.o

# Compile algos.cpp to algos.o and create libalgos.a
libalgos.a: algos.o
	$(AR) $(ARFLAGS) libalgos.a algos.o

algos.o: algos.cpp
	$(CXX) $(CXXFLAGS) -o algos.o algos.cpp

# Compile and link test_turing.c into an executable
test_turing: test_turing.c user_gene.o 
	$(CC) $(LDFLAGS) test_turing.c user_gene.o  -o test_turing 

single_threaded: libalgos.a user_gene.o 
	$(CC) $(LDFLAGS) single_threaded.c user_gene.o  -o single_threaded 
# Clean the build
clean:
	rm -f user_gene.o algos.o libalgos.a test_turing single_threaded 
