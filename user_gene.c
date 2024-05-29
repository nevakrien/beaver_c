#include "utils.h"
#include "genetics.h"
#include "turing.h" //order matters we defined MAX_STATES
#include <stdlib.h>


//srand(123);
static const int MAX_STEPS=10000;

static Bit randomBit() {
    return (Bit)(RNG() % 2); 
}

static MoveDir randomDir() {
    int r = RNG() % 3;
    switch (r) {
        case 0: return Stay;
        case 1: return Right;
        case 2: return Left;
    }
    return Stay;
}

static int randomNext() {
    return -1 + (RNG() % (MAX_STATES + 1)); 
}

rank_t fitness(const DNA d){
	Tape tape=initTape();
	if(runTuring(d.data,&tape,MAX_STEPS)){
		freeTape(tape);
		return -1;
	}

	inline int findSum(ZVec v){
		int score=0;
		for(int i=0;i<v.length;i++){
			score+=v.data[i];
		}
		return score;
	}
	rank_t ans= findSum(tape.pos)+findSum(tape.neg);
	freeTape(tape);
	return ans;
}

DNA MakeRandomDNA(){
	DNA ans;
	for(int i=0;i<MAX_STATES;i++){
		for(int j=0;j<2;j++){
			ans.data[i].write[j]=randomBit();
			ans.data[i].move[j]=randomDir();
			ans.data[i].nextState[j]=randomNext();
		}
		
	}
	return ans;
}
DNA MakeKid(const DNA a,const DNA b){
	DNA ans;
	for(int i=0;i<MAX_STATES;i++){
		if(RNG() % 2){
			ans.data[i]=a.data[i];
		}
		else{
			ans.data[i]=b.data[i];
		}
	}
	return ans;
}
DNA Mutate(const DNA d){
	DNA ans=d;
	int i=RNG()%MAX_STATES;
	int j=RNG()%6;
	switch(j){
		case 0:ans.data[i].write[0]= 1-ans.data[i].write[0];
		case 1:ans.data[i].write[1]= 1-ans.data[i].write[1];
		case 2:ans.data[i].move[0]=randomDir();
		case 3:ans.data[i].move[1]=randomDir();
		case 4:ans.data[i].move[0]=randomNext();
		case 5:ans.data[i].move[1]=randomNext();
	}

	return ans;
}

void OutputCreature(const Creature c){
	printf("best fit:\nscore:%d\n\n",c.score);
	for(int i=0;i<MAX_STATES;i++){
		print_state(c.dna.data[i]);
		printf("\n");
	}
	printf("tape:\n");
	Tape tape=initTape();
	runTuring(c.dna.data,&tape,MAX_STEPS);
	print_tape(tape);
	freeTape(tape);
}