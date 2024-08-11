#ifndef GENETICS_H
#define GENETICS_H

#include "user_gene.h"
#include "utils.h"
#define RNG() rand()//threadRNG()

typedef struct{
	rank_t score;
	DNA dna;
} Creature;

rank_t fitness(const DNA d);
DNA MakeRandomDNA();
DNA MakeKid(const DNA a,const DNA b);
DNA Mutate(const DNA d);
void OutputCreature(const Creature c);//this is used to give the best fit to user space. can be handled in any way

#ifndef STACK_DNA
void freeDNA(DNA d);
DNA CloneDNA(const DNA d);

int SerializeDNA(const DNA d,char** data); //try andalso alocate bytes for rank if you can
DNA DeserializeDNA(char* data,int length);
#else 
static inline void freeDNA(DNA d) {
    // No-op when using STACK_DNA
    (void)d; // To avoid unused parameter warning
}
static inline DNA CloneDNA(const DNA d){return d;}
#endif


static inline Creature cloneCreature(const Creature c){
	return (Creature){c.score,CloneDNA(c.dna)};
	//if its stack dna this CloneDNA is static 
	//so the entire thing becomes a simple assiment 
	//this was confirmed by checking the dissasmbly
}

#ifdef CUSTOM_COMPARE_CREATURES
int compareCreatures(const void *a, const void *b);
#else
static int compareCreatures(const void *a, const void *b) {
	rank_t A=((Creature*)a)->score;
	rank_t B=((Creature*)b)->score;

	if(A<=B){
		return 1; //decending
	}
	// if(A==B){
	// 	return 0;
	// }
	return -1;
}
#endif 


#endif //GENETICS_H