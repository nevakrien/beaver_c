#include "utils.h"
#include "genetics.h"

const int quarter_pop_size=25;
const int half_pop_size=2*quarter_pop_size;
const int pop_size=2*half_pop_size;//must be even
const int num_iters=1000;

int main(){
	DNA pop[pop_size];
	rank_t scores[pop_size];
	int topIndexs[half_pop_size];

	for(int i=0;i<pop_size;i++){
		pop[i]=MakeRandomDNA();
		scores[i]=fitness(pop[i]);
	}

	for(int iter=0;iter<num_iters;iter++){
		getTopk(scores,pop_size,half_pop_size,topIndexs);

		DNA pop[half_pop_size] temp;
		for(int i=0;i<half_pop_size;i++){
			temp[i]=CloneDNA(pop[topIndexs[i]]);//probably a mistake if we actually have heap stuff...
		}
		for(int i=0;i<pop_size;i++){
			FreeDNA(pop[0]);//probably a mistake if we actually have heap stuff...
		}


	}



	return 0;
}