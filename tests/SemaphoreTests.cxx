// Default to 10
#if !defined(CONTEXTS)
#  define CONTEXTS 10
#endif


#include "zthread/Thread.h"
#include "zthread/Guard.h"
#include "zthread/Semaphore.h"
#include <unistd.h>
#include "zthread/DefaultThreadFactory.h"

#include <assert.h>
#include <iostream>
using namespace std;
using namespace ZThread;

// This tests the prority sensative mutex classes

static Semaphore p1(1,1);

class PriorityThread : public Thread {
public:

  virtual ~PriorityThread() throw() { }

  virtual void run() throw() {

    try {

      Guard<Semaphore> g(p1);
      cout << getPriority() << endl;
      Thread::sleep(1000);

    } catch(...) { 
      assert(0); // This should not occur
    } 

  }

};

int main() {

  try {

    Priority p[] = {Low, Medium, High};

    PriorityThread t[CONTEXTS];
    
    for(int i=CONTEXTS; i > 0;) {
      t[i].setPriority(p[--i % 3]);
      t[i].start();
    }
    
    for(int i=CONTEXTS; i > 0;)
      t[--i].join();

  } catch(...) { 
    assert(0); // This should not occur
  } 

}
