//
// This example demonstrates how one might implement some kind of service
// using ZThreads. 
//

#include <zthread/Thread.h>
#include <zthread/Condition.h>
#include <zthread/Guard.h>
#include <zthread/Mutex.h>

#include <iostream>

using namespace ZThread;
using namespace std;


#define SLEEP_TIME 5

class Service : public Runnable {

  Mutex lock;
  Condition stopCondition;
  
  bool active;

public:

  Service() : stopCondition(lock), active(true) {
  }
  
  void stop() {

    {
      Guard<Mutex> guard(lock);
      active = false;
    }

    stopCondition.signal();

  }

  virtual ~Service() throw() {}
  
  virtual void run() throw() {

    try {
    
      cerr << "Service started" << endl;

      while(active) {

        Guard<Mutex> guard(lock);
    
        cerr << "Services performed..." << endl;

        if(stopCondition.wait(1000))
          break;

      }


    } catch(Interrupted_Exception& ie) { 
      cerr << "Service interrupted" << endl;
    } catch(Synchronization_Exception& e) {
      cerr << e.what() << endl;
    }

    cerr << "Service stopped" << endl;

  }
    
};

int main(int argc, char* argv[]) {

  try {

    Thread t;

    Handle<Service> h = RunnablePtr(new Service);

    t.run( h );

    // Sleep while the task executes
    try {
    
      cerr << "Sleeping " << SLEEP_TIME << " seconds..." << endl;
      Thread::sleep(SLEEP_TIME * 1000);
      cerr << "Woke up" << endl;

    } catch(Synchronization_Exception& e) { 
      cerr << e.what() << endl;
    }

    t.interrupt();

    h->stop();
    t.join();

  } catch(Synchronization_Exception& e) { cerr << e.what() << endl; }
  
  cerr << "Starting second instance" << endl;

  try {

    Thread t;

    Handle<Service> h = RunnablePtr(new Service);

    t.run( h );

    // Sleep while the task executes
    try {
    
      cerr << "Sleeping " << SLEEP_TIME << " seconds..." << endl;
      Thread::sleep(SLEEP_TIME * 1000);
      cerr << "Woke up" << endl;

    } catch(Synchronization_Exception& e) { 
      cerr << e.what() << endl;
    }


    t.interrupt();

    t.join();

  } catch(Synchronization_Exception& e) { cerr << e.what() << endl; }

#ifdef WIN32
  cerr << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif
  return 0;

}
