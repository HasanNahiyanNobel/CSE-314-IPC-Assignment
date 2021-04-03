#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <cstdio>
#include <cstring>

using namespace std;

void IPC (string a_string) {
	cout << a_string;
}

int main () {
	IPC("Hello!");
	return 0;
}