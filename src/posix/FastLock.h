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
 *
 *  SUNY @ Buffalo, hereby disclaims all copyright interest in the
 *  ZThreads library written by Eric Crahen
 */

#ifndef __ZTFASTLOCK_H__
#define __ZTFASTLOCK_H__

#include "zthread/NonCopyable.h"
#include <pthread.h>

namespace ZThread {

/**
 * @class FastLock
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-10T09:13:44-0400>
 * @version 2.1.6
 *
 * This is the smallest and fastest synchronization object in the library.
 * It is an implementation of fast mutex, an all or nothing exclusive
 * lock. It should be used only where you need speed and are willing 
 * to sacrifice all the state & safety checking provided by the framework
 * for speed.
 */ 
class FastLock : private NonCopyable {
    
  pthread_mutex_t _mtx;

 public:
  
  /**
   * Create a new FastLock. No safety or state checks are performed.
   *
   * @exception Initialization_Exception - not thrown
   */
  inline FastLock() {

    pthread_mutex_init(&_mtx, 0);

  }
  
  /**
   * Destroy a FastLock. No safety or state checks are performed.
   */
  inline ~FastLock() {

    pthread_mutex_destroy(&_mtx);

  }
  
  /**
   * Acquire an exclusive lock. No safety or state checks are performed.
   *
   * @exception Synchronization_Exception - not thrown
   */
  inline void acquire() {
    
    pthread_mutex_lock(&_mtx);

  }

  /**
   * Try to acquire an exclusive lock. No safety or state checks are performed.
   * This function returns immediately regardless of the value of the timeout
   *
   * @param timeout Unused
   * @return bool
   * @exception Synchronization_Exception - not thrown
   */
  inline bool tryAcquire(unsigned long timeout=0) {

    return (pthread_mutex_trylock(&_mtx) == 0);

  }
  
  /**
   * Release an exclusive lock. No safety or state checks are performed.
   * The caller should have already acquired the lock, and release it 
   * only once.
   * 
   * @exception Synchronization_Exception - not thrown
   */
  inline void release() {
    
    pthread_mutex_unlock(&_mtx);

  }
  
  
}; /* FastLock */


};

#endif
