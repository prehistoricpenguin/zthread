#include "zthread/Guard.h"

#include <assert.h>
#include <iostream>
using namespace std;
using namespace ZThread;

// This tests the semantics of the Guard class and its policies.
class VerboseLockable {
  
  int _balance;
  bool _locked;

public:

  VerboseLockable() : _balance(0), _locked(false) {}
  ~VerboseLockable() { assert(_balance == 0); }

  void acquire() {
    cout << "acquire()" << endl;
    _balance++;
  }

  bool tryAcquire(unsigned long) {
    cout << "acquire()" << endl;
    _balance++;
    return true;
  }


  void release() {
    cout << "release()" << endl;
    assert(--_balance >= 0);
  }

  bool isLocked() const { return _locked; }
 
};



template <class T, class LockType = VerboseLockable>
class GuardedObject {
  
  LockType _lock;
  T _p;

  class DeferredLockedScope {
  public:
    
    /**
     * Action taken when a locked scoped is being constructed to
     * share a LockType.
     *
     *
     * @param lock1 LockType1& is the LockHolder that holds the desired lock
     * @param lock2 LockType1& is the LockHolder that wants to share
     */
    template <class LockType1, class LockType2>
    static void shareScope(LockHolder<LockType1>& l1, LockHolder<LockType2>& l2) {
      
      l1.disable();
      l2.getLock().acquire();
      
    }
    
    /**
     * Acquire the given LockHolder.
     *
     * @param lock LockType& is a type of LockHolder.
     */
    template <class LockType1>
    static void createScope(LockHolder<LockType1>& l) {
      // Don't acquire the lock when scope the Guard is created
    }
    
    /**
     * Release the given LockHolder.
     *
     * @param lock LockType& is a type of LockHolder.
     */
    template <class LockType1>
    static void destroyScope(LockHolder<LockType1>& l) {
      l.getLock().release();
    }
    
  };
  
  class Proxy : Guard<LockType, DeferredLockedScope> {
    
    T& _object;
    
  public:
    
    Proxy(LockType& lock, T& object) : 
      Guard<LockType, DeferredLockedScope>(lock), _object(object) { }
  
    T* operator->() {
      return &_object;
    }

  };

public:

  Proxy operator->() {
    
    Proxy p(_lock, _p);
    return p;
    
  }


};

class A {
public:
  int x;
};

int main() {

  // Create a Guard that locks for this scope
  VerboseLockable lock0, lock1;
  Guard<VerboseLockable, LockedScope> g0(lock0);

  // Share the lock with a nested scope
  { 
    
    Guard<VerboseLockable, UnlockedScope> g1(g0);
  
  }

  // Policies that share the lock can loop safely
  // (can nest multiple scopes on the same level)
  for(int i = 0; i < 10; i++) {
 
    Guard<VerboseLockable, UnlockedScope> g1(g0);
 
  }

  // Overlapped guards have the potential to end the 
  // effective scope of a lock early.
    
  Guard<VerboseLockable, OverlappedScope> g1(g0, lock1);
  
  // Here g0's effective scope has ended, and g1's has begun

  cout << "OK" << endl;

  GuardedObject<A> gg;
  gg->x = 9;

  cout << "OK" << endl;

  gg->x = 92;

  cout << gg->x << endl;

}
