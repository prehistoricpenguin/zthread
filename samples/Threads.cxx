//: c11:SimpleThread.cxx
// Very simple Threading example.
#include "zthread/Thread.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class Task : public Runnable {

  int countDown;

public:

  Task(int n) : countDown(n) {}

  virtual ~Task() throw() {}

  virtual void run() throw() {

    while(countDown--) 
      cout << countDown << endl;

    cout << "Liftoff!" << endl;

  }

};

int main() {

  Thread t;
  
  try {
    t.run(new Task(10));
    t.join();
  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

} ///:~
