#ifndef USER_GENE_H
#define USER_GENE_H

#include"turing.h"
#include <limits.h>

#define MAX_STATES 10

typedef int rank_t;
#define MIN_RANKT INT_MIN

typedef struct{
	State data[MAX_STATES];
} DNA;

#define STACK_DNA

#endif //USER_GENE_H