//: c11:Bridge.cxx
#include "zthread/Guard.h"
#include "zthread/CountingSemaphore.h"
#include "zthread/Mutex.h"
#include "zthread/Semaphore.h"
#include "zthread/Singleton.h"
#include "zthread/Thread.h"
#include <iostream>

using namespace ZThread;
using namespace std;

class Display {
  Mutex lock;
public:

  void readerAcquired() {
    Guard<Mutex> g(lock);
    cout << "read lock acquired " << endl;
  }

  void readerReleased() {
    Guard<Mutex> g(lock);
    cout << "read lock released" << endl;
  }

  void writerAcquired() {
    Guard<Mutex> g(lock);
    cout << "write lock acquired" << endl;
  }

  void writerReleased() {
    Guard<Mutex> g(lock);
    cout << "write lock released" << endl;
  }

};

class ReadWriteLock {

  Semaphore lock;
  CountingSemaphore read, write;

  int nr_active, nr_waiting;
  int nw_active, nw_waiting;

  int capacity;

public:

  ReadWriteLock(int n) : lock(1) {

    nr_active = nw_active = 0;
    nr_waiting =  nw_waiting = 0;
    capacity = n;

  }

  void acquireRead() {

    {

      Guard<Semaphore> g(lock);

      // No wait if there are no writers and we aren't at capacity
      if((nw_active + nw_waiting == 0) && nr_active < capacity) {

        nr_active++;
        read.post();

      } else
        nr_waiting++;

    }

    read.wait();

  }

  void releaseRead() {

    Guard<Semaphore> g(lock);

    nr_active--;

    // If this is the last reader, wake any waiting writers
    if(nr_active < 1 && nw_waiting > 0) {

      for(int count = 0; nw_waiting > 0 && count < capacity;)  {

        // Wake a writer
        write.post();

        nw_active++;
        nw_waiting--;

        count++;
      }

    } else if(nw_waiting == 0 && nr_waiting > 0) {

      // Wake a reader
      read.post();

      nr_active++;
      nr_waiting--;

    }

  }

  void acquireWrite() {

    {

      Guard<Semaphore> g(lock);

      // No wait if there are no readers and we aren't at capacity
      if((nr_active + nr_waiting == 0) && nw_active < capacity) {

        nw_active++;
        write.post();

      } else
        nw_waiting++;

    }

    write.wait();

  }

  void releaseWrite() {

    Guard<Semaphore> g(lock);

    nw_active--;

    // If this is the last writer, wake any waiting readers
    if(nw_active < 1 && nr_waiting > 0) {

      for(int count = 0; nr_waiting > 0 && count < capacity;)  {

        // Wake a reader
        read.post();

        nr_active++;
        nr_waiting--;

        count++;
      }

    } else if(nr_waiting == 0 && nw_waiting > 0) {

      // Wake a writer
      write.post();

      nw_active++;
      nw_waiting--;

    }

  }

};

class Reader : public Runnable {

  ReadWriteLock& lock;

public:

  Reader(ReadWriteLock& rwlock) : lock(rwlock) {}
  virtual ~Reader() throw() {}

  virtual void run() throw() {

    lock.acquireRead();
    Singleton<Display>::instance()->readerAcquired();

    Thread::sleep(2000);
    lock.releaseRead();

    Singleton<Display>::instance()->readerReleased();

  }

};


class Writer : public Runnable {

  ReadWriteLock& lock;

public:

  Writer(ReadWriteLock& rwlock) : lock(rwlock) {}
  virtual ~Writer() throw() {}

  virtual void run() throw() {

    lock.acquireWrite();
    Singleton<Display>::instance()->writerAcquired();

    Thread::sleep(1000);
    lock.releaseWrite();

    Singleton<Display>::instance()->writerReleased();

  }

};

int main() {

  try {

    ReadWriteLock lock(3);

    Handle<Reader> r(new Reader(lock));
    Handle<Writer> w(new Writer(lock));

    Thread t[10];

    for(int i = 0; i < 5; i++)
      t[i].run(r);

    for(int i = 5; i < 10; i++)
      t[i].run(w);

    for(int i = 0; i < 10; i++)
      t[i].join();

  } catch(Synchronization_Exception& e) {
    cerr << e.what() << endl;
  }

} ///:~
