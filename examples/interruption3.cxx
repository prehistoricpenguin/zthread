#include "zthread/Thread.h"
#include "zthread/FastMutex.h"
#include "zthread/MonitoredQueue.h"

#include <iostream>

using namespace ZThread;
using namespace std;

class Task : public Runnable {
public:

  virtual void run() throw() {
  
    try {
    
      if(Thread::interrupted())
        throw Interrupted_Exception();

      Thread::sleep(1000);
      cout << "Task Complete!" << endl;
      
    } catch(Interrupted_Exception&) {

      // Interrupted, clean up & don't propogate the exception
      cout << "Task Interrupted!" << endl;
 
    }

  }
  
};

class WorkerThread : public Thread {

  MonitoredQueue<Runnable*, FastMutex>& _q;
  
public:

  WorkerThread(MonitoredQueue<Runnable*, FastMutex>& q) : _q(q) { }
  
  virtual ~WorkerThread() throw() { }
  
  // ...
  
  virtual void run() throw() {
  
    Runnable* task = 0;
    
    try {
      
      while(!Thread::interrupted()) {
      
        task = _q.next();
        task->run();
          
        delete task;
        
      }
       
    } catch(Interrupted_Exception&) {
      // Interrupted waiting for tasks, update a log entry
    } catch(Cancellation_Exception&) {
      // Queue canceled, normal exit
    }
    
    if(task)
      delete task;
      
  }
  
};

#define NUM_TASKS 10

int main() {

  try {
  
    MonitoredQueue<Runnable*, FastMutex> q;

    for(int i=0; i<NUM_TASKS; i++)
      q.add(new Task());
    
    // Start the worker thread
    WorkerThread t(q);
    t.start();
    
    // Let it run for a while   
    Thread::sleep(2000);

    // User hits a button, wants to cancel current job. 
    t.interrupt();
    Thread::sleep(2000);

    // User hits a button, wants to cancel current job. 
    t.interrupt();
    Thread::sleep(2000);

    q.cancel();
    t.join();
    
  } catch(Synchronization_Exception&) {
    // Error starting thread
  }
    
  return 0;
  
}

