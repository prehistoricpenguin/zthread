#include "zthread/Thread.h"
#include "zthread/Condition.h"
#include "zthread/Guard.h"
#include "zthread/Mutex.h"

#include <iostream>

using namespace ZThread;
using namespace std;

class WorkerThread : public Thread {

  Mutex lock;
  Condition workReady;
  
public:

  WorkerThread() : workReady(lock) { }
  
  virtual ~WorkerThread() throw() { }
  
  
  virtual void run() throw() {
  
    try {
          
      // Wait for some work to arrive
      Guard<Mutex> g(lock);           
      workReady.wait();
      
      doSomeWork();
      
       
    } catch(Interrupted_Exception&) {
      // Thread interrupted, exit
      cout << "Interrupted!" << endl;
    }
    
  }
 
  void doSomeWork() {
    // ...
  }

};


int main() {

  try {
  
    // Start the worker thread
    WorkerThread t;
    t.start();
    
    // Let it run for a while
    
    
    // User hits a button, want to exit
    t.interrupt();
    t.join();
    
  } catch(Synchronization_Exception&) {
    // Error starting thread
  }
    
  return 0;
  
}

