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

#ifndef __ZTMUTEX_H__
#define __ZTMUTEX_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread { 
  
  class FifoMutexImpl;

/**
 * @class Mutex
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-02T08:10:54-0400>
 * @version 2.2.1
 *
 * A Mutex is used to provide serialized (one thread at a time) access to some portion
 * of code. This is accomplished by attempting to acquire() the Mutex before entering that
 * piece of code, and by release()ing the Mutex when leaving that region. It is a non-reentrant,
 * MUTual EXclusion Lockable object. 
 *
 * Using a Mutex requires special care. It is often easy to forget to release() a Mutex
 * after having acquire()ed it. Especially in situations where the flow of execution is
 * not linear because of branching and/or exception. The Guard class is an application of
 * the <i>Scoped Locking</i> pattern which helps to remove this difficulty from 
 * multi-threaded code. 
 *
 * @see Guard
 *
 * <b>Scheduling</b>
 *
 * Threads competing to acquire() a Mutex are granted access in FIFO order.
 *
 * <b>Error Checking</b>
 *
 * A Mutex will throw a Deadlock_Exception if an attempt to acquire() a Mutex more 
 * than once is made from the context of the same thread.
 *
 * A Mutex will throw an InvalidOp_Exception if an attempt to release() a Mutex is 
 * made from the context of a thread that does not currently own that Mutex.
 */
class ZTHREAD_API Mutex : public Lockable, private NonCopyable {
  
  FifoMutexImpl* _impl;
  
 public:

  //! Create a new Mutex.
  Mutex() 
    /* throw(Synchronization_Exception) */;

  //! Destroy this Mutex.
  virtual ~Mutex() throw();
  
  /**
   * Acquire a Mutex, possbily blocking until the the current owner of the 
   * Mutex release()es it or until an exception is thrown.
   *
   * Only one thread may acquire() the Mutex at any given time.
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Deadlock_Exception thrown when the same thread attempts to acquire()
   * a Mutex more than once, without having first release()ed it.
   *
   * @pre the calling thread must not have already acquire()ed Mutex
   *
   * @post the calling thread succesfully acquire()ed Mutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void acquire() 
    /* throw(Synchronization_Exception) */;

  /**
   * Acquire a Mutex, possbily blocking until the the current owner of the 
   * Mutex release()es it, until an exception is thrown or until the given amount
   * of time expires.
   *
   * Only one thread may acquire() the Mutex at any given time.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * 
   * @return bool true if the Mutex was acquire()ed before the timeout expired, otherwise
   * false
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Deadlock_Exception thrown when the same thread attempts to acquire()
   * a Mutex more than once, without having first release()ed it.
   *
   * @pre the calling thread must not have already acquire()ed Mutex
   *
   * @post the calling thread succesfully acquire()ed Mutex only if no exception
   * was thrown.
   *
   * @see Lockable::tryAcquire(unsigned long)
   */
  virtual bool tryAcquire(unsigned long) 
    /* throw(Synchronization_Exception) */;
  
  /**
   * Release a Mutex allowing another thread to acquire() it.
   *
   * see Lockable::release()
   *
   * @exception InvalidOp_Exception - thrown if there is an attempt to release() this
   * Mutex from the context of a thread that had not previously acquire()ed it. 
   *
   * @pre the calling thread must have first acquire()d the Mutex.
   * @post the calling thread succesfully release()d Mutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void release() 
    /* throw(Synchronization_Exception) */;
  
};


} // namespace ZThread

#endif // __ZTMUTEX_H__
