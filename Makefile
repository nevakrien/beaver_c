# Makefile to compile user_gene.c into user_gene.o
# and compile and link test_turing.c into an executable with debugging information

# Compiler to use
CC=gcc

#
OPT_FLAG=-O3 -g -flto
# Base Compiler flags for object files
CFLAGS=-c $(OPT_FLAG) -Wall

# Static library creation flags
AR=ar
ARFLAGS=rcs

# Compiler flags for executables, includes debugging
LDFLAGS=$(OPT_FLAG)

# Default target
all: single_threaded parallel_for test_turing

# Compile user_gene.c to user_gene.o
user_gene.o: user_gene.c
	$(CC) $(CFLAGS) user_gene.c -o user_gene.o


# Compile and link test_turing.c into an executable
test_turing: test_turing.c user_gene.o 
	$(CC) $(LDFLAGS) test_turing.c user_gene.o  -o test_turing 

single_threaded:  user_gene.o 
	$(CC) $(LDFLAGS) ga.c user_gene.o  -o single_threaded 

# parallel_for: user_gene.o
# 	$(CC) $(LDFLAGS) ga.c user_gene.o -fopenmp -o parallel_for 

# Clean the build
clean:
	rm -f user_gene.o  test_turing ga 
	#parallel_for
