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

#ifndef __ZTCOUNTINGSEMAPHORE_H__
#define __ZTCOUNTINGSEMAPHORE_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread {
  
class FifoSemaphoreImpl;
  
/**
 * @class CountingSemaphore
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-02T08:12:01-0400>
 * @version 2.2.1
 *
 * A CountingSemaphore in the same way as a Semaphore. Its an ownerless 
 * Lockable object. It differs in that there is no upper bound on this 
 * object and it will not throw exceptions because a maximum value has been exceeded.
 *
 * @see Semaphore
 *
 * <b>Scheduling</b>
 *
 * Threads blocked on a CountingSemaphore are resumed in FIFO order.
 *
 * <b>Error Checking</b>
 *
 * None. An attempt to increase a CountingSemaphore beyond its maximum value will 
 * not result in an InvalidOp_Exception.
 */
class ZTHREAD_API CountingSemaphore : public Lockable, private NonCopyable {
  
  FifoSemaphoreImpl* _impl;  

 public:

  /**
   * Create a new CountingSemaphore. 
   *
   * @param count - initial count
   *
   * @exception Synchronization_Exception thrown if the Semaphore could not 
   * be created
   */
  CountingSemaphore(int initialCount = 0) 
    /*throw(Synchronization_Exception) */;

  //! Destroy the CountingSemaphore
  virtual ~CountingSemaphore() throw();

  /**
   * Decrement the count.
   *
   * @see acquire() - provided to reflect the traditional Semaphore
   * semantics
   */ 
  void wait() 
    /* throw(Synchronization_Exception) */;


  /**
   * Decrement the count.
   *
   * @see tryAcquire() - provided to reflect the traditional Semaphore
   * semantics
   */
  bool tryWait(unsigned long) 
    /* throw(Synchronization_Exception) */;

  /**
   * Increment the count.
   *
   * @see release()  - provided to reflect the traditional Semaphore
   * semantics
   */
  void post() 
    /* throw(Synchronization_Exception) */;
  
  /**
   * Get the current count of the semaphore. This value may change immediately
   * after this function returns to the calling thread.
   *
   * @return int - count
   */
  virtual int count() 
    throw();    
  
  /**
   * Decrement the count, blocking that calling thread if the count becomes 0 or 
   * less than 0. The calling thread will remain blocked until the count is 
   * raised above 0, an exception is thrown or the given amount of time expires.
   * 
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * 
   * @return bool true if the Semaphore was acquire()ed before the timeout expired, 
   * otherwise false
   *
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Synchronization_Exception thrown if there is some other error.
   * 
   * @see Lockable::tryAcquire(unsigned long)
   */
  virtual bool tryAcquire(unsigned long timeout)  
    /* throw(Synchronization_Exception) */;

  /**
   * Decrement the count, blocking that calling thread if the count becomes 0 or 
   * less than 0. The calling thread will remain blocked until the count is 
   * raised above 0 or if an exception is thrown.
   * 
   * @exception Interrupted_Exception thrown when the calling thread is interupt()ed.
   * A thread may be interrupted at any time, prematurely ending any wait.
   * @exception Synchronization_Exception thrown if there is some other error.
   * 
   * @see Lockable::acquire()
   */
  virtual void acquire() 
    /* throw(Synchronization_Exception) */;

  /**
   * Increment the count, unblocking one thread if count is posative.
   *
   * @exception Synchronization_Exception thrown if there is some other error.
   *
   * @see Lockable::release()
   */
  virtual void release() 
    /* throw(Synchronization_Exception) */;

  
}; 


} // namespace ZThread

#endif // __ZTCOUNTINGSEMAPHORE_H__

