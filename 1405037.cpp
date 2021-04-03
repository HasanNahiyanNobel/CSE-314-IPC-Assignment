#include <iostream>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include "Semaphore.h"

#define PRODUCTION_QUEUE_SIZE 5
#define CHOCOLATEE_QUEUE_SIZE 5
#define VANILLAAAA_QUEUE_SIZE 5

using namespace std;

Semaphore mutex, empty, full;

void initializations();
void wait(Semaphore *S);
void signal(Semaphore *S);
void IPC_thread();

int main () {

	return 0;
}

void initializations () {
	mutex.value	= 1;
	empty.value	= PRODUCTION_QUEUE_SIZE;
	full.value	= 0;
}

void wait (Semaphore *S) {
	S->value--;
	if (S->value < 0) {

	}
}