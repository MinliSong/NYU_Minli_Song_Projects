#include <iostream>
#include "TaskQueue.h"
#include <queue>
#include <deque>
#include <mutex>
#include "sthread.h"
#include "RequestHandlers.h"
#include "RequestGenerator.h"
#include <cassert>

using namespace std;

TaskQueue::
TaskQueue()
{
    smutex_init(&mutex);
    front = rear = NULL;
    qsize = 0;
    scond_init(&nonEmpty);
}

TaskQueue::
~TaskQueue()
{
    while(front!=NULL){ // delelte the nodes 
	Node* temp = front;
	front = front->next;
	delete temp;
    }
    smutex_destroy(&mutex);
    scond_destroy(&nonEmpty);
    qsize = 0;
    rear = NULL;
}

/*
 * ------------------------------------------------------------------
 * size --
 *
 *      Return the current size of the queue.
 *
 * Results:
 *      The size of the queue.
 *
 * ------------------------------------------------------------------
 */
int TaskQueue::
size()
{   
    return qsize; // Keep compiler happy until routine done.
}

/*
 * ------------------------------------------------------------------
 * empty --
 *
 *      Return whether or not the queue is empty.
 *
 * Results:
 *      The true if the queue is empty and false otherwise.
 *
 * ------------------------------------------------------------------
 */
bool TaskQueue::
empty()
{	
    
    if(qsize ==0){
	    return true;
    }
    return false; // Keep compiler happy until routine done.
}

/*
 * ------------------------------------------------------------------
 * enqueue --
 *
 *      Insert the task at the back of the queue.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void TaskQueue::
enqueue(Task task)
{	 
    smutex_lock(&mutex);
    Node* temp = new Node; 
    if(temp == NULL){ 
	cout<<"overflow";
	return;
    }
    temp->task = task; 
    temp->next = NULL;
    assert(temp!=NULL);
    if(front == NULL){ //the input is the first task 
        front = rear = temp;
    }
    else{ //at least one task already in queue
	rear->next = temp;
	rear = temp;
    }
  
    qsize ++;
    scond_signal(&nonEmpty,&mutex);
    smutex_unlock(&mutex);
}

/*
 * ------------------------------------------------------------------
 * dequeue --
 *
 *      Remove the Task at the front of the queue and return it.
 *      If the queue is empty, block until a Task is inserted.
 *
 * Results:
 *      The Task at the front of the queue.
 *
 * ------------------------------------------------------------------
 */
Task TaskQueue::
dequeue()
{    
    smutex_lock(&mutex);
    
    while(qsize==0){ //wait until task avalible
	scond_wait(&nonEmpty,&mutex);
    }
    qsize --;
  
    assert(front!=NULL);
    assert(rear!=NULL);
    Task t = front->task;

    if(front == rear){ // only one task in queue 
	front = rear = NULL;
    }
    else{ //many tasks in queue
        Node *temp = front;
	front = front->next;
	free(temp);
    }
    smutex_unlock(&mutex);
    return t;
}

