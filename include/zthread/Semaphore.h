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

#ifndef __ZTSEMAPHORE_H__
#define __ZTSEMAPHORE_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread {
  
class FifoSemaphoreImpl;
 
/**
 * @class Semaphore
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-02T08:11:14-0400>
 * @version 2.2.1
 *
 * A Semaphore is an ownerless Lockable object. Its probably best described as 
 * a set of 'permits'. A Semaphore is initlized with an intial count and
 * a maximum count, these would correspond to the number of 'permits' currently
 * available and the number of' permits' in total.
 *
 * acquire()ing the Semaphore means taking a permit, but if there are none
 * (the count is 0) the Semaphore will block the calling thread.
 *
 * release()ing the Semaphore means returning a permit, unblocking a thread 
 * waiting for one.
 *
 * A Semaphore with an initial value of 1 and maximum value of 1 will act as 
 * a Mutex.
 *
 * <b>Scheduling</b>
 *
 * Threads blocked on a Semaphore are resumed in FIFO order.
 *
 * <b>Error Checking</b>
 *
 * An attempt to increase a Semaphore beyond its maximum value will result in 
 * an InvalidOp_Exception.
 */
class ZTHREAD_API Semaphore : public Lockable, private NonCopyable {
  
  FifoSemaphoreImpl* _impl;
  
 public:
  
  /**
   * Create a new Semaphore. 
   *
   * @param count - initial count
   * @param maxCount - upper bound
   *
   * @exception Synchronization_Exception thrown if the Semaphore could not 
   * be created
   */
  Semaphore(int count = 1, unsigned int maxCount = 1) 
    /* throw(Synchronization_Exception) */;

  //! Destroy the Semaphore
  virtual ~Semaphore()
    throw();

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
  virtual int count() throw();    

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
   * @exception InvalidOp_Exception thrown if this operation would increase the
   * count beyond the maximum
   * @exception Synchronization_Exception thrown if there is some other error.
   *
   * @see Lockable::release()
   */
  virtual void release() 
    /* throw(Synchronization_Exception) */;

  
}; 


} // namespace ZThread

#endif // __ZTSEMAPHORE_H__
