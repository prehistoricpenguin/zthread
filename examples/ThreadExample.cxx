#include <zthread/Guard.h>
#include <zthread/Handle.h>
#include <zthread/Mutex.h>
#include <zthread/Thread.h>
#include <zthread/ThreadLocal.h>

#define THREADS 5

using namespace ZThread;

#include <iostream>
using namespace std;

ThreadLocal<int> tls;
Mutex mtx;

static int value = 100;

class NullTask : public Runnable {
public:

  virtual ~NullTask() throw() {};

  virtual void run() throw() { }

};

// Lightwieght task
class LWP : public Runnable {
public:


  LWP() throw() { cout << "Crea" << endl; };
  virtual ~LWP() throw() { cout << "Destr" << endl; };

  virtual void run() throw() {

    {
      
      Guard<Mutex> guard(mtx);
      
      cout << "Running...." << endl << endl;
      cout << "Setting TLS" << endl;
      
      tls.set(value++ );
      
      cout << "TLS for this thread: " << tls.get() << endl;
      
    }

    try {

      Thread::current().setPriority(Medium);
      cout << Thread::current().getPriority() << endl;
      Thread::sleep(2000);

    } catch(Synchronization_Exception& e) { 
      cout << "===" << e.what() << endl;
      cout << "OK" << endl;
    }

    cout << "OK" << endl;

  }


};


class HWP : public Thread {
public:
  virtual ~HWP() throw() {};

  virtual void run() throw() {

    try {

      Guard<Mutex> guard(mtx);
      tls.set( value++ );

      cout << "TLS for this thread: " << tls.get() << endl;
      
      Thread::sleep(3000); // gets interrupted

    } catch(Synchronization_Exception& e) { 
      cout << e.what() << endl;
      cout << "OK" << endl;
    }
    
    cout << "HWP done" << endl;

  } 

};


int main() {

  try {

    Handle<LWP> h = RunnablePtr(new LWP);
    Handle<LWP> h2(h);
    Handle<LWP> h3(h2);

    LWP lwp;

    Thread t;
    t.run(new LWP());

    Thread t2;
    t2.setDaemon(true);
    t2.run(h);

    t.join();

    HWP hwp;
    hwp.start();
    hwp.join();


    /*

    cout << "TLS for main thread: " << tls.get() << endl;

    Thread daemon;
    daemon.setDaemon(true);
    daemon.run(new LWP);

    Thread t[THREADS];

    
    int n;
    for(n=0;n<THREADS;n++)
      t[n].run(lwp);

    for(n=0;n<THREADS;n++)
      t[n].join(); 

    HWP hwp;
    hwp.start();
    hwp.join();
    */
  } catch(Synchronization_Exception& e) { 
    cout << e.what() << endl;
  }


#ifdef WIN32
  cout << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif

  return 0;

}
