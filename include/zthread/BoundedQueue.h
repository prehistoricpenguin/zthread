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

#ifndef __ZTBOUNDEDQUEUE_H__
#define __ZTBOUNDEDQUEUE_H__

#include "zthread/Condition.h"
#include "zthread/Guard.h"
#include "zthread/Queue.h"

#include <deque>

namespace ZThread {

/**
 * @class BoundedQueue
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T10:38:07-0400>
 * @version 2.2.0
 *
 * A BoundedQueues is a Queue implementation that provides  serialized access to the 
 * items added to it. It addition, a BoundedQueues will block threads calling 
 * empty() methods until the Queue becomes empty. Similarly, threads calling the 
 * next() methods will be blocked until items arrive. Finally, threads calling the
 * the add() methods will be blocked if the Queue is at capacity.
 */
template <class T, class LockType, typename StorageType=std::deque<T> >
class BoundedQueue : public Queue<T> {

  //! Maximum capacity for the Queue
  unsigned int _capacity;

  //! Serialize access
  LockType _lock;

  //! Signaled if not full
  Condition _notFull;

  //! Signaled if not empty
  Condition _notEmpty;

  //! Signaled if empty
  Condition _isEmpty;

  //! Storage backing the queue
  StorageType _queue;

  //! Cancellation flag
  volatile bool _canceled;

  public:

  /**
   * Create a new Queue with the given capacity. This 
   * capacity can not be altered once the Queue is created.
   * 
   * @param int capacity
   */
  BoundedQueue(unsigned int capacity) /* throw(Synchronization_Exception) */ 
    :  _capacity(capacity), _canceled(false) { }
  
  //! Destroy this Queueand any remaining items 
  virtual ~BoundedQueue() throw() {
    
    for(typename StorageType::iterator i = _queue.begin(); _queue.size() > 0;) {

      delete *i;
      i = _queue.erase(i);

    }


  }

  /**
   * Get the capacity of this Queue. When the Queue is at capacity
   * the add() methods will block.
   *
   * @return unsigned int capacity
   */
  unsigned int capacity() { 
    return _capacity; 
  }

  /**
   * Add an object to this Queue, tranfering the ownership of that
   * object to the Queue. As the new owner, the Queue becomes responsible 
   * for deleting the add()ed object. 
   *
   * This method may block the caller for an indefinite amount
   * of time if the Queue is at capacity.
   *
   * @param item - object to attempt to add to this Queue
   * 
   * @see Queue::add(T)
   *
   * @post if the Queue was empty, threads blocked by a next() method will 
   * be awakend if no exception is thrown
   */
  virtual void add(T item) 
    /* throw(Synchronization_Exception) */{

    Guard<LockType> g(_lock);
      
    // Wait for the capacity of the Queue to drop 
    while ((_queue.size() == _capacity) && !_canceled)
      _notFull.wait(_lock);
      
    if(_canceled)
      throw Cancellation_Exception();

    _queue.push_back(item);
    _notEmpty.signal(); // Wake any waiters
      
    
  }
    
  /**
   * Add an object to this Queue, tranfering the ownership of that
   * object to the Queue. As the new owner, the Queue becomes responsible 
   * for deleting the add()ed object. 
   *
   * This method may block the caller for an indefinite amount
   * of time if the Queue is at capacity.
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
   * be awakend if no exception is thrown
   */
  virtual void add(T item, unsigned long timeout) 
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock, timeout);
      
    // Wait for the capacity of the Queue to drop 
    while ((_queue.size() == _capacity) && !_canceled)
      if(!_notFull.wait(_lock, timeout))
       throw Timeout_Exception();

    if(_canceled)
      throw Cancellation_Exception();

    _queue.push_back(item);
    _notEmpty.signal(); // Wake any waiters
      
    
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
      
    while ( _queue.size() == 0 && !_canceled)
      _notEmpty.wait(_lock);
    
    if( _queue.size() == 0) // Queue canceled
      throw Cancellation_Exception();  

    T item = _queue.front();
    _queue.pop_front();
      
    _notFull.signal(); // Wake any thread trying to add

    if(_queue.size() == 0) // Wake empty waiters
      _isEmpty.broadcast();

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
   * blocked by an empty() method will be awakend.
   */
  virtual T next(unsigned long timeout)
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock, timeout);
    
    // Wait for items to be added
    while (_queue.size() == 0 && !_canceled) {
      if(!_notEmpty.wait(_lock, timeout))
        throw Timeout_Exception();
    }

    if(_queue.size() == 0)  // Queue canceled
      throw Cancellation_Exception();  

    T item = _queue.front();
    _queue.pop_front();

    _notFull.signal(); // Wake add() waiters

    if(_queue.size() == 0) // Wake empty() waiters
      _isEmpty.broadcast();
    
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

    _canceled = true;
    _notEmpty.broadcast(); // Wake next() waiters

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

    // Faster check since the Queue will not become un-canceled
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
   * @param timeout - maximum amount of time (millseconds) this method could block
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

  /**
   * Test this Queue to see if it is empty. This will block the calling
   * thread <i>until</i> the Queue becomes empty.
   * 
   * @return boolean  true if empty, otherwise false
   *
   * @exception Cancellation_Exception should not be thrown. A cancel()ed
   * Queue that contains no more elements should report that it is 
   * empty.
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Timeout_Exception thrown if the given amount
   * of time has expired before an item becomes available
   * @exception Synchronization_Exception thrown if there is some error in
   * counting the items.
   *
   * @see Queue::empty()
   */
  virtual bool empty() 
    /* throw(Synchronization_Exception) */ {


    Guard<LockType> g(_lock);

    while(_queue.size() > 0) // Wait for an empty signal
      _isEmpty.wait();
    
    return true;


  }

  /**
   * Test this Queue to see if it is empty. This will block the calling
   * thread <i>until</i> the Queue becomes empty.
   * 
   * @param timeout - maximum amount of time (millseconds) this method could block
   *
   * @return boolean  true if empty, otherwise false
   *
   * @exception Cancellation_Exception should not be thrown. A cancel()ed
   * Queue that contains no more elements should report that it is 
   * empty.
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Timeout_Exception thrown if the given amount
   * of time has expired before an item becomes available
   * @exception Synchronization_Exception thrown if there is some error in
   * counting the items.
   *
   * @see Queue::empty(unsigned long)
   */
  virtual bool empty(unsigned long timeout) 
    /* throw(Synchronization_Exception) */ {


    Guard<LockType> g(_lock, timeout);

    while(_queue.size() > 0) // Wait for an empty signal
      _isEmpty.wait(timeout);
    
    return true;


  }

};

} // namespace ZThread

#endif // __ZTBOUNDEDQUEUE_H__
