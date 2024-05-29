#ifndef DIST_H
#define DIST_H

#include <mpi.h>
//#include "genetics.h"  // Assume this contains the definition of `Creature`
static inline void EXIT(int i){
	MPI_Finalize();
	exit(i);
}

//untested mess
// MPI Request for receiving messages
extern MPI_Request recv_request;
extern Creature recv_creature;



// Macro to process received Creature
#ifdef STACK_DNA

#define INITIATE_CREATURE_RECEPTION() \
    MPI_Irecv(&recv_creature, sizeof(Creature), MPI_BYTE, 0, CREATURE_TAG, MPI_COMM_WORLD, &recv_request)
#define PROCESS_RECEIVED_CREATURE() \
    (MPI_Test(&recv_request, &message_received, &status) && message_received)


#else
#define INITIATE_CREATURE_RECEPTION() \
    MPI_Irecv(&recv_size, 1, MPI_INT, 0, CREATURE_TAG, MPI_COMM_WORLD, &recv_request)

#define PROCESS_RECEIVED_CREATURE() \
    extern int recv_size;
    process_received_variable_creature()
    // Function used in the variable-size Creature case
	int 	() {
	    static int data_received = 0;
	    int message_received;
	    MPI_Status status;
	    
	    if (!data_received && MPI_Test(&recv_request, &message_received, &status) && message_received) {
	        // First part: size received, now receive the data
	        recv_creature.size = recv_size;
	        recv_creature.data = (double*) malloc(recv_size * sizeof(double));
	        MPI_Irecv(recv_creature.data, recv_size, MPI_DOUBLE, 0, CREATURE_TAG, MPI_COMM_WORLD, &recv_request);
	        data_received = 1;
	        return 0;
	    } else if (data_received && MPI_Test(&recv_request, &message_received, &status) && message_received) {
	        // Data part received
	        data_received = 0;  // Reset for next reception
	        return 1;
	    }
	    return 0;
	}
#endif



#endif //DIST_H