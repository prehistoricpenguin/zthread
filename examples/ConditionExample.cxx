
//
// This example demonstrates the use of Condition variables to create
// a shared resource
//

#include <zthread/Thread.h>
#include <zthread/Condition.h>
#include <zthread/Guard.h>
#include <zthread/FastMutex.h>

#include <list>
#include <iostream>

using namespace ZThread;
using namespace std;


class Resources {

  FastMutex lock;
  Condition notEmpty;
  Condition notFull;

  list<int> data;
  unsigned int max;

public:

  Resources() : notEmpty(lock), notFull(lock) {
    max = 5;
  }

  void put(int val)
    throw(Synchronization_Exception) {

    Guard<FastMutex> guard(lock);

    while(data.size() == max)
      notFull.wait();

    data.push_back(val);

    notEmpty.signal();


  }

  int get()
    throw(Synchronization_Exception) {

    Guard<FastMutex> guard(lock);

    while(data.size() == 0)
      notEmpty.wait();

    int val = data.front(); data.pop_front();
      
    notFull.signal();

    return val;

  }

  int count()
    throw(Synchronization_Exception) {

    Guard<FastMutex> guard(lock);
    return data.size();

  }

};

static Resources pool;

class Consumer : public Runnable {
public:

  virtual ~Consumer() throw() {}

  virtual void run() throw() {

    try {

      for(;;) {
      
        cerr << "GET: " << pool.get() << endl;;
        Thread::sleep(1000);

      }

    } catch(Synchronization_Exception& e) { 
      cerr << "Consumer exception: "<< e.what() <<endl; 
    }

  }

};

class Producer : public Runnable {
public:

  virtual ~Producer() throw() {}

  virtual void run() throw () {

    try {

      for(;;) {
      
        int val = (rand() % 10) + 1;

        cerr <<"PUT: " <<val << endl;
        pool.put(val);

        Thread::sleep(500);

      }

    } catch(Synchronization_Exception& e) { 
      cerr << "Producer exception: "<<e.what() <<endl; 
    }

  }

};

int main(int argc, char* argv[]) {

  try {

    Thread c;
    c.run( RunnablePtr(new Consumer) );

    Thread p;
    p.run( RunnablePtr(new Producer) );

    Thread::sleep(10000);
    
    c.interrupt();
    p.interrupt();

    c.join();
    p.join();

  } catch(Synchronization_Exception& e) { cerr << e.what() << endl; }

#ifdef WIN32
  cerr << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif
  return 0;

}
