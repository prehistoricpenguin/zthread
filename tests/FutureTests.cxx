#include "zthread/FastMutex.h"
#include "zthread/Future.h"
#include "zthread/SynchronousExecutor.h"
#include "zthread/PoolExecutor.h"

#include <iostream>
using namespace std;
using namespace ZThread;

SynchronousExecutor<FastMutex> executor;

class MyCallable : public Future<int>::Callable {
public:

  virtual ~MyCallable() throw() { }

  virtual void call(Future<int>::ResultRef& result) {

    
    cout << "Hi!" << endl;
    Thread::sleep(2000);
    
    int n = 100;
    result->setResult(n);

  }

};

class ThrowingCallable : public Future<int>::Callable {
public:

  virtual ~ThrowingCallable() throw() { }

  virtual void call(Future<int>::ResultRef& result) {

    try {

      cout << "I'll throw an exception soon!" << endl;
      Thread::sleep(2000);
      
      throw string("oh no!");

    } catch(string& s) {
      result->setException(s);
    }

  }

};

int main() {

  try {

    Future<int> f1( CallablePtr(new MyCallable), executor);
    cout << "Result: " << f1.getResult() << endl;


    try {

      Future<int> f2( CallablePtr(new ThrowingCallable), executor);
      f2.getResult();
      
    } catch(...) {

      cout << "Future threw an exception!" << endl;

    }

  } catch(...) { 
    assert(0); // This should not occur
  } 

}


