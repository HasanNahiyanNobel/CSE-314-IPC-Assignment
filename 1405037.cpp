#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <chrono>
#include <queue>
#include <thread>

/// Namespace
using namespace std;
using namespace chrono;

/// Constants
const int kProductionQueueSize = 50;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = '-';
const bool kIsMutexLockOn = true; // For debug purposes
const int kSleepingTimeAfterEachConsoleOutput = 0; // In milliseconds
const int kExecutionTimeLimit = 10000; // In milliseconds, though execution will continue if a thread has called GetExecutionTime() before reaching that time limit.

/// Global variables
queue<char> production_queue;
int queue_size = 0;
pthread_mutex_t mutex;
auto start = high_resolution_clock::now();

/// Function declarations
void * MakeChefXWork(void *pVoid);
void * MakeChefYWork(void *pVoid);
void * MakeChefZWork(void *pVoid);
void ShowQueue(queue<char> a_queue);
void SleepABit(int sleeping_time);
int GetExecutionTime();

/// Main function
int main () {
	pthread_t chef_x_thread;
	pthread_t chef_y_thread;
	pthread_t chef_z_thread;

	pthread_create(&chef_x_thread, nullptr, MakeChefXWork, nullptr);
	pthread_create(&chef_y_thread, nullptr, MakeChefYWork, nullptr);
	pthread_create(&chef_z_thread, nullptr, MakeChefZWork, nullptr);

	pthread_join(chef_x_thread, nullptr);
	pthread_join(chef_y_thread, nullptr);
	pthread_join(chef_z_thread, nullptr);

	cout << "Queue size: " << queue_size << endl;
	cout << "Execution ended in: " << GetExecutionTime() << "ms" << endl;

	return 0;
}

/// Function Definitions
void * MakeChefXWork (void *pVoid) {
	while (GetExecutionTime() < kExecutionTimeLimit) {
		if (kIsMutexLockOn) pthread_mutex_lock(&mutex); // Critical section starts
		if (production_queue.size() < kProductionQueueSize) {
			production_queue.push(kChocolateCakeChar);
			queue_size++;

			for (int j=0; j<5000; j++) {queue_size++;queue_size--;} // To get a clearer view of race condition

			cout << "New cake by Chef X; production_queue:\t";
			ShowQueue(production_queue);
			cout << endl;
			SleepABit(kSleepingTimeAfterEachConsoleOutput);
		}
		if (kIsMutexLockOn) pthread_mutex_unlock(&mutex); // Critical section ends
	}
	return nullptr;
}

void * MakeChefYWork (void *pVoid) {
	while (GetExecutionTime() < kExecutionTimeLimit) {
		if (kIsMutexLockOn) pthread_mutex_lock(&mutex); // Critical section starts
		if (production_queue.size() < kProductionQueueSize) {
			production_queue.push(kVanillaCakeChar);
			queue_size++;

			for (int j=0; j<5000; j++) {queue_size++;queue_size--;} // To get a clearer view of race condition

			cout << "New cake by Chef Y; production_queue:\t";
			ShowQueue(production_queue);
			cout << endl;
			SleepABit(kSleepingTimeAfterEachConsoleOutput);
		}
		if (kIsMutexLockOn) pthread_mutex_unlock(&mutex); // Critical section ends
	}
	return nullptr;
}

void * MakeChefZWork (void *pVoid) {
	while (GetExecutionTime() < kExecutionTimeLimit) {
		if (kIsMutexLockOn) pthread_mutex_lock(&mutex); // Critical section starts
		if (production_queue.size() > 0) {
			char a_cake = production_queue.front();
			production_queue.pop();
			queue_size--;

			for (int j=0; j<5000; j++) {queue_size++;queue_size--;} // To get a clearer view of race condition

			cout << "Cake taken by Chef Z; production_queue:\t";
			ShowQueue(production_queue);
			cout << endl;
			SleepABit(kSleepingTimeAfterEachConsoleOutput);
		}
		if (kIsMutexLockOn) pthread_mutex_unlock(&mutex); // Critical section ends
	}
	return nullptr;
}

void ShowQueue (queue<char> a_queue) {
	if (a_queue.size()==0) {
		return;
	}
	queue<char> the_queue = a_queue;

	//Output the first element, without a preceding space
	cout << the_queue.front();
	the_queue.pop();

	//Output the rest
	while (!the_queue.empty()) {
		cout << ' ' << the_queue.front();
		the_queue.pop();
	}
}

void SleepABit (int sleeping_time) {
	this_thread::sleep_for(milliseconds{sleeping_time});
}

int GetExecutionTime () {
	auto now = chrono::high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - start);
	return duration.count();
}
