#ifndef OPT_H
#define OPT_H

#include "genetics.h"

static inline Creature RandomCreature(){
	Creature ans;
	ans.dna=MakeRandomDNA();
	ans.score=fitness(ans.dna);
	return ans;
}

static inline Creature MutateCreature(const Creature c){
	Creature ans;
	ans.dna=Mutate(c.dna);
	ans.score=fitness(ans.dna);
	return ans;
}

#endif //OPT_H