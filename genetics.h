#ifndef GENETICS_H
#define GENETICS_H

#include "user_gene.h"

rank_t fitness(const DNA d);
DNA MakeRandomDNA();
DNA MakeKid(const DNA a,const DNA b);
DNA Mutate(const DNA d);

#ifndef STACK_DNA
void freeDNA(DNA d);
DNA CloneDNA(const DNA d);
#else 
static inline void freeDNA(DNA d) {
    // No-op when using STACK_DNA
    (void)d; // To avoid unused parameter warning
}
static inline DNA CloneDNA(const DNA d){return d;};
#endif

void OutputDNA(const DNA d);//this is used to give the best fit to user space. can be handled in any way

#endif //GENETICS_H