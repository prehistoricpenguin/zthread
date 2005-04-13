/*
 * Copyright (c) 2005, Eric Crahen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "Debug.h"

#include "zthread/Runnable.h"
#include "ThreadImpl.h"
#include "ThreadQueue.h"
#include "DeferredInterruptionScope.h"

#include <assert.h>

namespace ZThread {

  TSS<ThreadImpl*> ThreadImpl::_threadMap;

  namespace {
  
    class Launcher : public Runnable {
      
      ThreadImpl* _impl;
      Task _task;
      
    public:
      
      Launcher(ThreadImpl* impl, const Task& task) : _impl(impl), _task(task) {}
      
      void run() {      
        _impl->dispatch(_task);
        delete this;
      }
            
    };
    
  }

  ThreadImpl::ThreadImpl() 
    : _state(State::REFERENCE), _priority(Medium), _autoCancel(false) {
    
    ZTDEBUG("Reference thread created.\n");
    
  }

  ThreadImpl::ThreadImpl(const Task& task, bool autoCancel) 
    : _state(State::IDLE), _priority(Medium), _autoCancel(autoCancel) {
    
    ZTDEBUG("User thread created.\n");

    start(task);

  }
  
  
  ThreadImpl::~ThreadImpl() {
    
    _tls.clear();

    if(isActive()) {
      
      ZTDEBUG("You are destroying an executing thread!\n");
      abort();
      
    }
    
    ZTDEBUG("Thread destroyed.\n");
    
  }
  
  Monitor& ThreadImpl::getMonitor() {
    return _monitor;
  }
  
  void ThreadImpl::cancel(bool autoCancel) {
    if(!autoCancel || _autoCancel) 
      _monitor.cancel(); 
  }
  
  bool ThreadImpl::interrupt() { 
    return _monitor.interrupt(); 
  }
  
  bool ThreadImpl::isInterrupted() {
    return _monitor.isInterrupted();
  }
  
  bool ThreadImpl::isCanceled() {
    return _monitor.isCanceled();
  }

  Priority ThreadImpl::getPriority() const {
    return _priority;  
  }
  
 

  bool ThreadImpl::isReference() {
    return _state.isReference();
  }
  
  /**
   * Join the thread, blocking the caller until it is interrupted or until
   * the thread represented by this object exits.
   *
   * Reference threads are not under the control of ZThreads and cannot be
   * joined.
   */
  bool ThreadImpl::join(unsigned long timeout) {
     
    // Serial access to this ThreadImpl's state
    Guard<Monitor> g1(_monitor);
  
    // Make sure a thread is not trying to join() itself.
    if(ThreadOps::isCurrent(this))
      throw Deadlock_Exception("Cannot join self.");
    
    // Reference threads can't be joined. 
    if(_state.isReference())
      throw InvalidOp_Exception("Can not join this thread.");
    
    /*  
        
    TODO: Insert cyclic join check. 
    
    */
    
    // If the task has not completed yet, wait for completion
    if(!_state.isJoined()) {
      
      // Add the current thread to the joiner list
      ThreadImpl* impl = current();
      _joiners.push_back(impl);

      Monitor::STATE result;

      { // Release this ThreadImpl's lock while the joiner sleeps

        _monitor.release();  
        Guard<Monitor> g3(impl->getMonitor());
 
        result = impl->_monitor.wait(timeout);
 
        _monitor.acquire();
         
      }
       
      // Update the joiner list
      List::iterator i = std::find(_joiners.begin(), _joiners.end(), impl);
      if(i != _joiners.end())
        _joiners.erase(i);
      
      
      switch(result) {
        
        case Monitor::TIMEDOUT:
          return false;
          
        case Monitor::INTERRUPTED:
          throw Interrupted_Exception();
          
        default:
          break;
          
      }
      
    }

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
    if(_state.isStarting() || _state.isRunning())
      ThreadOps::setPriority(this, p);
    
    _priority = p;
    
  }
  
  
  /**
   * Test the state Monitor of this thread to determine if the thread
   * is an active thread created by zthreads.
   *
   * @return bool indicating the activity of the thread.
   */
  bool ThreadImpl::isActive() {
    
    Guard<Monitor> g(_monitor);
    return _state.isStarting() || _state.isRunning(); 
    
  }
  
  
  /**
   * Get a reference to an implmenetation that maps to the current thread.
   * Accomplished by checking the TLS map. This will always return a valid
   * ThreadImpl instance.
   *
   * @return ThreadImpl* current implementation that maps to the
   * executing thread.
   */ 
  ThreadImpl* ThreadImpl::current() {
    
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
  void ThreadImpl::yield() {
    
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

  void ThreadImpl::start(const Task& task) {

    Guard<Monitor> g1(_monitor);

    // A Thread must be idle in order to be eligable to run a task.
    if(!_state.isIdle())
      throw InvalidOp_Exception("Thread is not idle.");
  
    _state.setStarting();
  
    // Spawn a new thread, blocking the parent (current) thread until
    // the child starts.

    ThreadImpl* parent = current();
    Launcher* launcher = new Launcher(this, task);

    // Inherit ThreadLocal values from the parent
    typedef ThreadLocalMap::const_iterator It;

    for(It i = parent->getThreadLocalMap().begin(); i != parent->getThreadLocalMap().end(); ++i) 
      if( (i->second)->isInheritable() )
        getThreadLocalMap()[ i->first ] = (i->second)->clone();

    if(!spawn(launcher)) {

      // Return to the idle state & report the error if it doesn't work out.
      delete launcher;
      _state.setIdle();    

      throw Synchronization_Exception("Could not spawn new thread");

    }

    // Update the priority of this thread
    ThreadOps::setPriority(this, parent->_state.isReference() ? _priority : parent->_priority);
    
    // Insert a user-thread mapping if the launch is successful
    this->addReference();
    ThreadQueue::instance()->insertUserThread(this);

  }

  /**
   * To create a child thread we need the following from the parent,
   *
   * Priority
   * Inherited TSS values
   *
   * 
   */
  void ThreadImpl::dispatch(Task task) {

    // Map the implementation object onto the running thread.
    _threadMap.set(this);
    

/*
    // Update the reference count on a ThreadImpl before the 'Thread' 
    // that owns it can go out of scope (by signaling the parent)
    impl->addReference();
 */
/*
*/
  
    // Once the state is updated this thread will behave normally. There is a brief
    // period of time between the parent thread spawning its child and this point where
    // the state is STARTING. Child threads that are STARTING aren't quite ready yet.
    _state.setRunning();

/*
    // Wake the parent once the thread is setup
    parent->_monitor.notify();
*/
    ZTDEBUG("Thread starting...\n");

    try {
    
      task->run();

    } catch(...) {

      // Result of running a task that threw an exception.
      ZTDEBUG("The task has thrown an unhandled exception\n");
    
    }

    ZTDEBUG("Thread joining...\n"); 
    
    { // Update the state of the thread
    
      Guard<Monitor> g(_monitor);
      _state.setJoined();
    
      // Wake the joiners that will be easy to join first
      List::iterator i;
      for(i = _joiners.begin(); i != _joiners.end();) {
      
        ThreadImpl* joiner = *i;
        Monitor& m = joiner->getMonitor();

        if(m.tryAcquire()) {
  
          m.notify();
          m.release();   
       
          i = _joiners.erase(i);

        } else
          ++i;

      } 

      // Wake the joiners that might take a while next
      for(i = _joiners.begin(); i != _joiners.end(); ++i) {
      
        ThreadImpl* joiner = *i;
        Monitor& m = joiner->getMonitor();

        m.acquire();
        m.notify();
        m.release();   
       
      }
      
    }

    ZTDEBUG("Thread exiting...\n"); 

    // Insert a pending-thread mapping, allowing the resources to be reclaimed 
    ThreadQueue::instance()->insertPendingThread(this);

    // Cleanup ThreadLocal values
    getThreadLocalMap().clear();

    // Update the reference count allowing it to be destroyed 
    delReference();

  }


} // namespace ZThread
