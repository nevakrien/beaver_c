#ifndef TURING_H
#define TURING_H
#include"utils.h"
#include <string.h>

typedef enum {
	BIT_0 = 0,
	BIT_1 = 1
} Bit;

typedef enum {
	Stay=0,
	Right=1,
	Left=-1,
} MoveDir;

typedef struct {
	//2 options for each read result
	Bit write[2];    
	MoveDir move[2];     
	int nextState[2]; // -1 for halt
} State;

//#define MAX_STATES 100

static void print_state(State s) {
    printf("Write bits: [%d, %d]\n", s.write[0], s.write[1]);
    printf("Move directions: [%d, %d]\n", s.move[0], s.move[1]);
    printf("Next states: [%d, %d]\n", s.nextState[0], s.nextState[1]);
}

//infinitely extending zero padded vector
typedef struct {
	int length;
	Bit* data;
} ZVec;

typedef struct{
	ZVec pos;
	ZVec neg;
}Tape;

static void print_tape(Tape tape){
	printf("[");
	for(int i=tape.neg.length-1;i>=0;i--){
		printf("%d,",tape.neg.data[i]);
	}
	for(int i=0;i<tape.pos.length;i++){
		printf("%d,",tape.pos.data[i]);
	}

	printf("]\n");
}

//#define ERROR_TAPE (Tape){0}

static inline Bit readZVec(ZVec* v,int index){
	int len=v->length;

	if(index<len){
		return v->data[index];
	}

	
	Bit* data=realloc(v->data,8*len*sizeof(Bit));
	null_check(data);

	memset(data+len,0,7*len*sizeof(Bit));
	v->length=8*len;
	v->data=data;

	return BIT_0;
}

static inline void UnsafewriteZVec(ZVec* v,int index,Bit val){
	//printf("writing:%d\n",val);

	//since we already checked that we are of proper size on the read
	v->data[index]=val;
}

static inline Bit readTape(Tape* tape,int index){
	if(index<0){
		index=-index-1;
		return readZVec(&tape->neg,index);
	}
	return readZVec(&tape->pos,index);
}

static inline void UnsafewriteTape(Tape* tape,int index,Bit val){
	if(index<0){
		index=-index-1;
		if(index>=tape->neg.length) UNREACHABLE();
		return UnsafewriteZVec(&tape->neg,index,val);
	}
	if(index>=tape->pos.length) UNREACHABLE();
	return UnsafewriteZVec(&tape->pos,index,val);
}

static Tape initTape(){
	Tape tape;
	ZVec v;
	v.data=calloc(128,sizeof(Bit));
	null_check(v.data);
	v.length=128;
	tape.pos=v;

	v.data=calloc(128,sizeof(Bit));
	null_check(v.data);
	v.length=128;
	tape.neg=v;
	return tape;
}

static void freeTape(Tape tape){
	free(tape.pos.data);
	free(tape.neg.data);
}

static void resetTape(Tape tape){
	memset(tape.pos.data,0,tape.pos.length);
	memset(tape.neg.data,0,tape.neg.length);
}

static int runTuring(const State* states,Tape* tape,int maxSteps){
	State state=states[0];
	int loc=0;

	//Tape tape=initTape();

	for(int i=0;i!=maxSteps;i++){
		const Bit cur=readTape(tape,loc);
		//printf("read:%d\n",cur);

		UnsafewriteTape(tape,loc,state.write[cur]);
		loc+=state.move[cur];
		//printf("move:%d\n",state.move[cur]);

		int stateID=state.nextState[cur];
		//printf("stateID:%d\n",stateID);
		if(stateID<0) return 0;
		state=states[stateID];
		
		#ifdef MAX_STATES
			if(stateID>=MAX_STATES) UNREACHABLE();
		#endif //MAX_STATES

	}
	return 1;
}
#endif //TURING_H
