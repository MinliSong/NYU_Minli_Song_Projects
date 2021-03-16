#include <cstring>
#include <cstdlib>
#include <set>
#include "EStore.h"
#include "TaskQueue.h"
#include "sthread.h"
#include "RequestGenerator.h"
#include "RequestHandlers.h"
#include <iostream>

using namespace std;

class Simulation
{
    public:
    TaskQueue supplierTasks;
    TaskQueue customerTasks;
    EStore store;

    int maxTasks;
    int numSuppliers;
    int numCustomers;

    explicit Simulation(bool useFineMode) : store(useFineMode) { }
};

/*
 * ------------------------------------------------------------------
 * supplierGenerator --
 *
 *      The supplier generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the supplier queue, then
 *      stop all supplier threads by enqueuing arg->numSuppliers
 *      stop requests.
 *
 *      Use a SupplierRequestGenerator to generate and enqueue
 *      requests.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
supplierGenerator(void* arg)
{
 
    Simulation* sim = reinterpret_cast<Simulation*>(arg);
    SupplierRequestGenerator *mySupplierGenerator = new SupplierRequestGenerator(&sim->supplierTasks); 
    mySupplierGenerator->enqueueTasks(sim->maxTasks,&sim->store);
    mySupplierGenerator->enqueueStops(sim->numSuppliers);

  //  cout << "Supplier Generator thread created and all tasks are in queue" << sim->maxTasks <<endl;
    sthread_exit(); 
    return NULL;
}

/*
 * ------------------------------------------------------------------
 * customerGenerator --
 *
 *      The customer generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the customer queue, then
 *      stop all customer threads by enqueuing arg->numCustomers
 *      stop requests.
 *
 *      Use a CustomerRequestGenerator to generate and enqueue
 *      requests.  For the fineMode argument to the constructor
 *      of CustomerRequestGenerator, use the output of
 *      store.fineModeEnabled() method, where store is a field
 *      in the Simulation class.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
customerGenerator(void* arg)
{   
    Simulation* sim = reinterpret_cast<Simulation*>(arg);

    CustomerRequestGenerator *myCustomerGenerator = new CustomerRequestGenerator(&sim->customerTasks,sim->store.fineModeEnabled());  

    myCustomerGenerator->enqueueTasks(sim->maxTasks,&sim->store);
    myCustomerGenerator->enqueueStops(sim->numCustomers);

  //  cout << "Customer Generator thread created and all tasks are in queue"<< sim->maxTasks <<endl;;
    sthread_exit();
    return NULL;
}

/*
 * ------------------------------------------------------------------
 * supplier --
 *
 *      The main supplier thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the supplier queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
supplier(void* arg)
{   

    Simulation* sim = reinterpret_cast<Simulation*>(arg);
    Task task;

    while(1){//keep solving supplier tasks until stop handler envoke
        task = sim->supplierTasks.dequeue();
        task.handler(task.arg);
    }

    return NULL;
}

/*
 * ------------------------------------------------------------------
 * customer --
 *
 *      The main customer thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the customer queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
customer(void* arg)
{

    Simulation* sim = reinterpret_cast<Simulation*>(arg);
    Task task;

    while(1){ //keep solving customer task until stop handler envoke
        task = sim->customerTasks.dequeue();
        task.handler(task.arg);   
    }
    

    return NULL;
}

/*
 * ------------------------------------------------------------------
 * startSimulation --
 *      Create a new Simulation object. This object will serve as
 *      the shared state for the simulation. 
 *
 *      Create the following threads:
 *          - 1 supplier generator thread.
 *          - 1 customer generator thread.
 *          - numSuppliers supplier threads.
 *          - numCustomers customer threads.
 *
 *      After creating the worker threads, the main thread
 *      should wait until all of them exit, at which point it
 *      should return.
 *
 *      Hint: Use sthread_join.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
static void
startSimulation(int numSuppliers, int numCustomers, int maxTasks, bool useFineMode)
{
    Simulation* sim = new Simulation(useFineMode);//initial simulation 
    sim->numSuppliers = numSuppliers;
    sim->numCustomers = numCustomers;
    sim->maxTasks = maxTasks;
    sim->supplierTasks = TaskQueue();
    sim->customerTasks = TaskQueue();

    sthread_t supplierGeneratorThrd;//create supplier generator thread
    sthread_t customerGeneratorThrd;//create customer generator thread
    sthread_create(&supplierGeneratorThrd,supplierGenerator,(void *)sim);//supplier generator thread

    sthread_create(&customerGeneratorThrd,customerGenerator,(void *)sim);//customer generator thread
   
    int i;
    int n;
    sthread_t supplierThrd[numSuppliers];//crrete numSuppliers supplier threads
    for(i = 0; i < numSuppliers; i++){
	sthread_create(&supplierThrd[i],supplier,(void *)sim); 

    }
    

    sthread_t customerThrd[numCustomers];//create numCustomer customer threads
    for(i = 0; i < numCustomers; i++){
       sthread_create(&customerThrd[i],customer,(void *)sim);
    }
    

    //wait for all threads finish
    for(n = 0; n < numSuppliers; n++){ 
        sthread_join(supplierThrd[n]);
    }

    for(n = 0; n < numCustomers; n++){
        sthread_join(customerThrd[n]);
    }

    sthread_join(supplierGeneratorThrd);
    sthread_join(customerGeneratorThrd);
    
    sthread_exit();
}

int main(int argc, char **argv)
{
    bool useFineMode = false;
    // Seed the random number generator.
    // You can remove this line or set it to some constant to get deterministic
    // results, but make sure you put it back before turning in.
    srand(time(NULL));

    if (argc > 1)
        useFineMode = strcmp(argv[1], "--fine") == 0;
    startSimulation(10, 10, 100, useFineMode);

    return 0;
}

