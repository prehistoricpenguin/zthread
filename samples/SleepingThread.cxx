//: c11:SleepingThread.cpp
// Calling sleep() to wait for awhile..
#include "zthread/Thread.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class SleepingThread : public Thread {

  static int threadCount;

  int countDown;
  int threadNumber;

public:

  SleepingThread() : countDown(5), threadNumber(++threadCount) {   
  }

  virtual ~SleepingThread() throw() {
  }

  virtual void run() throw() {
    try {
      while(true) {
        cout << *this << endl;
        if(--countDown == 0) return;
        sleep(100);
      }

    } catch(Interrupted_Exception&) {
      // Exit thread on error
    }
  }

  int getNumber() const { return threadNumber; }

  int getCount() const { return countDown; }

  friend ostream& 
  operator<<(ostream& os, const SleepingThread& t) {
    return os << "#" << t.getNumber() << " : " << t.getCount();
  }

};

int SleepingThread::threadCount = 0;

int main() {

  SleepingThread t[5];

  for(int i = 0; i < 5; i++) {
    t[i].start();
    t[i].join();
  }

} ///:~
