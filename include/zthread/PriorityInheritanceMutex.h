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

#ifndef __ZTPRIORITYINHERITANCEMUTEX_H__
#define __ZTPRIORITYINHERITANCEMUTEX_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread { 
  
  class PriorityInheritanceMutexImpl;

/**
 * @class PriorityInheritanceMutex
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T08:10:44-0400>
 * @version 2.2.1
 *
 * A PriorityInheritanceMutex is similar to a PriorityMutex, it is a non-reentrant, 
 * priority sensative MUTual EXclusion Lockable object. It differs only in its 
 * scheduling policy.
 *
 * @see PriorityMutex
 *
 * <b>Scheduling</b>
 *
 * Threads competing to acquire() a PriorityInheritanceMutex are granted access in 
 * order of priority. Threads with a higher priority will be given access first.
 *
 * When a higher priority thread tries to acquire() a PriorityInheritanceMutex and is 
 * about to be blocked by a lower priority thread that has alerady acquire()d it, the
 * lower priority thread will temporarily have its effective priority raised to that 
 * of the higher priority thread until it release()s the mutex; at which point its 
 * previous priority will be restored. 
 */
class ZTHREAD_API PriorityInheritanceMutex : public Lockable, private NonCopyable {
  
  PriorityInheritanceMutexImpl* _impl;
  
 public:

  //! Create a new PriorityInheritanceMutex.
  PriorityInheritanceMutex() 
    /* throw(Synchronization_Exception) */;

  //! Destroy this PriorityInheritanceMutex.
  virtual ~PriorityInheritanceMutex()
    throw();
  
  /**
   * Acquire a PriorityInheritanceMutex, possbily blocking until the the current owner of the 
   * PriorityInheritanceMutex release()es it or until an exception is thrown.
   *
   * Only one thread may acquire() the PriorityInheritanceMutex at any given time.
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Deadlock_Exception thrown when the same thread attempts to acquire()
   * a PriorityInheritanceMutex more than once, without having first release()ed it.
   *
   * @pre the calling thread must not have already acquire()ed PriorityInheritanceMutex
   *
   * @post the calling thread succesfully acquire()ed PriorityInheritanceMutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void acquire() 
    /* throw(Synchronization_Exception) */;

  /**
   * Acquire a PriorityInheritanceMutex, possbily blocking until the the current owner of the 
   * PriorityInheritanceMutex release()es it, until an exception is thrown or until the given amount
   * of time expires.
   *
   * Only one thread may acquire() the PriorityInheritanceMutex at any given time.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * 
   * @return bool true if the PriorityInheritanceMutex was acquire()ed before the timeout expired, otherwise
   * false
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Deadlock_Exception thrown when the same thread attempts to acquire()
   * a PriorityInheritanceMutex more than once, without having first release()ed it.
   *
   * @pre the calling thread must not have already acquire()ed PriorityInheritanceMutex
   *
   * @post the calling thread succesfully acquire()ed PriorityInheritanceMutex only if no exception
   * was thrown.
   *
   * @see Lockable::tryAcquire(unsigned long)
   */
  virtual bool tryAcquire(unsigned long) 
    /* throw(Synchronization_Exception) */;
  
  /**
   * Release a PriorityInheritanceMutex allowing another thread to acquire() it.
   *
   * see Lockable::release()
   *
   * @exception InvalidOp_Exception - thrown if there is an attempt to release() this
   * PriorityInheritanceMutex from the context of a thread that had not previously acquire()ed it. 
   *
   * @pre the calling thread must have first acquire()d the PriorityInheritanceMutex.
   * @post the calling thread succesfully release()d PriorityInheritanceMutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void release() 
    /* throw(Synchronization_Exception) */;
  
}; 


} // namespace ZThread

#endif // __ZTPRIORITYINHERITANCEMUTEX_H__
