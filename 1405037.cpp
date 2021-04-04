#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <queue>

using namespace std;

const int kProductionQueueSize = 200;
const char kChocolateCakeChar = 'c';
const char kVanillaCakeChar = '-';

queue<char> production_queue;
int queue_size = 0;

void * MakeChefXWork(void *pVoid);
void * MakeChefYWork(void *pVoid);
void ShowQueue(queue<char> a_queue);

int main () {
	pthread_t chef_x_thread;
	pthread_t chef_y_thread;

	pthread_create(&chef_x_thread, NULL, MakeChefXWork, NULL);
	pthread_create(&chef_y_thread, NULL, MakeChefYWork, NULL);

	pthread_join(chef_x_thread, NULL);
	pthread_join(chef_y_thread, NULL);

	return 0;
}

void * MakeChefXWork (void *pVoid) {
	for (int i=0; i < kProductionQueueSize; i++) {
		production_queue.push(kChocolateCakeChar);
		queue_size++;
		cout << kChocolateCakeChar;
	}
	return nullptr;
}

void * MakeChefYWork (void *pVoid) {
	for (int i=0; i < kProductionQueueSize; i++) {
		production_queue.push(kVanillaCakeChar);
		cout << kVanillaCakeChar;
		queue_size++;
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