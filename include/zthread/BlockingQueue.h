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

#ifndef __ZTBLOCKINGQUEUE_H__
#define __ZTBLOCKINGQUEUE_H__

#include "zthread/Guard.h"
#include "zthread/Condition.h"
#include "zthread/Queue.h"

#include <deque>

namespace ZThread {

/**
 * @class BlockingQueue
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T10:39:17-0400>
 * @version 2.2.1
 *
 * Like a LockedQueue, a BlockingQueue is a Queue implementation that provides 
 * serialized access to the items added to it. It differs by causing threads
 * causing the next() methods to block until an item becomes available.
 */
template <class T, class LockType, typename StorageType=std::deque<T> >
class BlockingQueue : public Queue<T> {

  //! Serialize access
  LockType _lock;

  //! Signaled when empty
  Condition _notEmpty;

  //! Storage backing the queue
  StorageType _queue;

  //! Cancellation flag
  volatile bool _canceled;

public:

  //! Create a new BlockingQueue
  BlockingQueue() /* throw(Synchronization_Exception) */ : _canceled(false) {}

  //! Destroy this BlockingQueue, delete remaining items
  virtual ~BlockingQueue() throw() {

    for(typename StorageType::iterator i = _queue.begin(); _queue.size() > 0;) {

      delete *i;
      i = _queue.erase(i);

    }

  }

  /**
   * Adds an object to this Queue.
   *
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained or until an exception is thrown.
   *
   * @param item - object to attempt to add to this Queue
   * 
   * @see Queue::add(T)
   *
   * @post if the Queue was empty, threads blocked by a next() method will 
   * be awakend if no exception is thrown
   */
  virtual void add(T item) 
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    
    if(_canceled)
      throw Cancellation_Exception();

    _queue.push_back(item);

    _notEmpty.signal();

  }

  /**
   * Adds an object to this Queue.
   *
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained, until an exception is thrown or until the
   * given amount of time expires.
   *
   * @param item - object to attempt to add to this Queue
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * 
   * @return true if the item was add()ed before the given timeout expired. Otherwise
   * false
   * 
   * @see Queue::add(T, unsigned long)
   *
   * @post if the Queue was empty, threads blocked by a next() method will 
   * be awakend if no exception is thrown and the method returns true
   */
  virtual bool add(T item, unsigned long timeout) 
    /* throw(Synchronization_Exception) */ {

    try {

      Guard<LockType> g(_lock, timeout);
      
      if(_canceled)
      throw Cancellation_Exception();
      
      _queue.push_back(item);

      _notEmpty.signal();

    } catch(Timeout_Exception&) { return false; }
 
    return true;    

  }

  /**
   * Get an object from this Queue. 
   *
   * This method will block the calling thread until an item
   * arrives in the Queue or until an exception is thrown.
   *
   * @return T next available object
   * 
   * @see Queue::next()
   *
   * @exception Cancellation_Exception thrown if there are no items available 
   * in the Queue and the Queue has become cancel()ed
   *
   * @post if the Queue becomes empty as a result of this method, threads 
   * blocked by an empty() or size() method will be awakend.
   */
  virtual T next()
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);

    while(_queue.size() == 0 && !_canceled)
      _notEmpty.wait(_lock);

    if(_queue.size() == 0 )
      throw Cancellation_Exception();
    
  
    T item = _queue.front();
    _queue.pop_front();
    
    return item;

  }


  /**
   * Get an object from this Queue. 
   *
   * This method will block the calling thread until an item
   * arrives in the Queue or until an exception is thrown.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * @return T next available object
   * 
   * @see Queue::next(unsigned long)
   *
   * @exception Cancellation_Exception thrown if there are no items available 
   * in the Queue and the Queue has become cancel()ed
   *
   * @post if the Queue becomes empty as a result of this method, threads 
   * blocked by an empty() or size() method will be awakend.
   */
  virtual T next(unsigned long timeout)
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock, timeout);

    while(_queue.size() == 0 && !_canceled) {
      if(!_notEmpty.wait(_lock, timeout))
        throw Timeout_Exception();
    }

    if(_queue.size() == 0 )
      throw Cancellation_Exception();
  
    T item = _queue.front();
    _queue.pop_front();
    
    return item;

  }


  /**
   * Cancel this queue. 
   *
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained or until an exception is thrown.
   *
   * @see Queue::cancel()
   *
   * @post If threads are blocked on a next(), empty() or size() method, 
   * they will awakend
   */
  virtual void cancel()
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);

    _notEmpty.broadcast();
    _canceled = true;

  }

  /**
   * Determine if this Queue has been cancel()ed.
   *
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained or until an exception is thrown.
   *
   * @return bool true if cancel() was called prior to this method, otherwise false.
   * 
   * @see Queue::isCanceled()
   */
  virtual bool isCanceled()
    /* throw(Synchronization_Exception) */ {

    // Faster check since the queue will not become un-canceled
    if(_canceled)
      return true;
    
    Guard<LockType> g(_lock);

    return _canceled;

  }

  /**
   * Count the items present in this Queue. 
   *
   * This method will not block the calling thread.
   *
   * @return size_t number of elements available in the Queue. These are
   * retrievable through the next() methods.
   *
   * @see Queue::size()
   */
  virtual size_t size()
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    return _queue.size();

  }

  /**
   * Count the items present in this Queue. 
   *
   * This method will not block the calling thread.
   *
   * @return size_t number of elements available in the Queue. These are
   * retrievable through the next() methods.
   *
   * @see Queue::size(unsigned long)
   */
  virtual size_t size(unsigned long timeout)
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock, timeout);
    return _queue.size();

  }

};

} // namespace ZThread

#endif // __ZTBLOCKINGQUEUE_H__
