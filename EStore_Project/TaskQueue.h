#pragma once

#include <queue>
#include "sthread.h"

typedef void (*handler_t) (void *); 

struct Task {
    handler_t handler;
    void* arg;
};

struct Node{ //node for the task queue inplement by linked list
    Task task;
    Node* next;
};

/*
 * ------------------------------------------------------------------
 * TaskQueue --
 * 
 *      A thread-safe task queue. This queue should be implemented
 *      as a monitor.
 *
 * ------------------------------------------------------------------
 */
class TaskQueue {
    private:
    smutex_t mutex;
    int qsize;//trace the size of the queue
    Node* front;
    Node* rear;
    scond_t nonEmpty;

    public:
    TaskQueue();
    ~TaskQueue();

    void enqueue(Task task);
    Task dequeue();

    int size();
    bool empty();
};

