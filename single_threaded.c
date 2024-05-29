#include "utils.h"
#include "genetics.h"
#include "opt.h"

const int quarter_pop_size=25;
const int half_pop_size=2*quarter_pop_size;
const int pop_size=2*half_pop_size;//must be even
const int num_iters=1000;

int main(){
	srand(123);
	Creature best;
	Creature pop[pop_size];

	for(int i=0;i<pop_size;i++){
		pop[i]=RandomCreature();
	}

	qsort(pop,pop_size,sizeof(Creature),compareCreatures);
	best=cloneCreature(pop[0]);//depending on how we choose to do things clone could be droped here

	for(int iter=0;iter<num_iters;iter++){
		for(int i=half_pop_size;i<pop_size;i++){
			pop[i]=RandomCreature();
		}
		for(int i=quarter_pop_size;i<half_pop_size;i++){
			pop[i]=MutateCreature(pop[i]);
		}
		qsort(pop,pop_size,sizeof(Creature),compareCreatures);
		if(best.score<pop[0].score){
			best=cloneCreature(pop[0]);
		}
	}

	OutputCreature(best);

	return 0;
}