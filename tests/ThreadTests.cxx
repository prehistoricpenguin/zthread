// To use this test, adjust CONTEXTS to your liking,
// and run the test (linked with a debug enabled is recommended)
// If the test deadlocks, or asserts then the test has failed.

// Default to 100 
#if !defined(CONTEXTS)
#  define CONTEXTS 100
#endif

#include "zthread/Thread.h"
#include "zthread/FastMutex.h"
#include "zthread/CancelableTask.h"

#include <assert.h>
#include <iostream>
using namespace std;
using namespace ZThread;

class NullTask : public Runnable {
public:

  virtual ~NullTask() throw() {
  }

  virtual void run() throw() {

  }

};


class MyTask : public CancelableTask {
public:

  virtual ~MyTask() throw() {
  }

  virtual void doRun() throw() {

  }

  virtual void doCancel(Thread::Reference& current) throw() {

  }

};

// This test tries to find any problems involved in starting up
// and joining a set of threads. It can't garuntee that a class
// is 100% correct, but it can root out alot of bugs. If there are
// any race conditions in the Monitor class, this will probably find 
// them.


int main() {
  
  Thread t[CONTEXTS];

  Handle<MyTask> z = RunnablePtr(new MyTask);

  try {
    
    cout << "Thread Test:" << endl;
    cout << "Testing Thread.run()" << endl;

    for(int i=0;i<CONTEXTS;++i)
      t[i].run(z);

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
    assert(0);
  }

  try {

    cout << "Testing Thread.join()" << endl;

    for(int i=0;i<CONTEXTS;++i)
      t[i].join();

    cout << "Thread Test: PASSED" << endl;

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
    assert(0);
  }

  return 0;

}
