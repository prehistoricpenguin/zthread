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

#ifndef __ZTPRIORITYCONDITION_H__
#define __ZTPRIORITYCONDITION_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"
#include "zthread/Waitable.h"

namespace ZThread {
  
class PriorityConditionImpl;

/**
 * @class PriorityCondition
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-02T08:11:02-0400>
 * @version 2.2.1
 *
 * A PriorityCondition is a Condition that is sensative to thread priority.
 *
 * @see Condition
 *
 * <b>Scheduling</b>
 *
 * Threads blocked on a PriorityCondition are resumed in priority order, highest priority
 * first
 */
class ZTHREAD_API PriorityCondition : public Waitable, private NonCopyable {

  PriorityConditionImpl* _impl;
  
 public:
  
  /**
   * Create a new condition variable associated with the given Lockable object.
   *
   * @param l - Lockable object to associate with this PriorityCondition object.
   *
   * @exception Synchronization_Exception thrown if there is some error 
   * creating the PriorityCondition object
   */
  PriorityCondition(Lockable& l) 
    /* throw(Synchronization_Exception) */;

  //! Destroy PriorityCondition object
  ~PriorityCondition()
    throw(); 
  
  /**
   * Wake <i>ONE</i> threads wait()ing on this PriorityCondition.
   *
   * @post a wait()ing thread, if any exists, will be awakened.
   *
   * @exception Synchronization_Exception thrown if there is an error 
   * performing this operation
   */
  void signal() 
    /* throw(Synchronization_Exception) */;

  /**
   * Wake <i>ALL</i> threads wait()ing on this PriorityCondition.
   *
   * @post all wait()ing threads, if any exist, will be awakened.
   *
   * @exception Synchronization_Exception thrown if there is an error 
   * performing this operation
   */
  void broadcast() 
    /* throw(Synchronization_Exception) */;


  /**
   * Wait for this PriorityCondition object to be signal()ed. The thread calling this method
   * will block until the PriorityCondition is signal()ed or broadcast() to. 
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
   * Wait for this PriorityCondition object to be signal()ed. The thread calling this method
   * will block until the PriorityCondition is signal()ed, broadcast() to or until the timeout 
   * expires.
   *
   * This operation atomically release()s the associated Lockable and blocks the calling thread.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   *
   * @return true if the PriorityCondition was signal()ed before the timeout expired, otherwise
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

#endif // __ZTPRIORITYCONDITION_H__
