#include <zthread/Guard.h>
#include <zthread/Handle.h>
#include <zthread/Mutex.h>
#include <zthread/Thread.h>
#include <zthread/ThreadLocal.h>

#define THREADS 5

using namespace ZThread;

#include <iostream>
using namespace std;
Mutex mtx;

// Lightwieght task
class LWP : public Runnable {

public:

  virtual ~LWP() throw() { };

  virtual void run() throw() {
    Guard<Mutex> g(mtx);
    cout << "OK" << endl;
  }

};

int main() {

  Thread t;
  
  try {

    t.run(new LWP());
    t.join();
    
  } catch(Synchronization_Exception& e) { 
    cout << e.what() << endl;
  }

  return 0;

}
