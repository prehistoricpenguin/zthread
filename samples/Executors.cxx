//: c11:Executors.cxx
// Demonstrate Executor use.
#include "zthread/ConcurrentExecutor.h"
#include "zthread/Mutex.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class Task : public Runnable {

  int countDown;

public:

  Task(int n) : countDown(n) {}

  virtual ~Task() throw() { }

  virtual void run() throw() {

    while(countDown--) 
      cout << countDown << endl;

    cout << "Liftoff!" << endl;

  }

};

int main() {

  ConcurrentExecutor<Mutex> executor;
  
  try {

    for(int i = 0; i < 5; i++)
      executor.execute( new Task(10) );

    executor.cancel();

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

} ///:~
