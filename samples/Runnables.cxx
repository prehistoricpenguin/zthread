//: c11:Runnables.cpp
// Demonstration of the Runnable interface.
#include "zthread/Runnable.h"
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

  Task t(10);
  t.run();

} ///:~
