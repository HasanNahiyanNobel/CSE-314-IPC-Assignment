#include <iostream>
#include <cstdio>
#include <thread>
#include <queue>
#define PRODUCTION_QUEUE_SIZE 200
#define CHOCOLATE_CAKE_CHAR 'c'
#define VANILLA___CAKE_CHAR '-'

using namespace std;

queue<char> production_queue;
int queue_size = 0;

void MakeChefXWork ();
void MakeChefYWork ();
void ShowQueue(queue<char> a_queue);

int main () {
	thread chef_x_thread(MakeChefXWork);
	thread chef_y_thread(MakeChefYWork);

	chef_x_thread.join();
	chef_y_thread.join();

	cout << endl;
	ShowQueue(production_queue);
	cout << "\nQueue size: " << queue_size << endl;

	return 0;
}

void MakeChefXWork () {
	for (int i=0; i<PRODUCTION_QUEUE_SIZE; i++) {
		production_queue.push(CHOCOLATE_CAKE_CHAR);
		queue_size++;
		cout << CHOCOLATE_CAKE_CHAR;
	}
}

void MakeChefYWork () {
	for (int i=0; i<PRODUCTION_QUEUE_SIZE; i++) {
		production_queue.push(VANILLA___CAKE_CHAR);
		cout << VANILLA___CAKE_CHAR;
		queue_size++;
	}
}

void ShowQueue(queue<char> a_queue) {
	queue<char> the_queue = a_queue;
	cout << "Front-> ";
	while (!the_queue.empty()) {
		cout << the_queue.front();
		the_queue.pop();
	}
	cout << " ->End\n";
}