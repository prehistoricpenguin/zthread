//: c11:SimplePriorities.cpp
// Shows the use of thread priorities.
#include "zthread/Thread.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class SimplePriorities : public Thread {

  static const double E;
  static const double PI;

  static int threadCount;

  int countDown;
  int threadNumber;

  volatile double d; // No optimization

public:

  SimplePriorities(Priority priority) : countDown(5), threadNumber(++threadCount), d(0) {
    setPriority(priority);
    start();
  }

  virtual ~SimplePriorities() throw() {
  }

  virtual void run() throw() {
    while(true) {

      // An expensive, interruptable operation:
      for(int i = 1; i < 100000; i++) {
        d = d + (PI + E) / (double)i;
        if(i%1000 == 0 && interrupted()) return;
      }

      cout << *this << endl;
      if(--countDown == 0) return;

    }
  }
  
  int getNumber() const { return threadNumber; }

  int getCount() const { return countDown; }

  friend ostream& 
  operator<<(ostream& os, const SimplePriorities& t) {
    return os << "#" << t.getNumber() << " : " << t.getCount();
  }

};

const double SimplePriorities::E  = 2.7182818284590452354;
const double SimplePriorities::PI = 3.14159265358979323846;
int SimplePriorities::threadCount = 0;


int main() {

  SimplePriorities t0(Low);
  SimplePriorities t1(Medium);
  SimplePriorities t2(High);

  t0.join();
  t1.join();
  t2.join();

} ///:~
