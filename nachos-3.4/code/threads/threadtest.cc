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
#include "synch.h"
#include <vector>
#include <unistd.h>
using namespace std;
// testnum is set in main.cc
int testnum = 1;
int t = 0;
Thread *t1 = new Thread("forked thread1");
Thread *t2 = new Thread("forked thread2");
Thread *t3 = new Thread("forked thread3");
Lock * mylock = new Lock("my lock");
Condition * mycond = new Condition("my condition");

#define MAX_VALUE 10
int value = 0;
Lock * pc_lock = new Lock("pc lock");
Condition * pc_cond = new Condition("pc condition");
Semaphore * pc_sem_empty = new Semaphore("pc_sem_empty", 10);
Semaphore * pc_sem_full = new Semaphore("pc_sem_full", 0);
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
    
    for (num = 0; num < 10; num++) {
	printf("*** thread %d looped %d times threadId %d userId %d\n", which, num, currentThread->getThreadId(), currentThread->getUserId());
        currentThread->Yield();
    }
}

void
SimpleThreadLock1(int pvalue)
{
    printf("simplethreadlock1 testlock == %d\n", testlock);
    mylock->Acquire();          // 加锁
    int AX = testlock;
    int BX = 20;
    currentThread->Yield();
    testlock = AX - BX;
    mylock->Release();           // 释放
    printf("simplethreadlock1 t == %d\n", testlock);
}

void
SimpleThreadLock2(int pvalue)
{
    printf("simplethreadlock2 testlock == %d\n", testlock);
    mylock->Acquire();
    int AX = testlock;
    int BX = 30;
    currentThread->Yield();
    testlock = AX - BX;
    mylock->Release();
    printf("simplethreadlock2 t == %d\n", testlock);
}

void 
SimpleThreadLock3(int v)
{
    printf("wait st4\n");
    mylock->Acquire();
    mycond->Wait(mylock);
    printf("recv st4 signal\n");
    mylock->Release();
}
void 
SimpleThreadLock4(int v)
{
    mylock->Acquire();
    mycond->Signal(mylock);
    printf("send st4 signal\n");
    mylock->Release();
}

void 
SimpleThreadProduce(int v)
{
	int i = 0;
	for (i = 0; i < 20; i++) {
    	pc_lock->Acquire();
		if (value >= MAX_VALUE) {
			pc_cond->Wait(pc_lock);
		}
		value += 1;
		printf("produce value ==== %d\n", value);
		pc_cond->Signal(pc_lock);
		pc_lock->Release();
		if (i % 2 == 0) {
    			currentThread->Yield();
		}
	}
}

void
SimpleThreadConsume(int v)
{
	int i = 0;
	for (i = 0; i < 20; i++) {
		pc_lock->Acquire();
		if (value == 0) {
			pc_cond->Wait(pc_lock);
		}
		value -= 1;
		printf("consume value ==== %d\n", value);
		pc_cond->Signal(pc_lock);
		pc_lock->Release();
    		currentThread->Yield();
	}
}

void
SimpleThreadSemProduce(int v)
{
	int i = 0;
	for (i = 0; i < 20; i++) {
		pc_sem_empty->P();
		pc_lock->Acquire();
		value += 1;
		printf("sem product value ==== %d\n", value);
		pc_lock->Release();
		pc_sem_full->V();
		if (i % 3 == 0)
    		currentThread->Yield();
	}
}

void
SimpleThreadSemConsume(int v)
{
	int i = 0;
	for (i = 0; i < 20; i++) {
		pc_sem_full->P();
		pc_lock->Acquire();
		value -= 1;
		printf("sem consume value ==== %d\n", value);
		pc_lock->Release();
		pc_sem_empty->V();
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
//-----------------------------------------------------
//        Thread *t1 = new Thread("forked thread1");
//        t1->setPriority(4);
//        t1->Fork(SimpleThread, t1->getThreadId());
//        Thread *t2 = new Thread("forked thread2");
//        t2->setPriority(1);
//        t2->Fork(SimpleThread, t2->getThreadId());
//        Thread *t3 = new Thread("forked thread3");
//        t3->setPriority(3);
//        t3->Fork(SimpleThread, t3->getThreadId());
	    //Ts();

//-----------------------------------------------------
	/*
        for (i = 0; i < 3; i++) {
            Thread *t1 = new Thread("forked thread1");
            t1->Fork(SimpleThread, t1->getThreadId());
	    //Ts();
        }
	*/
        //t1->Fork(SimpleThreadProduce, 1);
        //t1->setPriority(1);
        //t2->Fork(SimpleThreadConsume, 2);
        //t2->setPriority(2);
        t1->Fork(SimpleThreadSemProduce, 1);
        t1->setPriority(1);
        t2->Fork(SimpleThreadSemConsume, 2);
        t2->setPriority(2);
//	for (int k = 0; k < 1000; k++)
//	{
//	    usleep(10000);
//	}
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

