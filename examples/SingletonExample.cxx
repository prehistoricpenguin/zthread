//
// This example demonstrates the thread safe singleton and use of 
// Guard object to perfom scoped locking
//
#include <zthread/Thread.h>
#include <zthread/Singleton.h>
#include <zthread/Guard.h>
#include <zthread/FastMutex.h>

#include <iostream>

using namespace ZThread;
using namespace std;

#define TASKS 30


class SafeCounter {

  FastMutex lock;
  int total;

public:

  SafeCounter() : total(0) {
    cerr << "Creating counter" << endl;
  }

  ~SafeCounter() {
    cerr << "Destroying counter" << endl;
  }

  void increment() { // Increment the counter
    
    Guard<FastMutex> guard(lock);
    total++;

  }

  int count() {
    
    Guard<FastMutex> guard(lock);
    return total;

  }

};



class Task : public Runnable {
public:

  virtual ~Task() throw() {}

  virtual void run() throw() {
    Singleton<SafeCounter>::instance()->increment();
  }

};


int main(int argc, char* argv[]) {


  Thread t[TASKS];
  int n;

  for(n=0;n<TASKS;n++)
    t[n].run( RunnablePtr(new Task) );

  for(n=0;n<TASKS;n++)
    t[n].join();

  cerr << "Total: " << Singleton<SafeCounter>::instance()->count() << endl;

#ifdef WIN32
  cerr << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif
  return 0;

}
