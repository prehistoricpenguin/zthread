//
// This example demonstrates the use of sempahores to build a simple
// resource queue. The procuder should fill the queue faster than the
// consumer can empty it - stopping the producer when it catches a 
// range exceeded exception. Then the consumer will continue to 
// empty the queue - after 20 seconds the example quits
//
#include <zthread/Thread.h>
#include <zthread/Semaphore.h>
#include <zthread/FastMutex.h>
#include <zthread/Guard.h>

#include <iostream>
#include <list>

using namespace ZThread;
using namespace std;

// Bounded resource queue, starts empty - max 5 items

class Resources {

  FastMutex lock;
  Semaphore* s;
  list<int> data;

public:

  Resources() 
    throw(Initialization_Exception) {

    s = new Semaphore(0, 5);

  }

  ~Resources() {

    delete s;

  }

  int get()
    throw(Synchronization_Exception) {

    cout << s->count() << endl;
    int val;
    s->acquire();

    Guard<FastMutex> guard(lock);
    
    cerr << data.size() << endl;
    val = data.front(); data.pop_front();

    return val;

  }

  void put(int val)
    throw(Synchronization_Exception) {
   
    Guard<FastMutex> guard(lock);
    data.push_back(val);

    s->release();

  }

  int count()
    throw(Synchronization_Exception) {

    Guard<FastMutex> guard(lock);
    return s->count();

  }

};


static Resources pool;
static FastMutex ilock;
static int id = 0;

class Consumer : public Runnable {
public:

  virtual ~Consumer() throw() {}

  virtual void run() throw() {

    try {
      
      ilock.acquire();
      int me = id++;
      ilock.release();

      cout << me << " : consumer" << endl;

      for(;;) {
      
        cout << "GET: " << pool.get() << " from " << me << endl;

      }

    } catch(Synchronization_Exception& e) { 
      cout << "Consumer exception: " << e.what() << endl; 
    }

  }

};

class Producer : public Runnable {
public:

  virtual ~Producer() throw() {}

  virtual void run() throw() {

    try {

      for(;;) {
      
        int val = (rand() % 10) + 1;

        cout << "PUT: " << val << endl;
        pool.put(val);

        Thread::sleep(250);

      }

    } catch(Synchronization_Exception& e) { 
      cout << "Producer exception: " << e.what() << endl; 
    }

  }

};

int main(int argc, char* argv[]) {

  try {

    Thread c1, c2;

    c1.run( RunnablePtr(new Consumer) ); 
    c2.run( RunnablePtr(new Consumer) );

    Thread p;
    p.run( RunnablePtr(new Producer) );

    Thread::sleep(15000);


    try { 
      c1.interrupt();
    } catch(Synchronization_Exception& e) { cout << "I:" <<  e.what() << endl; }

    try {
      p.interrupt();
    } catch(Synchronization_Exception& e) { cout << "J:" <<  e.what() << endl; }

    try { 
      c2.interrupt();
    } catch(Synchronization_Exception& e) { cout << "K:" <<  e.what() << endl; }

    try { 

    c1.join();
    cout << "Consumer 1 joined." << endl;

    p.join();
    cout << "Producer joined." << endl;

    c2.join();
    cout << "Consumer 2 joined." << endl;
    } catch(Synchronization_Exception& e) { cout << "J:" <<  e.what() << endl; }

  } catch(Synchronization_Exception& e) { cout << e.what() << endl; }

#ifdef WIN32
  cout << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif
  return 0;

}
  
