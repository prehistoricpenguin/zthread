//: c11:Blocked.cxx
// Demonstrate interrupting a blocked thread.
#include "zthread/Condition.h"
#include "zthread/Mutex.h"
#include "zthread/Thread.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class Blocked : public Runnable {
  Mutex lock;
  Condition cond;
public:

  Blocked() : cond(lock) {}

  virtual ~Blocked() throw() {}

  virtual void run() throw() {
    
    try {

      Guard<Mutex> g(lock);

      for(;;)
        cond.wait();      

    } catch(Interrupted_Exception&) {
      // Exit the task 
    } catch(Synchronization_Exception& e) {
      cerr << e.what() << endl;
    }

  }
 
};

int main() {

  try {

    Thread t;

    t.run(new Blocked);

    Thread::sleep(4000);

    t.interrupt();
    t.join();

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

} ///:~