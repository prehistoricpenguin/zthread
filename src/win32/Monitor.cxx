#include "Monitor.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace ZThread;

Monitor::Monitor() : _owner(0), _waiting(false) {
  
  _handle = ::CreateEvent(0, TRUE, FALSE, 0);  
  if(_handle == NULL) {
    assert(0);
  }

}
 
Monitor::~Monitor() {
  
  assert(!_waiting);

  ::CloseHandle(_handle);  

}

Monitor::STATE Monitor::wait(unsigned long ms) {
  
  // Update the owner on first use. The owner will not change, each
  // thread waits only on a single Monitor and a Monitor is never
  // shared
  if(_owner == 0)
    _owner = ::GetCurrentThreadId();

  STATE state(INVALID);
  
  // Serialize access to the state of the Monitor
  // and test the state to determine if a wait is needed.
  _waitLock.acquire();

  if(pending(ANYTHING)) {
    
    // Return without waiting when possible
    state = next();

    _waitLock.release();
    return state;

  }
  // Unlock the external lock if a wait() is probably needed. 
  // Access to the state is still serial.
  _lock.release();

  // Wait for a transition in the state that is of interest, this
  // allows waits to exclude certain flags (e.g. INTERRUPTED) 
  // for a single wait() w/o actually discarding those flags -
  // they will remain set until a wait interested in those flags
  // occurs.
  //  if(!currentState(interest)) {

  // Wait, ignoring signals
  _waiting = true;
  int status = 0;

  // Block until the event is set.  
  _waitLock.release();

  // The event is manual reset so this lack of atmoicity will not
  // be an issue

  DWORD dwResult = 
    ::WaitForSingleObject(_handle, ((ms == 0) ? INFINITE : (DWORD)ms));

  // Reacquire serialized access to the state
  _waitLock.acquire();

  // Awaken only when the event is set or the timeout expired
  assert(dwResult == WAIT_OBJECT_0 || dwResult == WAIT_TIMEOUT);

  if(dwResult == WAIT_TIMEOUT)
    push(TIMEDOUT);
  
  // Get the next available STATE
  state = next();  
  _waiting = false;  
    
  // Reaquire the external lock
  _lock.acquire();

  ::ResetEvent(_handle);

  // Acquire the internal lock & release the external lock
  _waitLock.release();
  
  return state;

}


bool Monitor::interrupt() {

  // Serialize access to the state
  _waitLock.acquire();
  
  bool wasInterruptable = !pending(INTERRUPTED);
  bool hadWaiter = _waiting;
 
  if(wasInterruptable) {
 
    // Update the state & wake the waiter if there is one
    push(INTERRUPTED);
    
    if(hadWaiter) 
      if(::SetEvent(_handle) == NULL) {
        assert(0);
      }
    
  }

  _waitLock.release();
  
#if !defined(ZTHREAD_DISABLE_INTERRUPT)
  
  // If a thread was not blocked, and the thread is not this thread,
  // raise a signal.
  if(!hadWaiter && !(_owner == ::GetCurrentThreadId())) {
    
/*    HANDLE hInterrupt = (HANDLE)Thread::interruptKey().get();

    if(hInterrupt != 0x00) // Signal the interrupt
      if(::SetEvent(hInterrupt) == NULL) {
        assert(0);
      }
*/
  }

#endif
  
  return wasInterruptable;

}

bool Monitor::isInterrupted() {

  // Serialize access to the state
  _waitLock.acquire();

  bool wasInterrupted = pending(INTERRUPTED);
  clear(INTERRUPTED);
    
  _waitLock.release();

  return wasInterrupted;

}


bool Monitor::notify() {

  // Serialize access to the state
  _waitLock.acquire();

  bool wasNotifyable = !pending(INTERRUPTED);
 
  if(wasNotifyable) {
  
    // Set the flag and wake the waiter if there
    // is one
    push(SIGNALED);
    
    // If there is a waiter then send the signal.
    if(_waiting) 
      if(::SetEvent(_handle) == NULL) {
        assert(0);
      }

  }

  _waitLock.release();

  return wasNotifyable;

}


bool Monitor::cancel() {

  // Serialize access to the state
  _waitLock.acquire();

  bool wasInterrupted = !pending(INTERRUPTED);
  bool hadWaiter = _waiting;
 
  push(CANCELED);

  if(wasInterrupted) {
 
    // Update the state & wake the waiter if there is one
    push(INTERRUPTED);
    
    // If there is a waiter then send the signal.
    if(_waiting) 
      if(::SetEvent(_handle) == NULL) {
        assert(0);
      }
    
  }

  _waitLock.release();

  return wasInterrupted;

}

bool Monitor::isCanceled() {

  // Serialize access to the state
  _waitLock.acquire();

  bool wasCanceled = examine(CANCELED);
    
  if(_owner == ::GetCurrentThreadId())
    clear(INTERRUPTED);

  _waitLock.release();

  return wasCanceled;

}

