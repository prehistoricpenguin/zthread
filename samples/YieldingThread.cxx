//: c11:YieldingThread.cpp
// Suggesting when to switch threads with yield().
#include "zthread/Thread.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class YieldingThread : public Thread {

  static int threadCount;

  int countDown;
  int threadNum;

public:

  YieldingThread() : countDown(5), threadNum(++threadCount) {
    start();
  }

  virtual ~YieldingThread() throw() {
  }

  virtual void run() throw() {
    while(true) {
      cout << *this << endl;
      if(--countDown == 0) return;
      yield();
    }
  }


  int getNum() const { return threadNum; }

  int getCount() const { return countDown; }

  friend ostream& 
  operator<<(ostream& os, const YieldingThread& yt) {
    return os << "#" << yt.getNum() << " : " << yt.getCount();
  }

};

int YieldingThread::threadCount = 0;

int main() {

  YieldingThread t[5];

  for(int i = 0; i < 5; i++)
    t[i].join();

} ///:~
