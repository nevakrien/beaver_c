#ifndef DIST_H
#define DIST_H

#include <mpi.h>
#include "genetics.h"
#include <stdio.h>
#include <stdlib.h>


#define CREATURE_TAG 0
#define TERMINATION_TAG 1

//untested mess
//globals
extern MPI_Request send_req;
extern MPI_Request recv_request;
extern Creature recv_creature;
extern MPI_Status status;

static void check_mpi_error(int errcode) {
    if (errcode != MPI_SUCCESS) {
        char error_string[BUFSIZ];
        int length_of_error_string, error_class;

        MPI_Error_class(errcode, &error_class);
        MPI_Error_string(error_class, error_string, &length_of_error_string);

        fprintf(stderr, "MPI Error: %s\n", error_string);
        MPI_Abort(MPI_COMM_WORLD, errcode); // Aborts MPI execution environment
        exit(1);
    }
}


static void EXIT(int i){
	if(i){
		MPI_Abort(MPI_COMM_WORLD, 1); // Aborts MPI execution environment
		exit(1);
	}	
	MPI_Finalize();
	exit(0);
}




// Macro to process received Creature

#ifdef STACK_DNA

// Function to initiate sending a Creature
static inline void send_creature(int target,const Creature* creature) {
    check_mpi_error(MPI_Isend((void*)creature, sizeof(Creature), MPI_BYTE, target, CREATURE_TAG, MPI_COMM_WORLD, &send_req));
}

static inline Creature receive_creature() {
	Creature ans;
    check_mpi_error(MPI_Recv(&ans, sizeof(Creature), MPI_BYTE, MPI_ANY_SOURCE, CREATURE_TAG, MPI_COMM_WORLD, &status));
    return ans;
}

#else
 
static inline int SerializeCreature(const Creature c,char** data){
	int size=SerializeDNA(c.dna,data);
	*data=realloc(*data,size+sizeof(rank_t));
	null_check(*data);
	*(rank_t *)(*data+size)=c.score;
	return size+sizeof(rank_t);

}
static inline Creature DeserializeCreature(char* data,int length){
	Creature ans;
	int size=length-sizeof(rank_t)
	ans.dna=DeserializeDNA(data,size);
	ans.score=*(rank_t *)(data+size);
	return ans
}


static inline Creature receive_creature() {
    int message_available = 0;
    int incoming_size = 0;

    check_mpi_error(MPI_Get_count(&status, MPI_BYTE, &incoming_size));

    // Allocate buffer based on the size found
    char* buffer = null_check((char*)malloc(incoming_size));

    check_mpi_error(MPI_Recv(buffer, incoming_size, MPI_BYTE, status.MPI_SOURCE, CREATURE_TAG, MPI_COMM_WORLD, &status));
    Creature ans = DeserializeCreature(buffer, incoming_size);
    free(buffer);
    return ans;
}

static inline void send_creature(int target,const Creature* creature) {
    char* buffer = NULL;
    int size = SerializeCreature(*creature, &buffer);
    if (size < 0) {
        fprintf(stderr, "Serialization failed.\n");
        free(buffer);
        return;
    }

    // Send the buffer containing the serialized creature
    check_mpi_error(MPI_Isend(buffer, size, MPI_BYTE, target, CREATURE_TAG, MPI_COMM_WORLD, &send_req));
    free(buffer);
}
#endif //STACK_DNA
#endif //DIST_H