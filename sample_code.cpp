#include<cstdio>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include <unistd.h>
#include <iostream>
#include <chrono>


/// Namespace
using namespace std;
using namespace chrono;


/// Constants
const int kProductionQueueSize = 5;
const int kNumberOfProductions = 10;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = 'v';
const int kSleepingTimeAfterEachConsoleOutput = 1; // In seconds
const int kApproximatedExecutionTimeLimit = 30; // In seconds, though execution will continue if a thread has called GetExecutionTime() before reaching that time limit. Numerically if there are n threads, execution seems to continue (n-1) seconds more.
auto kStartTime = high_resolution_clock::now();


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
int GetExecutionTime();


///Main Function
int main() {
	pthread_t thread_chef_x;
	pthread_t thread_chef_y;
	pthread_t thread_chef_z;

	InitSemaphore();

	pthread_create(&thread_chef_x, nullptr, StartChefX, nullptr);
	pthread_create(&thread_chef_y, nullptr, StartChefY, nullptr);
	pthread_create(&thread_chef_z, nullptr, StartChefZ, nullptr);

	pthread_join(thread_chef_x, nullptr);
	pthread_join(thread_chef_y, nullptr);
	pthread_join(thread_chef_z, nullptr);

	//while(1);

	cout << "Execution ended in: " << GetExecutionTime() << "s" << endl;

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
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		cout << "X1\n";
		sem_wait(&empty);
		cout << "X2\n";
		pthread_mutex_lock(&pthreadMutex);
		cout << "X3\n";

		sleep(kSleepingTimeAfterEachConsoleOutput);
		production_queue.push(kChocolateCakeChar);
		cout << "Chef X produced a cake; queue size: " << production_queue.size() << endl;
		//printf("Chef X produced item %d\n",i);

		cout << "X4\n";
		pthread_mutex_unlock(&pthreadMutex);
		cout << "X5\n";
		sem_post(&full);
		cout << "X6\n";
	}
	return nullptr;
}

void* StartChefY (void* pVoid) {
	int i;
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		cout << "Y1\n";
		sem_wait(&empty);
		cout << "Y2\n";
		pthread_mutex_lock(&pthreadMutex);
		cout << "Y3\n";

		sleep(kSleepingTimeAfterEachConsoleOutput);
		production_queue.push(kVanillaCakeChar);
		cout << "Chef Y produced a cake; queue size: " << production_queue.size() << endl;
		//printf("Chef Y produced item %d\n",i);

		cout << "Y4\n";
		pthread_mutex_unlock(&pthreadMutex);
		cout << "Y5\n";
		sem_post(&full);
		cout << "Y6\n";
	}
	return nullptr;
}

void* StartChefZ (void* pVoid) {
	int i;
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		cout << "Z1\n";
		sem_wait(&full);
		cout << "Z2\n";
		pthread_mutex_lock(&pthreadMutex);
		cout << "Z3\n";

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

		cout << "Z4\n";
		pthread_mutex_unlock(&pthreadMutex);
		cout << "Z5\n";
		sem_post(&empty);
		cout << "Z6\n";
	}
	return nullptr;
}

/**
 * @return Execution time in seconds
 */
int GetExecutionTime () {
	auto now = chrono::high_resolution_clock::now();
	auto duration = duration_cast<seconds>(now - kStartTime);
	return duration.count();
}