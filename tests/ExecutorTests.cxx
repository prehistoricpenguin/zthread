
#include "zthread/Thread.h"
#include "zthread/ConcurrentExecutor.h"
#include "zthread/PoolExecutor.h"
#include "zthread/FastMutex.h"
#include "zthread/NullLockable.h"
#include "zthread/ThreadedExecutor.h"

#include <iostream>
using namespace std;
using namespace ZThread;

class Task : public Runnable {
public:

  virtual ~Task() throw() { } 

  virtual void run() throw() { 
    cout << " Task Executed " << endl;
  }

};

PoolExecutor<FastMutex> executor1(5);
ThreadedExecutor<FastMutex> executor2;

Handle<Task> task1 = RunnablePtr(new Task);

class TestingThread : public Thread {
public:

  virtual ~TestingThread() throw() { }

  virtual void run() throw() { 

    executor1.execute(task1);

  }

};


int main() {

  try {

    TestingThread t[10];
    for(int i=0; i < 10; ++i)
      t[i].start();

    for(int i=0; i < 10; ++i)
      t[i].join();

    executor1.execute(new Task());
    executor1.cancel();

    executor2.execute(new Task());
    executor2.execute(new Task());
    executor2.cancel();


  } catch(...) { 
    assert(0); // This should not occur
  } 

}


