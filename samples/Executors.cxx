//: c11:Executors.cxx
// Demonstrate Executor use.
#include "zthread/ConcurrentExecutor.h"
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
      cout << coutDown << endl;

    cout << "Liftoff!" << endl;

  }

};

int main() {

  ConcurrentExecutor<> executor;
  
  try {

    for(int i = 0; i < 5; i++)
      executor.run(new Task);

    executor.cancel();

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

} ///:~
