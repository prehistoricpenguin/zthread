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

#ifndef __ZTRECURSIVEMUTEX_H__
#define __ZTRECURSIVEMUTEX_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread { 
  
class RecursiveMutexImpl;

/**
 * @class RecursiveMutex
 *
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-02T08:11:19-0400>
 * @version 2.2.1
 *
 * A RecursiveMutex is a recursive, MUTual EXclusion Lockable object. It is  
 * recursive because it can be acquire()d and release()d more than once 
 * by the same thread, instead of causing a Deadlock_Exception. 
 *
 * @see Mutex
 * @see Guard
 *
 * <b>Scheduling</b>
 *
 * Threads competing to acquire() a Mutex are granted access in FIFO order.
 *
 * <b>Error Checking</b>
 *
 * A Mutex will throw an InvalidOp_Exception if an attempt to release() a Mutex is 
 * made from the context of a thread that does not currently own that Mutex.
 */
class ZTHREAD_API RecursiveMutex : public Lockable, private NonCopyable {
  
  RecursiveMutexImpl* _impl;
  
 public:
  
  //! Create a new RecursiveMutex.
  RecursiveMutex() /* throw(Synchronization_Exception) */;

  //! Destroy this RecursiveMutex.
  virtual ~RecursiveMutex()
    throw();

  /**
   * Acquire a RecursiveMutex, possbily blocking until the the current owner of the 
   * RecursiveMutex release()es it or until an exception is thrown.
   *
   * Only one thread may acquire() the RecursiveMutex at any given time. The same
   * thread may acquire a RecursiveMutex multiple times.
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   *
   * @post the calling thread succesfully acquire()ed RecursiveMutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void acquire() 
    /* throw(Synchronization_Exception) */;

  /**
   * Acquire a RecursiveMutex, possbily blocking until the the current owner of the 
   * RecursiveMutex release()es it, until an exception is thrown or until the given amount
   * of time expires.
   *
   * Only one thread may acquire() the RecursiveMutex at any given time. The same
   * thread may acquire a RecursiveMutex multiple times.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * 
   * @return bool true if the RecursiveMutex was acquire()ed before the timeout expired, otherwise
   * false
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   *
   * @pre the calling thread must not have already acquire()ed RecursiveMutex
   *
   * @post the calling thread succesfully acquire()ed RecursiveMutex only if no exception
   * was thrown.
   *
   * @see Lockable::tryAcquire(unsigned long)
   */
  virtual bool tryAcquire(unsigned long) 
    /* throw(Synchronization_Exception) */;

  /**
   * Release a RecursiveMutex allowing another thread to acquire() it.
   *
   * see Lockable::release()
   *
   * @exception InvalidOp_Exception - thrown if there is an attempt to release() this
   * RecursiveMutex from the context of a thread that had not previously acquire()ed it. 
   *
   * @pre the calling thread must have first acquire()d the RecursiveMutex.
   * @post the calling thread succesfully release()d RecursiveMutex only if no exception
   * was thrown.
   *
   * @see Lockable::acquire()
   */
  virtual void release() 
    /* throw(Synchronization_Exception) */;
  
}; 
  
} // namespace ZThread

#endif // __ZTRECURSIVEMUTEX_H__
