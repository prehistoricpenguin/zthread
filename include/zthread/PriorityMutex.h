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

#ifndef __ZTPRIORITYMUTEX_H__
#define __ZTPRIORITYMUTEX_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread { 
  
  class PriorityMutexImpl;

/**
 * @class PriorityMutex
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T08:10:49-0400>
 * @version 2.2.1
 *
 * A PriorityMutex is similar to a Mutex, with exception that a PriorityMutex 
 * has a difference scheduling policy. It is a non-reentrant, priority sensative 
 * MUTual EXclusion Lockable object. 
 *
 * @see Mutex
 *
 * <b>Scheduling</b>
 *
 * Threads competing to acquire() a Mutex are granted access in order of priority. Threads
 * with a higher priority will be given access first.
 */
class ZTHREAD_API PriorityMutex : public Lockable, private NonCopyable {
  
  PriorityMutexImpl* _impl;
  
 public:

  //! Create a PriorityMutex
  PriorityMutex() 
    /* throw(Synchronization_Exception) */;

  //! Destroy this PriorityMutex
  virtual ~PriorityMutex()
    throw();
  
  /**
   * Acquire a PriorityMutex, possbily blocking until the the current owner of the 
   * PriorityMutex release()es it or until an exception is thrown.
   *
   * Only one thread may acquire() the PriorityMutex at any given time.
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Deadlock_Exception thrown when the same thread attempts to acquire()
   * a PriorityMutex more than once, without having first release()ed it.
   *
   * @pre the calling thread must not have already acquire()ed PriorityMutex
   *
   * @post the calling thread succesfully acquire()ed PriorityMutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void acquire() 
    /* throw(Synchronization_Exception) */;

  /**
   * Acquire a PriorityMutex, possbily blocking until the the current owner of the 
   * PriorityMutex release()es it, until an exception is thrown or until the given amount
   * of time expires.
   *
   * Only one thread may acquire() the PriorityMutex at any given time.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * 
   * @return bool true if the PriorityMutex was acquire()ed before the timeout expired, otherwise
   * false
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Deadlock_Exception thrown when the same thread attempts to acquire()
   * a PriorityMutex more than once, without having first release()ed it.
   *
   * @pre the calling thread must not have already acquire()ed PriorityMutex
   *
   * @post the calling thread succesfully acquire()ed PriorityMutex only if no exception
   * was thrown.
   *
   * @see Lockable::tryAcquire(unsigned long)
   */
  virtual bool tryAcquire(unsigned long) 
    /* throw(Synchronization_Exception) */;
  
  /**
   * Release a PriorityMutex allowing another thread to acquire() it.
   *
   * see Lockable::release()
   *
   * @exception InvalidOp_Exception - thrown if there is an attempt to release() this
   * PriorityMutex from the context of a thread that had not previously acquire()ed it. 
   *
   * @pre the calling thread must have first acquire()d the PriorityMutex.
   * @post the calling thread succesfully release()d PriorityMutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void release() 
    /* throw(Synchronization_Exception) */;
  
}; 


} // namespace ZThread

#endif // __ZTPRIORITYMUTEX_H__
