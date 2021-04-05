#include<cstdio>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include <unistd.h>
#include <iostream>


/// Namespace
using namespace std;


/// Constants
const int kProductionQueueSize = 5;
const int kNumberOfProductions = 10;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = 'v';
const int kSleepingTimeAfterEachConsoleOutput = 1; // In seconds
const int kExecutionTimeLimit = 2000; // In milliseconds, though execution will continue if a thread has called GetExecutionTime() before reaching that time limit.


/// Global variables
sem_t empty;
sem_t full;
pthread_mutex_t pthreadMutex;
queue<char> production_queue;


/// Function definitions
void InitSemaphore();
void* StartChefX(void* pVoid);
void* StartChefY(void* pVoid);
void* StartChefZ(void* pVoid);


///Main Function
int main() {
	pthread_t thread_chef_x;
	pthread_t thread_chef_y;
	pthread_t thread_chef_z;

	InitSemaphore();

	pthread_create(&thread_chef_x, nullptr, StartChefX, nullptr);
	pthread_create(&thread_chef_y, nullptr, StartChefY, nullptr);
	pthread_create(&thread_chef_z, nullptr, StartChefZ, nullptr);

	while(1);
	return 0;
}


///Function Declarations
void InitSemaphore () {
	sem_init(&empty, 0, kProductionQueueSize);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&pthreadMutex, nullptr);
}

void* StartChefX (void* pVoid) {
	int i;
	for(i=0; i<kNumberOfProductions; i++) {
		sem_wait(&empty);
		pthread_mutex_lock(&pthreadMutex);

		sleep(kSleepingTimeAfterEachConsoleOutput);
		production_queue.push(kChocolateCakeChar);
		cout << "Chef X produced a cake; queue size: " << production_queue.size() << endl;
		//printf("Chef X produced item %d\n",i);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&full);
	}
	return nullptr;
}

void* StartChefY (void* pVoid) {
	int i;
	for(i=0; i<kNumberOfProductions; i++) {
		sem_wait(&empty);
		pthread_mutex_lock(&pthreadMutex);

		sleep(kSleepingTimeAfterEachConsoleOutput);
		production_queue.push(kVanillaCakeChar);
		cout << "Chef Y produced a cake; queue size: " << production_queue.size() << endl;
		//printf("Chef Y produced item %d\n",i);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&full);
	}
	return nullptr;
}

void* StartChefZ (void* pVoid) {
	int i;
	for(i=0; i<kNumberOfProductions; i++) {
		sem_wait(&full);
		pthread_mutex_lock(&pthreadMutex);

		sleep(kSleepingTimeAfterEachConsoleOutput);
		char cake = production_queue.front();
		production_queue.pop();
		if (cake==kChocolateCakeChar) {
			cout << "Chef Z took a *chocolate* cake; queue size: " << production_queue.size() << endl;
		}
		else {
			cout << "Chef Z took a *vanilla* cake; queue size: " << production_queue.size() << endl;
		}
		//printf("Chef Z consumed cake %d\n", cake);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&empty);
	}
	return nullptr;
}