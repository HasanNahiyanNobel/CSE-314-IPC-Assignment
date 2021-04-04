#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <queue>
#include <thread>

/// Namespace
using namespace std;

/// Constants
const int kProductionQueueSize = 50;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = '-';
const bool kIsMutexLockOn = true; // For debug purposes
const int kSleepingTimeAfterEachConsoleOutput = 1; // In seconds

/// Global variables
queue<char> production_queue;
int queue_size = 0;
pthread_mutex_t mutex;

/// Function declarations
void * MakeChefXWork(void *pVoid);
void * MakeChefYWork(void *pVoid);
void ShowQueue(queue<char> a_queue);
void SleepABit(int sleeping_time);

/// Main function
int main () {
	pthread_t chef_x_thread;
	pthread_t chef_y_thread;

	pthread_create(&chef_x_thread, NULL, MakeChefXWork, NULL);
	pthread_create(&chef_y_thread, NULL, MakeChefYWork, NULL);

	pthread_join(chef_x_thread, NULL);
	pthread_join(chef_y_thread, NULL);

	cout << "Queue size: " << queue_size << endl;

	return 0;
}

/// Function Definitions
void * MakeChefXWork (void *pVoid) {
	while (true) {
		if (kIsMutexLockOn) pthread_mutex_lock(&mutex); // Critical section starts
		if (queue_size < kProductionQueueSize) {
			production_queue.push(kChocolateCakeChar);
			queue_size++;

			for (int j=0; j<5000; j++) {queue_size++;queue_size--;} // To get a clearer view of race condition

			cout << "New cake by ChefX; production_queue: ";
			ShowQueue(production_queue);
			cout << endl;
			SleepABit(kSleepingTimeAfterEachConsoleOutput);
		}
		if (kIsMutexLockOn) pthread_mutex_unlock(&mutex); // Critical section ends
	}
	return nullptr;
}

void * MakeChefYWork (void *pVoid) {
	while (true) {
		if (kIsMutexLockOn) pthread_mutex_lock(&mutex); // Critical section starts
		if (queue_size < kProductionQueueSize) {
			production_queue.push(kVanillaCakeChar);
			queue_size++;

			for (int j=0; j<5000; j++) {queue_size++;queue_size--;} // To get a clearer view of race condition

			cout << "New cake by ChefY; production_queue: ";
			ShowQueue(production_queue);
			cout << endl;
			SleepABit(kSleepingTimeAfterEachConsoleOutput);
		}
		if (kIsMutexLockOn) pthread_mutex_unlock(&mutex); // Critical section ends
	}
	return nullptr;
}

void ShowQueue (queue<char> a_queue) {
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
	this_thread::sleep_for(chrono::seconds {sleeping_time});
}