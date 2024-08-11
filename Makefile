# Makefile to compile user_gene.c into user_gene.o
# and compile and link test_turing.c into an executable with debugging information

# Compiler to use
CC=zig cc#gcc-14#zig cc#cproc#tcc#clang#gcc

#
OPT_FLAG=-O2 #-march=native #-static -pthread#-flto
# Base Compiler flags for object files
CFLAGS=-c $(OPT_FLAG) -Wall

# Compiler flags for executables, includes debugging
LDFLAGS=$(OPT_FLAG)

# Default target
all: single_threaded test_turing 

# Compile user_gene.c to user_gene.o
user_gene.o: user_gene.c
	$(CC) $(CFLAGS) user_gene.c -o user_gene.o


# Compile and link test_turing.c into an executable
test_turing: test_turing.c user_gene.o 
	$(CC) $(LDFLAGS) test_turing.c user_gene.o  -o test_turing 

single_threaded:  user_gene.o 
	$(CC) $(LDFLAGS) ga.c user_gene.o  -o single_threaded 

# Clean the build
clean:
	rm -f user_gene.o  test_turing single_threaded

check: clean 
	make all
	#valgrind ./test_turing #acts weird with static links]
	./test_turing