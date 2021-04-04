#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <queue>

/// Namespace
using namespace std;

/// Constants
const int kProductionQueueSize = 5;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = '-';
const bool kIsMutexLockOn = true; // For debug purposes
const bool kShouldWaitForCharInputBeforeEveryOutputLine = false; // For debug purposes

/// Global variables
queue<char> production_queue;
int queue_size = 0;
pthread_mutex_t mutex;

/// Function declarations
void * MakeChefXWork(void *pVoid);
void * MakeChefYWork(void *pVoid);
void ShowQueue(queue<char> a_queue);

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
	for (int i=0; i < kProductionQueueSize; i++) {
		if (kIsMutexLockOn) pthread_mutex_lock(&mutex);
		// Critical section starts
		production_queue.push(kChocolateCakeChar);
		queue_size++;
		for (int j=0; j<5000; j++) {queue_size++;queue_size--;} // To get a clearer view of race condition
		cout << "New cake by ChefX; production_queue: ";
		ShowQueue(production_queue);
		cout << endl;
		// Critical section ends
		if (kIsMutexLockOn) pthread_mutex_unlock(&mutex);
		if (kShouldWaitForCharInputBeforeEveryOutputLine) getchar(); // Pause and wait for an "Enter" press
	}
	return nullptr;
}

void * MakeChefYWork (void *pVoid) {
	for (int i=0; i < kProductionQueueSize; i++) {
		if (kIsMutexLockOn) pthread_mutex_lock(&mutex);
		// Critical section starts
		production_queue.push(kVanillaCakeChar);
		queue_size++;
		for (int j=0; j<5000; j++) {queue_size++;queue_size--;} // To get a clearer view of race condition
		cout << "New cake by ChefY; production_queue: ";
		ShowQueue(production_queue);
		cout << endl;
		// Critical section ends
		if (kIsMutexLockOn) pthread_mutex_unlock(&mutex);
		if (kShouldWaitForCharInputBeforeEveryOutputLine) getchar(); // Pause and wait for an "Enter" press
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