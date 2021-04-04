#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <queue>

/// Namespace
using namespace std;

/// Constants
const int kProductionQueueSize = 200;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = '-';

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

	cout << "\nQueue size: " << queue_size << endl;

	return 0;
}

/// Function Definitions
void * MakeChefXWork (void *pVoid) {
	for (int i=0; i < kProductionQueueSize; i++) {
		pthread_mutex_lock(&mutex);
		// Critical section starts
		production_queue.push(kChocolateCakeChar);
		queue_size++;
		// Critical section ends
		pthread_mutex_unlock(&mutex);

		cout << kChocolateCakeChar;
	}
	return nullptr;
}

void * MakeChefYWork (void *pVoid) {
	for (int i=0; i < kProductionQueueSize; i++) {
		pthread_mutex_lock(&mutex);
		// Critical section starts
		production_queue.push(kVanillaCakeChar);
		cout << kVanillaCakeChar;
		queue_size++;
		// Critical section ends
		pthread_mutex_unlock(&mutex);
	}
	return nullptr;
}

void ShowQueue (queue<char> a_queue) {
	queue<char> the_queue = a_queue;
	cout << "Front-> ";
	while (!the_queue.empty()) {
		cout << the_queue.front();
		the_queue.pop();
	}
	cout << " ->End\n";
}