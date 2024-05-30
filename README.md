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
int SerializeDNA(const DNA d,char** data);  //if DNA is on the heap AND the build is an mpi build
DNA DeserializeDNA(char* data,int length);  //if DNA is on the heap AND the build is an mpi build
```

these should be provided in user_gene.h and implemented in user_gene.c
for DNA thats a stack object simply #define STACK_DNA

GA and potentially mpi and slurm 

# test
valgrind --suppressions=mpi-suppressions.supp mpirun -np 4 ./mpi

# results
inital tests showed that runing with only 4/6 processes was signifctly better (speed wise) than 24. 
this is because killing everything takes time. there is probably a good way to just kill all children I need to figure it out


since I set it as a constant number of runs using more processes means pools are less mature. 
it may be worth while to change this in the future