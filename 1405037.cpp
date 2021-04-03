#include <iostream>
#include <cstdio>
#include <thread>
#include <pthread.h>

using namespace std;

void foo (int x) {
	for (int i=0; i<100; i++) {
		cout << "Executing thread " << x << endl;
	}
}

int main () {
	thread thread_1(foo, 1);
	thread thread_2(foo, 2);
	thread thread_3(foo, 3);
	thread thread_4(foo, 4);
	thread thread_5(foo, 5);

	thread_1.join();
	thread_2.join();
	thread_3.join();
	thread_4.join();
	thread_5.join();

	return 0;
}