#include <iostream>
#include <cstdio>
#include <thread>
#include <queue>

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

	ShowQueue(production_queue);
	cout << "\nQueue size: " << queue_size << endl;

	return 0;
}

void MakeChefXWork () {
	for (int i=0; i<200; i++) {
		production_queue.push('-');
		queue_size++;
		cout << "-";
	}
}

void MakeChefYWork () {
	for (int i=0; i<200; i++) {
		production_queue.push('v');
		cout << "v";
		queue_size++;
	}
}

void ShowQueue(queue<char> a_queue) {
	queue<char> the_queue = a_queue;
	cout << "\nFront";
	while (!the_queue.empty()) {
		cout << ' ' << the_queue.front();
		the_queue.pop();
	}
	cout << "End";
	cout << '\n';
}