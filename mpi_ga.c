#include "utils.h"
#include "genetics.h"
#include "opt.h"
#include "dist.h"
#include <mpi.h>
#include <stdlib.h>  // For dynamic allocation
#include <stdio.h>   // For exit


#define quarter_pop_size 25
#define  half_pop_size 2*quarter_pop_size
#define pop_size 2*half_pop_size//must be even
#define  num_iters 1000

// Global variables
Creature pop[pop_size]={0};//test;
int world_size;
int world_rank;
MPI_Request send_req=MPI_REQUEST_NULL;
MPI_Request recv_req=MPI_REQUEST_NULL;
MPI_Status status;



static inline void network_rank_creatures(){
	int flag=0;
	Creature incoming_creature;
	//rank_t best_recived=MIN_RANKT;

	int i=pop_size-1; //candidate

	check_mpi_error(MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD,&flag, &status));
	while(flag){
		if (status.MPI_TAG == TERMINATION_TAG){
			EXIT(0);
		}
		Creature incoming=receive_creature();
		if(pop[i].score<incoming.score){
			pop[i]=incoming;
			freeDNA(pop[i].dna);
			i--;
			if(i<0){
				i=pop_size-1;
			}
			//only matters if bigger than our max
			//best_recived=max(incoming.score,best_recived);

		}
		else{ freeDNA(incoming.dna);}

		check_mpi_error(MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD,&flag, &status));
	}

	qsort(pop,pop_size,sizeof(Creature),compareCreatures);

	//if(best_recived<pop[0].score){
	    // // Send the best creature to node zero
	    // if (send_req != MPI_REQUEST_NULL) {
	    //     check_mpi_error(MPI_Wait(&send_req, MPI_STATUS_IGNORE));
	    // }
	    send_creature(0,&pop[0]);
    //}
}


//manager is ID=0
void manager_main(){
	Creature best={0};
	best.score=MIN_RANKT;

	for(int i=0;i<num_iters;i++){
		check_mpi_error(MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status));
		int source = status.MPI_SOURCE;
		
		Creature in=receive_creature();
		if(in.score>best.score){
			best=in;
		}
		//send
		for(int i=1;i<world_size;i++){
			if(i==source){
				continue;
			}
			send_creature(i,&in);
		}
	}

	//kill them all
	for(int i=1;i<world_size;i++){
		MPI_Isend(NULL, 0, MPI_BYTE, i, TERMINATION_TAG, MPI_COMM_WORLD, &send_req);
	}

	OutputCreature(best);
	EXIT(0);

}

//very rough sketch
int main(int argc, char** argv){
	MPI_Init(&argc,&argv);
	// Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    printf("my id:%d\n",world_rank);
    if(world_rank==0){
    	printf("world size:%d\n",world_size);
    	manager_main();
    }
    worker_main();
}

int worker_main(){
	SETRNG(123+world_rank);
	

	for(int i=0;i<pop_size;i++){
		pop[i]=RandomCreature();
	}

	network_rank_creatures();


	while(1){
		//new
		for(int i=half_pop_size+quarter_pop_size;i<pop_size;i++){
			pop[i]=RandomCreature();
		}

		//children
		for (int i = half_pop_size; i < pop_size-quarter_pop_size; i++) {
            int parent1 = RNG() % quarter_pop_size;  // Select parent1 from the top quarter
            int parent2 = RNG() % quarter_pop_size;  // Select parent2 from the top quarter
            pop[i] = MakeChild(pop[parent1], pop[parent2]);
        }

        //small mutations
		for(int i=quarter_pop_size;i<half_pop_size;i++){
			pop[i]=MutateCreature(pop[i]);
		}
		qsort(pop,pop_size,sizeof(Creature),compareCreatures);
		network_rank_creatures();
	}
	return 0;
}