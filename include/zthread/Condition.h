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

#ifndef __ZTCONDITION_H__
#define __ZTCONDITION_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"
#include "zthread/Waitable.h"

namespace ZThread {
  
class FifoConditionImpl;

/**
 * @class Condition
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T08:10:20-0400>
 * @version 2.2.1
 *
 * A Condition is a Waitable object used to block a thread until a particular 
 * condition is met. A Condition object is always used in conjunction with Lockable 
 * object. This object should be a FastMutex, Mutex, PriorityMutex or PriorityInheritanceMutex.
 *
 * Condition objects are reminicent of POSIX condition variables in several ways but 
 * are slightly different. 
 * 
 * A Condition is <i>not</i> subject to spurious wakeup.
 *
 * Like all Waitable objects, Conditions are sensative to Thread::interupt() which can
 * be used to prematurely end a wait().
 *
 * @see Thread::interupt()
 *
 * Before a wait() is performed on a Condition, the associated Lockable object should
 * have been acquire()ed. When the wait() begins, that Lockable object is release()d 
 * (wait() will atomically begin the wait and unlock the Lockable). 
 *
 * A thread blocked by wait() will remain so until an exception occurs, or until 
 * the thread awakened by a signal() or broadcast(). When the thread resumes execution,
 * the associated Lockable is acquire()d before wait() returns.
 *
 * <b>Scheduling</b>
 *
 * Threads blocked on a Condition are resumed in FIFO order.
 */
class ZTHREAD_API Condition : public Waitable, private NonCopyable {

  FifoConditionImpl* _impl;
  
 public:
  
  /**
   * Create a new condition variable associated with the given Lockable object.
   *
   * @param l - Lockable object to associate with this Condition object.
   *
   * @exception Synchronization_Exception thrown if there is some error 
   * creating the Condition object
   */
  Condition(Lockable& l) 
    /* throw(Synchronization_Exception) */;

  //! Destroy Condition object
  virtual ~Condition()
    throw(); 
  
  /**
   * Wake <i>ONE</i> threads wait()ing on this Condition.
   *
   * @post a wait()ing thread, if any exists, will be awakened.
   *
   * @exception Synchronization_Exception thrown if there is an error 
   * performing this operation
   */
  void signal() 
    /* throw(Synchronization_Exception) */;

  /**
   * Wake <i>ALL</i> threads wait()ing on this Condition.
   *
   * @post all wait()ing threads, if any exist, will be awakened.
   *
   * @exception Synchronization_Exception thrown if there is an error 
   * performing this operation
   */
  void broadcast() 
    /* throw(Synchronization_Exception) */;


  /**
   * Wait for this Condition object to be signal()ed. The thread calling this method
   * will block until the Condition is signal()ed or broadcast() to. 
   *
   * This operation atomically release()s the associated Lockable and blocks the calling thread.
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   *
   * @pre The thread calling this method must have first acquire()d the associated 
   * Lockable object. 
   *
   * @post A thread that has resumed execution without exception (because of a signal(), 
   * broadcast() or exception) will have acquire()d the associated Lockable object before
   * returning from a wait().
   *
   * @see Waitable::wait()
   */
  virtual void wait() 
    /* throw(Synchronization_Exception) */;   

  /**
   * Wait for this Condition object to be signal()ed. The thread calling this method
   * will block until the Condition is signal()ed, broadcast() to or until the timeout 
   * expires.
   *
   * This operation atomically release()s the associated Lockable and blocks the calling thread.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   *
   * @return true if the Condition was signal()ed before the timeout expired, otherwise
   * false.
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   *
   * @pre The thread calling this method must have first acquire()d the associated 
   * Lockable object. 
   *
   * @post A thread that has resumed execution without exception (because of a signal(), 
   * broadcast() or exception) will have acquire()d the associated Lockable object before
   * returning from a wait().
   *
   * @see Waitable::wait(timeout)
   */
  virtual bool wait(unsigned long timeout) 
    /* throw(Synchronization_Exception) */;

  
};
  
} // namespace ZThread

#endif // __ZTCONDITION_H__
