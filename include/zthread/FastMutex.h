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

#ifndef __ZTFASTMUTEX_H__
#define __ZTFASTMUTEX_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread {

class FastLock;

/**
 * @class FastMutex
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-05-30T15:38:35-0400>
 * @version 2.2.0
 *
 * A FastMutex is a small fast implementation of a non-recursive, mutally exclusive
 * Lockable object. This implementation is a bit faster than the other Mutex classes
 * as it involved the least overhead. However, this slight increase in speed is 
 * gained by sacrificing the robustness provided by the other classes. 
 *
 * Typically, a FastMutex is implemented using a spin lock. It should be reserved 
 * for synchronizing short sections of code.
 *
 * @see Mutex
 *
 * <b>Scheduling</b>
 *
 * No garuntees about the order threads are resumed in is made.
 *
 * <b>Error Checking</b>
 *
 * No error checking is performed, this means there is the potential for deadlock.
 */ 
class ZTHREAD_API FastMutex : public Lockable, private NonCopyable {
    
  FastLock* _lock;

 public:
  
  //! Create a new FastMutex
  FastMutex() 
    /* throw(Synchronization_Exception) */;
  
  //! Destroy a new FastMutex
  virtual ~FastMutex() 
    throw();
  
  /**
   * Acquire exclusive access. No safety or state checks are performed.
   *
   * @exception Synchronization_Exception never thrown
   *
   * @pre the caller should <i>not</i> have previously acquire()d this lock
   */
  virtual void acquire() 
    /* throw(Synchronization_Exception) */; 
  
  /**
   * Release exclusive access. No safety or state checks are performed.
   * 
   * @exception Synchronization_Exception never thrown
   *
   * @pre the caller should have previously acquire()d this lock
   */
  virtual void release() 
    /* throw(Synchronization_Exception) */;
  
  /**
   * Try to acquire exclusive access. No safety or state checks are performed.
   * This function returns immediately regardless of the value of the timeout
   *
   * @param timeout - unused
   * @return bool true if the lock was acquire()ed, otherwise false.
   *
   * @exception Synchronization_Exception never thrown
   * 
   * @pre the caller should <i>not</i> have previously acquire()d this lock
   */
  virtual bool tryAcquire(unsigned long timeout)
    /* throw(Synchronization_Exception) */;
  
}; /* FastMutex */

};

#endif
