/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2000-2002, Eric Crahen, See LGPL.TXT for details
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

#ifndef __ZTBARRIER_H__
#define __ZTBARRIER_H__

#include "zthread/Condition.h"
#include "zthread/Guard.h"
#include "zthread/Waitable.h"
#include "zthread/Runnable.h"

namespace ZThread {

/**
 * @class Barrier
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T08:11:27-0400> 
 * @version 2.2.1
 *
 * A Barrier is a Waitable object that serves as synchronization points for 
 * a set of threads. A Barrier is constructed for a fixed number (<i>N</i>) of threads.
 * Threads attempting to wait() on a Barrier (<i> 1 - N</i>) will block until the <i>N</i>th
 * thread arrives. The <i>N</i>th thread will awaken all the the others.
 * 
 * An optional Runnable command may be associated with the Barrier. This will be run()
 * when the <i>N</i>th thread arrives and Barrier is not broken.
 *
 * <b>Error Checking</b>
 *
 * A Barrier uses an all-or-nothing. All threads invovled must successfully 
 * meet at Barrier. If any one of those threads leaves before all the threads 
 * have (as the result of an error or exception) then all threads present at 
 * the Barrier will throw BrokenBarrier_Exception.
 *
 * A broken Barrier will cause all threads attempting to wait() on it to 
 * throw a BrokenBarrier_Exception.
 *
 * A Barrier will remain 'broken', until it is manually reset().
 */
template <unsigned int Count, class LockType>
class Barrier : public Waitable, private NonCopyable {

  //! Broken flag
  bool _broken;
  //! Thread count
  unsigned int _count;
  //! Wait generation
  unsigned int _generation;
  //! Serialize access
  LockType _lock;
  //! Signaled when all thread arrive
  Condition _arrived;
  //! Command to run when all the thread arrive
  Runnable* _command;

public:

  //! Create a Barrier
  Barrier() /* throw(Synchronization_Exception) */ : 
    _broken(false), _count(Count), _generation(0), _arrived(_lock), _command(0) { }

  /**
   * Create a Barrier that executes the given Runnable object when all
   * threads arrive.
   *
   * @param command Runnable to associate with this Barrier
   */
  Barrier(Runnable* command) /* throw(Synchronization_Exception) */ : 
    _broken(false), _count(Count), _generation(0), _arrived(_lock), _command(command) { }

  //! Destroy this Barrier
  virtual ~Barrier() {}

  /**
   * Enter barrier and wait for the other threads. This can block for an indefinite
   * amount of time.
   *
   * @exception BrokenBarrier_Exception thrown when any of the thread that has 
   * left the Barrier as a result of an error or exception. 
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   *
   * @see Waitable::wait()
   *
   * @post All threads have successfully arrived at the Barrier if no exception
   * is thrown
   */
  virtual void wait() 
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    
    if(_broken) 
      throw BrokenBarrier_Exception();

    // Break the barrier if an arriving thread is interrupted
    if(Thread::interrupted()) {
      
      // Release the other waiter, propogate the exception
      _arrived.broadcast();
      _broken = true;

      throw Interrupted_Exception();

    }
    
    if(--_count == 0) {
      
      // Wake the other threads if this was the last  
      // arriving thread
      _arrived.broadcast();
      
      // Try to run the associated task, if it throws then 
      // break the barrier and propogate the exception
      try {

        if(_command)
          _command->run();

        _generation++;

      } catch(Synchronization_Exception&) {

        _broken = true;
        throw;

      } catch(...) { assert(0); }

    } else {

      int myGeneration = _generation;

      try {

        // Wait for the other threads to arrive
        _arrived.wait();

      } catch(Interrupted_Exception&) {

        // Its possible for a thread to be interrupted before the 
        // last thread arrives. If the interrupted thread hasn't 
        // resumed, then just propgate the interruption

        if(myGeneration != _generation)
          Thread::current().interrupt();

        else _broken = true;

      } catch(Synchronization_Exception&) {

        // Break the barrier and propogate the exception
        _broken = true;
        throw;

      }
      
      // If the thread woke because it was notified by the thread
      // that broke the barrier, throw.
      if(_broken) 
        throw BrokenBarrier_Exception();
   
    } 

  }

  /**
   * Enter barrier and wait for the other threads. This can block for an definite
   * amount of time, which is, at most, the length of the timeout.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   *
   * @return bool true if the Barrier is successful before the time expires.
   *
   * @exception BrokenBarrier_Exception thrown when any of the thread that has 
   * left the Barrier as a result of an error or exception. 
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   *
   * @see Waitable::wait(unsigned long)
   *
   * @post All threads have successfully arrived at the Barrier if no exception
   * is thrown
   */
  virtual bool wait(unsigned long timeout) 
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    
    if(_broken) 
      throw BrokenBarrier_Exception();

    // Break the barrier if an arriving thread is interrupted
    if(Thread::interrupted()) {
      
      // Release the other waiter, propogate the exception
      _arrived.broadcast();
      _broken = true;

      throw Interrupted_Exception();

    }

    
    if(--_count == 0) {
      
      // Wake the other threads if this was the last  
      // arriving thread
      _arrived.broadcast();
      
      // Try to run the associated task, if it throws then 
      // break the barrier and propogate the exception
      try {

        if(_command)
          _command->run();

        _generation++;

      } catch(Synchronization_Exception&) {

        _broken = true;
        throw;

      } catch(...) { assert(0); }

    } else {

      int myGeneration = _generation;

      try {

        // Wait for the other threads to arrive
        if(!_arrived.wait(timeout))
          _broken = true;

      } catch(Interrupted_Exception&) {

        // Its possible for a thread to be interrupted before the 
        // last thread arrives. If the interrupted thread hasn't 
        // resumed, then just propgate the interruption

        if(myGeneration != _generation)
          Thread::current().interrupt();

        else _broken = true;

      } catch(Synchronization_Exception&) {

        // Break the barrier and propogate the exception
        _broken = true;
        throw;

      }
      
      // If the thread woke because it was notified by the thread
      // that broke the barrier, throw.
      if(_broken) 
        throw BrokenBarrier_Exception();
   
    } 

    return true;

  }

  /**
   * Break the Barrier. 
   *
   * @exception Synchronization_Exception thrown if there is an error 
   * performing this operation
   *
   * @post the Barrier is Broken, all waiting threads will throw the 
   * BrokenBarrier_Exception
   */
  void shatter() 
    /* throw(Synchronization_Exception) */ {
     
    Guard<LockType> g(_lock);   

    _broken = true;
    _arrived.broadcast();

  }

  /**
   * Reset the Barrier. 
   *
   * @exception Synchronization_Exception thrown if there is an error 
   * performing this operation
   *
   * @post the Barrier is no longer Broken and can be used again.
   */
  void reset() 
    /* throw(Synchronization_Exception) */ {
     
    Guard<LockType> g(_lock);   

    _broken = false;
    _generation++;
    _count = Count;
      
  }

};

  
} // namespace ZThread

#endif // __ZTBARRIER_H__
