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

#ifndef __ZTLOCKEDQUEUE_H__
#define __ZTLOCKEDQUEUE_H__

#include "zthread/Guard.h"
#include "zthread/Queue.h"

#include <deque>

namespace ZThread {

/**
 * @class LockedQueue
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-10T07:02:13-0400>
 * @version 2.2.1
 *
 * A LockedQueue is the simple Queue implementation that provides 
 * serialized access to the items added to it.
 */
template <class T, class LockType, typename StorageType=std::deque<T> >
class LockedQueue : public Queue<T> {

  //! Serialize access to the Queue
  LockType _lock;

  //! Storage backing the queue
  StorageType _queue;

  //! Cancellation flag
  volatile bool _canceled;

public:

  //! Create a LockedQueue
  LockedQueue() /* throw(Synchronization_Exception) */: _canceled(false) {}

  //! Destroy a LockedQueue
  virtual ~LockedQueue() throw() {

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
   */
  virtual void add(T item) 
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    
    if(_canceled)
      throw Cancellation_Exception();

    _queue.push_back(item);

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
   */
  virtual bool add(T item, unsigned long timeout) 
    /* throw(Synchronization_Exception) */ {

    try {

      Guard<LockType> g(_lock, timeout);
      
      if(_canceled)
      throw Cancellation_Exception();
      
      _queue.push_back(item);

    } catch(Timeout_Exception&) { return false; }
 
    return true;

  }

  /**
   * Get an object from this Queue. 
   *
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained or until an exception is thrown.
   *
   * @return T next available object
   * 
   * @exception NoSuchElement_Exception thrown if the queue is empty.
   *
   * @see Queue::next()
   */
  virtual T next()
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);

    if(_queue.size() == 0 && _canceled)
      throw Cancellation_Exception();
    
    if(_queue.size() == 0)
      throw NoSuchElement_Exception();

    T item = _queue.front();
    _queue.pop_front();
    
    return item;

  }


  /**
   * Get an object from this Queue. 
   *
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained or until an exception is thrown.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * @return T next available object
   * 
   * @exception NoSuchElement_Exception thrown if the queue is empty.
   *
   * @see Queue::next(unsigned long)
   */
  virtual T next(unsigned long timeout)
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock, timeout);

    if(_queue.size() == 0 && _canceled)
      throw Cancellation_Exception();
    
    if(_queue.size() == 0)
      throw NoSuchElement_Exception();

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
   */
  virtual void cancel()
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);

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
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained or until an exception is thrown.
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
   * This method will block the calling thread until exclusive access to 
   * the Queue can be obtained or until an exception is thrown.
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

#endif // __ZTLOCKEDQUEUE_H__




