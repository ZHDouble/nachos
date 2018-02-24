// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include <vector>
using namespace std;
// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 1; num++) {
	printf("*** thread %d looped %d times threadId %d userId %d\n", which, num, currentThread->getThreadId(), currentThread->getUserId());
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    int i = 0;
    DEBUG('t', "Entering ThreadTest1");
    if (threadVecNum.size() == 0) {
        printf("the thread num has max limit!");
    }
    else if (threadVecNum.size() < 0) {
        printf("the thread num is error!");
    }
    else {
        for (i = 0; i < 3; i++) {
            Thread *t1 = new Thread("forked thread1");
            t1->Fork(SimpleThread, t1->getThreadId());
	    //Ts();
        }
    }
    //SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

