#include <zthread/Thread.h>
#include <zthread/ThreadLocal.h>
#include <zthread/InheritableThreadLocal.h>
using namespace ZThread;

#include <iostream>
using namespace std;


class MyThreadLocal : public InheritableThreadLocal<int> {
public:  
  
  virtual ~MyThreadLocal() throw () {}

protected:

  virtual void* initialValue() const throw() {

    cout << "First access by this thread; setting initial value to 2." << endl;
    return (void*)2;

  }

  virtual void* childValue(void* parentValue) const throw() {

    int val = (int)parentValue;
    cout << "Value propogating from parent thread: " << val++ << endl;
    return (void*)val;

  }

};


static MyThreadLocal tls;

class MyChild : public Thread {
public:

  MyChild() {}
  virtual ~MyChild() throw() {}

  virtual void run() throw() {
    cout << tls.get() << endl;
  }

};

static MyThreadLocal anotherTls;

class AnotherChild : public Thread {
public:

  AnotherChild() {}
  virtual ~AnotherChild() throw() {}

  virtual void run() throw() {
    cout << tls.get() << endl;
    cout << anotherTls.get() << endl;
  }

};


int main() {

  MyChild child;

  child.start();
  child.join();

  tls.set(5);  
  
  cout << anotherTls.get() << endl;

  AnotherChild another;

  another.start();
  another.join();


#ifdef WIN32
  cout << "Press a key" << endl;
  getchar(); // Otherwise the console window will close to quickly
#endif

  return 0;

}
