#include "zthread/CancelableTask.h"
#include "zthread/ConcurrentExecutor.h"
#include "zthread/FastMutex.h"
#include <iostream>
#include <vector>

using namespace ZThread;
using namespace std;

class Task : public CancelableTask {
public:

  virtual void doRun() throw() {
  
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

#define NUM_TASKS 10

int main() {

  try {

    ConcurrentExecutor<FastMutex> executor;
    vector<Handle<Task>*> pending;
    
    // Create the tasks and submit them 
    for(int i=0; i<NUM_TASKS; i++) {

      pending.push_back(new Handle<Task>(new Task));
      executor.execute(*pending.back());

    }

    // User waits a while...
    Thread::sleep(1000);

    // ...then decides to cancel a specific job
    Handle<Task>& job = *pending[5];
    job->cancel();

    // Request the executor shutdown
    executor.cancel();

    // Clean up the task handles
    for(vector<Handle<Task>*>::iterator i = pending.begin(); i != pending.end(); ++i) 
      delete *i;

  } catch(Synchronization_Exception&) {
    // Error starting thread
  }
    
  return 0;
  
}

