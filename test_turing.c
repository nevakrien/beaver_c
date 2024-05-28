#include "turing.h"
#include <assert.h>
#include <stdlib.h>

void test1() {
    //in
    State states[] = {
        {{BIT_1, BIT_1}, {Right, Stay}, {1, 1}},
        {{BIT_0,BIT_1}, {Left, Stay}, {-1, 0}}
    };
    //reading 0 writing 1 right step going 1
    //reading 0 writing 0 haulting
    Tape tape = initTape();
    if(runTuring(states,&tape, 10)){
        perror("too long\n");
        exit(1);
    }
    assert(readTape(&tape, 0) == BIT_1);
    printf("test1 passed\n");
    freeTape(tape);
}

void test2() {
    State states[] = {{{BIT_0, BIT_1}, {Stay, Right}, {-1, -1}}};
    Tape tape = initTape();
    readTape(&tape,100);//actually alocate the mmemory
    tape.pos.data[0] = BIT_1;
    tape.pos.data[1] = BIT_0;
    if(runTuring(states,&tape, 10)){
        perror("too long\n");
        exit(1);
    }
    assert(readTape(&tape, 1) == BIT_0);
    assert(readTape(&tape, 2) == BIT_0);
    assert(readTape(&tape, 3) == BIT_0);
    printf("test2 passed\n");
    freeTape(tape);

}

void test3() {
    State states[] = {{{BIT_1, BIT_0}, {Right, Right}, {-1, 0}}};
    Tape tape = initTape();
    readTape(&tape,100);//actually alocate the mmemory

    for (int i = 0; i < 4; i++) {
        tape.pos.data[i] = BIT_1;
    }
    if(runTuring(states,&tape, 10)){
        perror("too long\n");
        exit(1);
    }
    assert(readTape(&tape, 4) == BIT_1);
    printf("test3 passed\n");
    freeTape(tape);

}

void test4() {
    State states[] = {{{BIT_1, BIT_0}, {Right, Right}, {0, 0}}};
    Tape tape = initTape();

    if(!runTuring(states,&tape, 10)){
        perror("didnt error on no hault\n");
        exit(1);
    }
    printf("test4 passed\n");
    freeTape(tape);

}

int main() {
    printf("very basic tests\n");
    //printf("\ntest1\n\n");
    test1();
    
    //printf("\ntest2\n\n");
    test2();

    //printf("\ntest3\n\n");
    test3();

    //printf("\ntest4\n\n");
    test4();
    return 0;
}


