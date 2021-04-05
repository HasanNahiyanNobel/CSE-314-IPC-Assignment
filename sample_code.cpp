#include<cstdio>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include <unistd.h>


/// Namespace
using namespace std;


/// Constants
const int kProductionQueueSize = 5;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = '-'; //TODO Change this to 'v'
const int kSleepingTimeAfterEachConsoleOutput = 1; // In seconds
const int kExecutionTimeLimit = 2000; // In milliseconds, though execution will continue if a thread has called GetExecutionTime() before reaching that time limit.


/// Global variables
sem_t empty;
sem_t full;
pthread_mutex_t pthreadMutex;
queue<int> production_queue;


/// Function definitions
void InitSemaphore();
void* StartChefX(void* pVoid);
void* StartChefZ(void* pVoid);


///Main Function
int main() {
	pthread_t thread1;
	pthread_t thread2;

	InitSemaphore();

	pthread_create(&thread1, nullptr, StartChefX, nullptr);
	pthread_create(&thread2, nullptr, StartChefZ, nullptr);

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
	for(i=1;i<=10;i++) {
		sem_wait(&empty);
		pthread_mutex_lock(&pthreadMutex);

		sleep(kSleepingTimeAfterEachConsoleOutput);
		production_queue.push(i);
		printf("Chef X produced item %d\n",i);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&full);
	}
	return nullptr;
}

void* StartChefZ (void* pVoid) {
	int i;
	for(i=1;i<=10;i++) {
		sem_wait(&full);
		pthread_mutex_lock(&pthreadMutex);

		sleep(kSleepingTimeAfterEachConsoleOutput);
		int item = production_queue.front();
		production_queue.pop();
		printf("Chef Z consumed item %d\n",item);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&empty);
	}
	return nullptr;
}