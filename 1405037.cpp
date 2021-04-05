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
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = 'v';
const int kProductionQueueSize = 5;
const int kChocolateQueueSize = 5;
const int kVanillaQueueSize = 5;
const string kProductionQueueName = "Queue 1";
const string kChocolateQueueName = "Queue 3";
const string kVanillaQueueName = "Queue 2";
const int kSleepingTimeAfterEachConsoleOutput = 1; // In seconds
const int kApproximatedExecutionTimeLimit = 30; // In seconds, though execution will continue if a thread has called GetExecutionTime() before reaching that time limit. Numerically if there are n threads, execution seems to continue (n-1) seconds more.
auto kStartTime = high_resolution_clock::now();


/// Global variables
sem_t empty_production_queue;
sem_t full_production_queue;
pthread_mutex_t mutex_print;
pthread_mutex_t mutex_production_queue;
sem_t empty_chocolate_queue;
sem_t full_chocolate_queue;
pthread_mutex_t mutex_chocolate_queue;
sem_t empty_vanilla_queue;
sem_t full_vanilla_queue;
pthread_mutex_t mutex_vanilla_queue;
queue<char> production_queue;
queue<char> chocolate_queue;
queue<char> vanilla_queue;


/// Function definitions
void InitSemaphore();
void* StartChefX(void* pVoid);
void* StartChefY(void* pVoid);
void* StartChefZ(void* pVoid);
void PrintQueue(queue<char> the_queue, const string& name, int max_size);
void PrintAllQueues();
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
	pthread_mutex_init(&mutex_print, nullptr);

	sem_init(&empty_production_queue, 0, kProductionQueueSize);
	sem_init(&full_production_queue, 0, 0);
	pthread_mutex_init(&mutex_production_queue, nullptr);

	sem_init(&empty_chocolate_queue, 0, kChocolateQueueSize);
	sem_init(&full_chocolate_queue, 0, 0);
	pthread_mutex_init(&mutex_chocolate_queue, nullptr);

	sem_init(&empty_vanilla_queue, 0, kVanillaQueueSize);
	sem_init(&full_vanilla_queue, 0, 0);
	pthread_mutex_init(&mutex_vanilla_queue, nullptr);
}

void* StartChefX (void* pVoid) {
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		sem_wait(&empty_production_queue);
		pthread_mutex_lock(&mutex_production_queue);

		production_queue.push(kChocolateCakeChar);

		pthread_mutex_lock(&mutex_print);
		cout << "Chef X produced a *chocolate* cake." << endl;
		PrintAllQueues();
		pthread_mutex_unlock(&mutex_print);

		sleep(kSleepingTimeAfterEachConsoleOutput);

		pthread_mutex_unlock(&mutex_production_queue);
		sem_post(&full_production_queue);
	}
	return nullptr;
}

void* StartChefY (void* pVoid) {
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		sem_wait(&empty_production_queue);
		pthread_mutex_lock(&mutex_production_queue);

		production_queue.push(kVanillaCakeChar);

		pthread_mutex_lock(&mutex_print);
		cout << "Chef Y produced a *vanilla* cake." << endl;
		PrintAllQueues();
		pthread_mutex_unlock(&mutex_print);

		sleep(kSleepingTimeAfterEachConsoleOutput);

		pthread_mutex_unlock(&mutex_production_queue);
		sem_post(&full_production_queue);
	}
	return nullptr;
}

void* StartChefZ (void* pVoid) {
	for(; GetExecutionTime() < kApproximatedExecutionTimeLimit;) {
		// Get a cake from production_queue
		sem_wait(&full_production_queue);
		pthread_mutex_lock(&mutex_production_queue);

		char cake = production_queue.front();
		production_queue.pop();

		string cake_name;
		if (cake==kChocolateCakeChar) cake_name = "chocolate";
		else cake_name = "vanilla";
		pthread_mutex_lock(&mutex_print);
		cout << "Chef Z took a *"<< cake_name <<"* cake from production queue." << endl;
		PrintAllQueues();
		pthread_mutex_unlock(&mutex_print);

		sleep(kSleepingTimeAfterEachConsoleOutput);

		pthread_mutex_unlock(&mutex_production_queue);
		sem_post(&empty_production_queue);

		// Put the cake into corresponding queue
		if (cake==kChocolateCakeChar) {
			sem_wait(&empty_chocolate_queue);
			pthread_mutex_lock(&mutex_chocolate_queue);

			chocolate_queue.push(cake);

			pthread_mutex_lock(&mutex_print);
			cout << "Chef Z decorated a *chocolate* cake." << endl;
			PrintAllQueues();
			pthread_mutex_unlock(&mutex_print);

			sleep(kSleepingTimeAfterEachConsoleOutput);

			pthread_mutex_unlock(&mutex_chocolate_queue);
			sem_post(&full_chocolate_queue);
		}
		else {
			sem_wait(&empty_vanilla_queue);
			pthread_mutex_lock(&mutex_vanilla_queue);

			vanilla_queue.push(cake);

			pthread_mutex_lock(&mutex_print);
			cout << "Chef Z decorated a *vanilla* cake." << endl;
			PrintAllQueues();
			pthread_mutex_unlock(&mutex_print);

			sleep(kSleepingTimeAfterEachConsoleOutput);

			pthread_mutex_unlock(&mutex_vanilla_queue);
			sem_post(&full_vanilla_queue);
		}
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
}

void PrintAllQueues () {
	PrintQueue(production_queue, kProductionQueueName, kProductionQueueSize);
	cout << "\t\t";
	PrintQueue(chocolate_queue, kChocolateQueueName, kChocolateQueueSize);
	cout << "\t\t";
	PrintQueue(vanilla_queue, kVanillaQueueName, kVanillaQueueSize);
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
