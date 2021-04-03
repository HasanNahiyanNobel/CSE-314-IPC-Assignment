#ifndef IPC_ASSIGNMENT_SEMAPHORE_H
#define IPC_ASSIGNMENT_SEMAPHORE_H


class Semaphore {
public:
	int value;
	struct process *list;
};


#endif //IPC_ASSIGNMENT_SEMAPHORE_H
