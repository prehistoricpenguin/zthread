/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2001, 2002 Eric Crahen, See LGPL.TXT for details
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include "Debug.h"

#include "zthread/Runnable.h"
#include "ThreadImpl.h"
#include "ThreadQueue.h"
#include "DeferredInterruptionScope.h"

#include <assert.h>

namespace ZThread {

TSS<ThreadImpl*> ThreadImpl::_threadMap;

class Launcher : public Runnable {

  ThreadImpl* x;
  ThreadImpl* y;
  const RunnableHandle& z;
  
public:
  
  Launcher(ThreadImpl* a, ThreadImpl* b, const RunnableHandle& c) : x(a), y(b), z(c) {}

  ~Launcher() throw() {}

  virtual void run() throw() {
    
    y->dispatch(x,y,z);
  
  }

};
 


/**
 * Initialize a new ThreadImpl object, giving it a default priority.
 */
ThreadImpl::ThreadImpl() 
  /* throw(Synchronization_Exception) */ : _joiner(0), _priority(Medium) {

  ZTDEBUG("Thread created.\n");

}

/**
 * Destroy the ThreadImpl 
 */
ThreadImpl::~ThreadImpl() 
  throw() {

  if(isActive()) {

    ZTDEBUG("You are destroying an executing thread!\n");
    assert(0);
    
  }

  ZTDEBUG("Thread destroyed.\n");

}

Monitor& ThreadImpl::getMonitor() {
  return _monitor;
}

void ThreadImpl::cancel() throw() { 
  _monitor.cancel(); 
}

bool ThreadImpl::interrupt() throw() { 
  return _monitor.interrupt(); 
}

bool ThreadImpl::isInterrupted() throw() {
  return _monitor.isInterrupted();
}

bool ThreadImpl::isCanceled() throw() {
  return _monitor.isCanceled();
}


Priority ThreadImpl::getPriority() const {
  return _priority;  
}

ThreadLocalMap& ThreadImpl::getThreadLocalMap() {
  return _localValues;
}


bool ThreadImpl::isReference() throw() {
  return _state.isReference();
}

/**
 * Join the thread, blocking the caller until it is interrupted or until
 * the thread represented by this object exits.
 *
 * Reference threads are not under the control of ZThreads and cannot be
 * joined.
 */
bool ThreadImpl::join(unsigned long timeout) 
  /* throw(Synchronization_Exception) */ {

  _monitor.acquire();
  
  // Make sure a thread is not trying to join() itself.
  if(ThreadOps::isCurrent(this))
    throw Deadlock_Exception("Cannot join self.");
  
  // A task needs to have been started or completed in order for a join()
  // to succeed.
  if(!(_state.isRunning() || _state.isComplete()))
    throw InvalidOp_Exception("Can not join this thread.");
  
  // If this thread is already being join()ed, throw an exception
  if(_joiner) 
    throw InvalidOp_Exception("Thread already being joined.");

  /*  
      
  TODO: Insert cyclic join check. I don't create situations with cyclic joins
  so its not a priority right now :)

  // Run through the threads involved in a join directly or indirectly
  // and detect any possible deadlock.
  for(ThreadImpl joiner = current(); joiner->_joiner != 0; joiner = _joiner)
    if(joiner->_joiner == joiner || joiner->_joiner->isJoinCyclic())
      throw Deadlock_Exception("Cyclic join.");
  */

  _joiner = current();
  
  // If the task has not completed, wait for completion
  if(!_state.isComplete()) {
    
    { // Move to the joining threads monitor
      
      Guard<Monitor> g(_joiner->getMonitor());
      _monitor.release();
      
      // Wait in the joining threads monitor
      switch(_joiner->_monitor.wait(timeout)) {
         
        case Monitor::TIMEDOUT:
          _joiner = 0;
          return false;
          
        case Monitor::INTERRUPTED:
          _joiner = 0;
          throw Interrupted_Exception();
          
        default:
          break;
          
      }
      
    } 
    
    // Reacquire the monitor for this thread.
    _monitor.acquire();
    
  }

  // Update the state.
  _state.setJoined();
  _joiner = 0;
  
  _monitor.release();
  
  // Join the native thread. This should not be a very long operation, the
  // thread will have exited at this point.
  if(!ThreadOps::join(this))
    throw Synchronization_Exception();

  return true;
  
}


/**
 * Translate the priority into a pthread value, and update the thread priority.
 *
 * This is not available on all platforms, and probably works differently 
 * the platforms that do support it. Pthreads does not have very portable 
 * priority support as far I am aware.
 *
 * If SCHED_OTHER is not supported priority values are still set but  
 * dont not actually in affect anything.
 *
 * @param prio PRIORITY value
 *
 * @exception Killed_Exception thrown by KILLED threads.
 * @exception InvalidOp_Exception thrown by IDLE, JOINING or JOINED threads.
 */
void ThreadImpl::setPriority(Priority p) {

  Guard<Monitor> g(_monitor);

  // Only set the native priority when the thread is running
  if(_state.isRunning())
    ThreadOps::setPriority(this, p);

  _priority = p;

}


/**
 * Test the state Monitor of this thread to determine if the thread
 * is an active thread created by zthreads.
 *
 * @return bool indicating the activity of the thread.
 */
bool ThreadImpl::isActive() throw() {

  Guard<Monitor> g(_monitor);
  
  // Daemon killed threads only reach the complete state
  // and are only destroyed when the ThreadQueue is poll()d
  if(_state.isDaemon())
    return _state.isRunning();

  // Regular threads are either unused or must reach the joined state
  return _state.isComplete() || _state.isRunning();
  
  
}

/**
 * Test the state Monitor of this thread to determine if the DAEMON
 * bit is set.
 *
 * @return bool indicating the status of the DAEMON bit when the method 
 * is called.
 */
bool ThreadImpl::isDaemon() throw() {

  Guard<Monitor> g(_monitor);
  return _state.isDaemon();
  
}

/**
 * Set the DAEMON status bit on the state Monitor for this Thread. A 
 * Thread must be in the IDLE state for this to be successful.
 */
void ThreadImpl::setDaemon(bool flag)
  /* throw(Synchronization_Exception) */ {
  
  Guard<Monitor> g(_monitor);

  // Only allow threads that have not yet been started become
  // daemons
  if(!_state.isIdle())
    throw InvalidOp_Exception();

  _state.setDaemon(flag);

}



/**
 * Get a reference to an implmenetation that maps to the current thread.
 * Accomplished by checking the TLS map. This will always return a valid
 * ThreadImpl instance.
 *
 * @return ThreadImpl* current implementation that maps to the
 * executing thread.
 */ 
ThreadImpl* ThreadImpl::current() throw() {

  // Get the ThreadImpl previously mapped onto the executing thread.
  ThreadImpl* impl = _threadMap.get();

  // Create a reference thread for any threads that have been 'discovered'
  // because they are not created by ZThreads.
  if(impl == 0) { 
   
    // Create a ThreadImpl to represent this thread. 
    impl = new ThreadImpl();
    impl->_state.setReference();
    
    ThreadOps::activate(impl);

    // Map a reference thread and insert it into the queue
    _threadMap.set(impl);

    ThreadQueue::instance()->insertReferenceThread(impl);

  }
  
  assert(impl != 0);
  return impl;

}

/**
 * Make current thread sleep for the given number of milliseconds.
 * This sleep can be interrupt()ed.
 * 
 * @param ms timeout for the sleep.
 *
 * @post the calling thread is blocked by waiting on the internal condition
 * variable. This can be signaled in the monitor of an interrupt
 */
void ThreadImpl::sleep(unsigned long ms) {

  // Make sleep()ing for 0 milliseconds equivalent to a yield.
  if(ms == 0) {

    yield();
    return;

  }

  // Get the monitor for the current thread
  Monitor& monitor = current()->getMonitor();
  
  // Acquire that threads Monitor with a Guard
  Guard<Monitor> g(monitor);

  for(;;) {

    switch(monitor.wait(ms)) {
        
      case Monitor::INTERRUPTED:
        throw Interrupted_Exception();

      default:
        return;

    }
    
  }

}


/**
 * Yield the current timeslice to another thread. 
 * If sched_yield() is available it is used. 
 * Otherwise, the state Monitor for this thread is used to simiulate a
 * yield by blocking for 1  millisecond, which should give the 
 * scheduler a chance to schedule another thread.
 */
void ThreadImpl::yield() throw() {

  // Try to yield with the native operation. If it fails, then 
  // simulate with a short wait() on the monitor.
  if(!ThreadOps::yield()) {
  
    // Get the monitor for the current thread
    Monitor& monitor = current()->getMonitor();

    // Attempt a wait().
    Guard<Monitor> g(monitor);    
    monitor.wait(1);

  }

}

void ThreadImpl::run(const RunnableHandle& task) {

  Guard<Monitor> g1(_monitor);

  // A Thread must be idle in order to be eligable to run a task.
  if(!_state.isIdle())
    throw InvalidOp_Exception("Thread is not idle.");
  
  _state.setRunning();
  
  // Spawn a new thread, blocking the parent (current) thread until
  // the child starts.

  ThreadImpl* parent = current();
  Launcher launch(parent, this, task);

  // Attempt to start the child thread
  Guard<Monitor> g2(parent->_monitor);

  if(!spawn(&launch)) {

    // Return to the idle state & report the error if it doesn't work out.
    _state.setIdle();    
    throw Synchronization_Exception();    


  }

  // Wait, uninterruptably, for the child's signal. The parent thread
  // still can be interrupted and killed; it just won't take effect 
  // until the child has started.

  Guard<Monitor, DeferredInterruptionScope> g3(parent->_monitor);

  if(parent->_monitor.wait() != Monitor::SIGNALED) {
    assert(0);
  }
  

}

void ThreadImpl::dispatch(ThreadImpl* parent, ThreadImpl* impl, const RunnableHandle& h) {

  RunnableHandle task(h);

  // Map the implementation object onto the running thread.
  _threadMap.set(impl);

  // Update the priority of the thread
  if(parent->_state.isReference()) 
    ThreadOps::setPriority(impl, 
                           parent->_state.isReference() ? impl->_priority : parent->_priority);

  // Copy values to inherit, propogate after the parent is released.
  ThreadLocalMap inheritedLocals(parent->_localValues);

  // Update the reference count on a ThreadImpl before the 'Thread' 
  // that owns it can go out of scope (by signaling the parent)
  bool isDaemon = impl->_state.isDaemon();
  if(isDaemon) {

    impl->addReference();
    ThreadQueue::instance()->poll();

  }

  impl->_monitor.clear(Status::INTERRUPTED);

  // Wake the parent once the thread is setup
  parent->_monitor.notify();

  // Propogate the inherited values
  impl->_localValues = inheritedLocals;

  ZTDEBUG("Thread starting...\n");

  try {
    
    task->run();

  } catch(...) {

    // Result of running a task that threw an exception.
    ZTDEBUG("The task has thrown an unhandled exception\n");
    
  }

  ZTDEBUG("Thread joining...\n"); 

  { // Update the state of the thread

    Guard<Monitor> g(impl->_monitor);
    impl->_state.setComplete();

    // Wake the joiner
    ThreadImpl* joiner = impl->_joiner;
    if(joiner) 
      joiner->_monitor.notify();

  }

  ZTDEBUG("Thread exiting...\n"); 

  // Cleanup the ThreadLocals
  impl->_localValues.clear();

  // Add the thread to queue 
  if(isDaemon)
    ThreadQueue::instance()->insert(impl);

}


} // namespace ZThread
