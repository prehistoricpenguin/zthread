// To use this test, adjust CONTEXTS and TRIALS to your liking,
// and run the test (linked with a debug enabled is recommended)
// If the test deadlocks, or asserts then the test has failed.

// This test puts an enormous amount of contention on a few spin
// locks and its not unsually to see the scheduler order the execution
// so that all the threads begin eating up the CPU. That's to be expected
// sometimes, but even then you should see things start to slowly move
// along and speed up again as threads exit or fight over a different
// lock.

// Default to 100 
#if !defined(CONTEXTS)
#  define CONTEXTS 100
#endif

// Default to 100 
#if !defined(TRIALS)
#  define TRIALS 100
#endif


#include "zthread/FastMutex.h"
#include "zthread/FastRecursiveMutex.h"
#include "zthread/Mutex.h"
#include "zthread/RecursiveMutex.h"
#include "zthread/Thread.h"

#include <assert.h>
#include <iostream>
using namespace std;
using namespace ZThread;

template <class TestCase, int Contexts=CONTEXTS>
class TestHarness {

  Thread t[Contexts];

public:

  TestHarness() {

    Handle<TestCase> test = RunnablePtr(new TestCase);

    for(int i=0;i<Contexts;++i)
      t[i].run(test);

  }

  ~TestHarness() {
    for(int i=0;i<Contexts;++i)
      t[i].join();
  }
  
};

// This test attempts to find bugs in a mutex class  
// by repeatedly locking & unlocking each mutex in 
// with a number of different threads.
template <class LockType, int Iterations=TRIALS>
class MutexTest1 : public Runnable {

  LockType mtx;
  volatile int _counter;

public:
  
  MutexTest1() throw() : _counter(0) {}
  virtual ~MutexTest1() throw() {}

  virtual void run() throw() {
    
    try {

      // Keep trying to grab the lock
      for(unsigned int i = Iterations; i != 0; --i) {

        while(!mtx.tryAcquire(100));       
       
        assert(_counter == 0);
        _counter++;
        _counter--;

        mtx.release();

        mtx.acquire();

        assert(_counter == 0);
        _counter++;
        _counter--;

        mtx.release();

      }

    } catch(...) { 
      assert(0); // This should not occur
    } 

  }

};

// This test attempts to find bugs in a recursive mutex class  
// by repeatedly locking & unlocking each mutex in 
// with a number of different threads. Depending on how the
// FastRecursiveMutex is implemented, this could be slow when
// a large number of threads are in contention for this mutex

template <class LockType, int Iterations=TRIALS>
class MutexTest2 : public Runnable {

  LockType mtx;
  volatile int _counter;

public:

  MutexTest2() throw() : _counter(0) {}
  virtual ~MutexTest2() throw() {}

  virtual void run() throw() {
    
    try {

      // Keep trying to grab the lock, aggresively
      for(unsigned int i = Iterations; i != 0; --i) {

        // Spin util we have access
        while(!mtx.tryAcquire(100));

        assert(_counter == 0);
        _counter++;
        
        // Already hold the lock, this should be true
        if(!mtx.tryAcquire(100)) {
          assert(0);
        }
        
        // Try once more just to try to mix things up
        mtx.acquire();
        
        mtx.release();          
        mtx.release();
        
        _counter--;          
        mtx.release();

      }

    } catch(...) { 
      assert(0); // This should not occur
    } 

  }

};


// Run through several test cases and "exercise" the Mutex
// classes. This cannot garuntee that the classes are %100
// correct because the scheduler is not predicatable - but
// this test will find mosy bugs pretty quickly.

class SerialTest : public Runnable {

  MutexTest1<FastMutex>          test1;
  MutexTest1<FastRecursiveMutex> test2;
  MutexTest1<Mutex>              test3;
  MutexTest1<RecursiveMutex>     test4;
  
public:
  
  virtual ~SerialTest() throw() {} 

  virtual void run() throw() {

    test1.run();
    test2.run();
    test3.run();
    test4.run();

  }

};

class RecursiveTest : public Runnable {

  MutexTest2<FastRecursiveMutex> test1;
  MutexTest2<RecursiveMutex>     test2;
  
public:
  
  virtual ~RecursiveTest() throw() {} 

  virtual void run() throw() {

    test1.run();
    test2.run();

  }

};

int main() {

  // Start with the serial test
  cout << "Serial Test:" << endl;
  cout << "Testing Mutex, RecursiveMutex, FastMutex and FastRecusiveMutex..." << endl;

  try {

    TestHarness<SerialTest> serialTest;
 
  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

  cout << "Serial Test: PASSED" << endl;
  cout << "RecursiveMutex and FastRecusiveMutex..." << endl;

  try {

    TestHarness<RecursiveTest> recursivelTest;

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

  cout << "Recursive Test: PASSED" << endl;

  return 0;

}
