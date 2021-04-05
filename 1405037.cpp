#include <iostream>
#include <chrono>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


/// Namespace
using namespace std;
using namespace chrono;


/// Constants
const int kProductionQueueSize = 5;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = 'v';
const string kProductionQueueName = "Queue 1";
const int kSleepingTimeAfterEachConsoleOutput = 1; // In seconds
const int kApproximatedExecutionTimeLimit = 30; // In seconds, though execution will continue if a thread has called GetExecutionTime() before reaching that time limit. Numerically if there are n threads, execution seems to continue (n-1) seconds more.
auto kStartTime = high_resolution_clock::now();


/// Global variables
sem_t empty;
sem_t full;
pthread_mutex_t pthreadMutex;
queue<char> production_queue;
queue<char> chocolate_queue;
queue<char> vanilla_queue;


/// Function definitions
void InitSemaphore();
void* StartChefX(void* pVoid);
void* StartChefY(void* pVoid);
void* StartChefZ(void* pVoid);
void PrintQueue(queue<char> the_queue, const string& name, int max_size);
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
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		sem_wait(&empty);
		pthread_mutex_lock(&pthreadMutex);

		production_queue.push(kChocolateCakeChar);

		cout << "Chef X produced a *chocolate* cake; queue size: " << production_queue.size() << endl;
		PrintQueue(production_queue, kProductionQueueName, kProductionQueueSize);

		sleep(kSleepingTimeAfterEachConsoleOutput);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&full);
	}
	return nullptr;
}

void* StartChefY (void* pVoid) {
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		sem_wait(&empty);
		pthread_mutex_lock(&pthreadMutex);

		production_queue.push(kVanillaCakeChar);

		cout << "Chef Y produced a *vanilla* cake; queue size: " << production_queue.size() << endl;
		PrintQueue(production_queue, kProductionQueueName, kProductionQueueSize);

		sleep(kSleepingTimeAfterEachConsoleOutput);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&full);
	}
	return nullptr;
}

void* StartChefZ (void* pVoid) {
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		sem_wait(&full);
		pthread_mutex_lock(&pthreadMutex);

		char cake = production_queue.front();
		production_queue.pop();

		string cake_name;
		if (cake==kChocolateCakeChar) cake_name = "chocolate";
		else cake_name = "vanilla";
		cout << "Chef Z took a *"<< cake_name <<"* cake; queue size: " << production_queue.size() << endl;
		PrintQueue(production_queue, kProductionQueueName, kProductionQueueSize);

		sleep(kSleepingTimeAfterEachConsoleOutput);

		pthread_mutex_unlock(&pthreadMutex);
		sem_post(&empty);
	}
	return nullptr;
}

void PrintQueue (queue<char> the_queue, const string& name, int max_size) {
	if (!name.empty()) {
		cout << name << ": ";
	}
	for (int i=0; i<max_size; i++) {
		if (the_queue.empty()) {
			cout << "- ";
		}
		else {
			cout << the_queue.front() << " ";
			the_queue.pop();
		}
	}
	cout << endl << endl;
}

/**
 * @return Execution time in seconds
 */
int GetExecutionTime () {
	auto now = chrono::high_resolution_clock::now();
	auto duration = duration_cast<seconds>(now - kStartTime);
	return duration.count();
}
