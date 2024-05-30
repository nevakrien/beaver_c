# beaver_c
this code is an attempt at a GA libarary (and example usage) that can be used by
provifing an implementation to the functions and data types in genetics.h

types:
DNA
rank_t

funcs:
```c
rank_t fitness(DNA d);
DNA MakeRandomDNA();
DNA MakeKid(DNA a,DNA b);
DNA Mutate(DNA d);
void freeDNA(DNA d); //if DNA is on the heap
DNA CloneDNA(DNA d)  //if DNA is on the heap
```

these should be provided in user_gene.h and implemented in user_gene.c
for DNA thats a stack object simply #define STACK_DNA

GA and potentially mpi and slurm 

# test
valgrind --suppressions=mpi-suppressions.supp mpirun -np 4 ./mpi