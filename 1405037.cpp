#include <iostream>
#include <cstdio>
#include <thread>
#include <queue>

using namespace std;

queue<char> production_queue;

void MakeChefXWork ();
void MakeChefYWork ();
void ShowQueue(queue<char> a_queue);

int main () {
	thread chef_x_thread(MakeChefXWork);
	thread chef_y_thread(MakeChefYWork);

	chef_x_thread.join();
	chef_y_thread.join();

	//ShowQueue(production_queue);

	return 0;
}

void MakeChefXWork () {
	for (int i=0; i<100; i++) {
		//production_queue.push('-');
		cout << "-";
	}
}

void MakeChefYWork () {
	for (int i=0; i<100; i++) {
		//production_queue.push('v');
		cout << "v";
	}
}

void ShowQueue(queue<char> a_queue) {
	queue<char> the_queue = a_queue;
	cout << "Front";
	while (!the_queue.empty()) {
		cout << ' ' << the_queue.front();
		the_queue.pop();
	}
	cout << "End";
	cout << '\n';
}