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
 *
 *  SUNY @ Buffalo, hereby disclaims all copyright interest in the
 *  ZThreads library written by Eric Crahen
 */

#ifndef __ZTFAIRREADWRITELOCK_H__
#define __ZTFAIRREADWRITELOCK_H__

#include "zthread/ReadWriteLock.h"
#include "zthread/Condition.h"
#include "zthread/Guard.h"
#include "zthread/Mutex.h"

namespace ZThread {

/**
 * @class FairReadWriteLock
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-19T08:09:25-0400>
 * @version 2.2.7
 *  
 * A ReadWriteLock that has a no special bias. It will 
 * maintain a balance between the order readers and writers
 * by allowing waiting threads to acquire one lock or the other in 
 * FIFO order.
 *
 * @see ReadWriteLock 
 */
class FairReadWriteLock : public ReadWriteLock {

  Mutex _lock;
  Condition _cond;
  
  volatile int _readers;

  /**
   * @class ReadLock
   */
  class ReadLock : public Lockable {

    FairReadWriteLock& _rwlock;

  public:

    ReadLock(FairReadWriteLock& rwlock) : _rwlock(rwlock) {}

    virtual ~ReadLock() throw() {}

    virtual void acquire() {

      Guard<Mutex> g(_rwlock._lock);
      ++_rwlock._readers; 

    }

    virtual bool tryAcquire(unsigned long timeout) {
      
      if(!_rwlock._lock.tryAcquire(timeout))
        return false;

      ++_rwlock._readers; 
      _rwlock._lock.release();

      return true;
    }

    virtual void release() {

      Guard<Mutex> g(_rwlock._lock);
      --_rwlock._readers; 

      if(_rwlock._readers == 0)
	_rwlock._cond.signal();

    }

  };

  /**
   * @class WriteLock
   */
  class WriteLock : public Lockable {

    FairReadWriteLock& _rwlock;

  public:

    WriteLock(FairReadWriteLock& rwlock) : _rwlock(rwlock) {}

    virtual ~WriteLock() throw() {}

    virtual void acquire() {

      _rwlock._lock.acquire();

      try {

        while(_rwlock._readers > 0)
          _rwlock._cond.wait();

      } catch(...) {

        _rwlock._lock.release();
        throw;

      }

    }

    virtual bool tryAcquire(unsigned long timeout) {
      
      if(!_rwlock._lock.tryAcquire(timeout))
        return false;

      try {

        while(_rwlock._readers > 0)
          _rwlock._cond.wait(timeout);

      } catch(...) {

        _rwlock._lock.release();
        throw;

      }

      return true;

    }

    virtual void release() {
      _rwlock._lock.release();
    }

  };

  friend class ReadLock;
  friend class WriteLock;

  ReadLock _rlock;
  WriteLock _wlock;

 public:
  
  /**
   * Create a fair or somewhat balanced ReadWriteLock
   */
  FairReadWriteLock() : _cond(_lock), _readers(0), _rlock(*this), _wlock(*this) {}
    /* throw(Synchronization_Exception) */ 

  /**
   * Destroy this ReadWriteLock
   */
  virtual ~FairReadWriteLock() throw() {}

  /**
   * Get a reference to the read lock
   *
   * @return Lockable& read lock
   */
  virtual Lockable& getReadLock() { return _rlock; }

  /**
   * Get a reference to the write lock
   *
   * @return Lockable& write lock
   */
  virtual Lockable& getWriteLock() { return _wlock; }
  
};

}; // __ZTFAIRREADWRITELOCK_H__

#endif
