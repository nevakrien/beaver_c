#include "utils.h"
#include "genetics.h"
#include "opt.h"

const int quarter_pop_size=25;
const int half_pop_size=2*quarter_pop_size;
const int pop_size=2*half_pop_size;//must be even
const int num_iters=1000;

int main(){
	Creature best;//we could actually just save it allways in pop[0]...
	Creature pop[pop_size];

	#pragma omp parallel for
	for(int i=0;i<pop_size;i++){
		pop[i]=RandomCreature();
	}

	qsort(pop,pop_size,sizeof(Creature),compareCreatures);
	best=cloneCreature(pop[0]);

	for(int iter=0;iter<num_iters;iter++){
		//new
		#pragma omp parallel for
		for(int i=half_pop_size+quarter_pop_size;i<pop_size;i++){
			pop[i]=RandomCreature();
		}

		//children
		#pragma omp parallel for
		for (int i = half_pop_size; i < pop_size-quarter_pop_size; i++) {
            int parent1 = RNG() % quarter_pop_size;  // Select parent1 from the top quarter
            int parent2 = RNG() % quarter_pop_size;  // Select parent2 from the top quarter
            pop[i] = MakeChild(pop[parent1], pop[parent2]);
        }

        //small mutations
        #pragma omp parallel for
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