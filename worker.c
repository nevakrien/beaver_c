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
Creature pop[pop_size];
int world_size;
int world_rank;
MPI_Request send_req=MPI_REQUEST_NULL;

#define CREATURE_TAG 0
#define TERMINATION_TAG 1

static inline void worker_handle_messages_and_sort() {
    MPI_Status status;
    Creature incoming_creature;
    int termination_signal = 0;
    int message_received;
    int index_to_replace = pop_size - 1;  // Start from the end of the population

    rank_t best_recived=MIN_RANKT;
    // Continuously check for new messages
    do {
        MPI_Request recv_req;
        MPI_Irecv(&incoming_creature, sizeof(Creature), MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_req);
        MPI_Test(&recv_req, &message_received, &status);

        if (message_received) {
            if (status.MPI_TAG == TERMINATION_TAG) {
                MPI_Recv(&termination_signal, 1, MPI_INT, 0, TERMINATION_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (termination_signal) {
                    EXIT(0);
                }
            } else if (status.MPI_TAG == CREATURE_TAG) {
                // Directly integrate the incoming creature into the population
                if (incoming_creature.score > pop[index_to_replace].score) {
                    pop[index_to_replace] = incoming_creature;
                }
                if(incoming_creature.score>best_recived){
                	best_recived=incoming_creature.score;
                }

                // Update the index in a wrap-around descending order
                index_to_replace--;
                if (index_to_replace < 0) {
                    index_to_replace = pop_size - 1;
                }
            }
        }
    } while (message_received);

    qsort(pop,pop_size,sizeof(Creature),compareCreatures);
    
    if(best_recived<pop[0].score){
	    // Send the best creature to node zero
	    if (send_req != MPI_REQUEST_NULL) {
	        MPI_Wait(&send_req, MPI_STATUS_IGNORE);
	    }
	    MPI_Isend(&pop[0], sizeof(Creature), MPI_BYTE, 0, CREATURE_TAG, MPI_COMM_WORLD, &send_req);
    }
}




// int worker_main(){
// 	SETRNG(123);
	

// 	for(int i=0;i<pop_size;i++){
// 		pop[i]=RandomCreature();
// 	}

// 	qsort(pop,pop_size,sizeof(Creature),compareCreatures);


// 	for(int iter=0;iter<num_iters;iter++){
// 		//new
// 		for(int i=half_pop_size+quarter_pop_size;i<pop_size;i++){
// 			pop[i]=RandomCreature();
// 		}

// 		//children
// 		for (int i = half_pop_size; i < pop_size-quarter_pop_size; i++) {
//             int parent1 = RNG() % quarter_pop_size;  // Select parent1 from the top quarter
//             int parent2 = RNG() % quarter_pop_size;  // Select parent2 from the top quarter
//             pop[i] = MakeChild(pop[parent1], pop[parent2]);
//         }

//         //small mutations
// 		for(int i=quarter_pop_size;i<half_pop_size;i++){
// 			pop[i]=MutateCreature(pop[i]);
// 		}
// 		qsort(pop,pop_size,sizeof(Creature),compareCreatures);
// 		if(best_score<pop[0].score){
// 			best=cloneCreature(pop[0]);
// 		}
// 	}

// 	OutputCreature(best);

// 	return 0;
// }

//very rough sketch
int main(int argc, char** argv){
	MPI_Init(&argc,&argv);
	// Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    worker_handle_messages_and_sort();

    EXIT(0);
}