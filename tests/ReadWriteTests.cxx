#include "zthread/FastMutex.h"
#include "zthread/FairReadWriteLock.h"
#include "zthread/BiasedReadWriteLock.h"

#include <iostream>
using namespace std;
using namespace ZThread;

int main() {

  try {
    
    BiasedReadWriteLock rwlock;

    rwlock.getReadLock().acquire();
    rwlock.getWriteLock().tryAcquire(100);

    rwlock.getReadLock().release();

  } catch(Synchronization_Exception& e) { 
    cerr << e.what() << endl;
    assert(0); // This should not occur
  } 

}


