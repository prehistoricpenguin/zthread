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

#ifndef __ZTFASTLOCK_H__
#define __ZTFASTLOCK_H__

#include "zthread/Exceptions.h"
#include "zthread/NonCopyable.h"
#include "../ThreadOps.h"
#include <windows.h>
#include <assert.h>

namespace ZThread {


/**
 * @class FastLock
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-05-27T08:52:14-0400>
 * @version 2.2.4
 *
 * This FastLock implementation is based on a Win32 Mutex
 * object. This will perform better under high contention, 
 * but will not be as fast as the spin lock under reasonable
 * circumstances.
 */ 
class FastLock : private NonCopyable {

  HANDLE _hMutex;
#ifndef NDEBUG
  volatile bool _locked;
#endif

  public:
  
  /**
   * Create a new FastLock
   */
  FastLock() { 

#ifndef NDEBUG
  _locked = false;
#endif

    _hMutex = ::CreateMutex(0, 0, 0);
    assert(_hMutex != NULL);
    if(_hMutex == NULL)
      throw Initialization_Exception();

  }

  
  ~FastLock() {
    ::CloseHandle(_hMutex);
  }

  void acquire() {

    if(::WaitForSingleObject(_hMutex, INFINITE) != WAIT_OBJECT_0) {
      assert(0);
      throw Synchronization_Exception();
    }

#ifndef NDEBUG

    // Simulate deadlock to provide consistent behavior. This
    // will help avoid errors when porting. Avoiding situations
    // where a FastMutex mistakenly behaves as a recursive lock.

    while(_locked)
      ThreadOps::yield();

  _locked = true;

#endif

  }

  void release() {

#ifndef NDEBUG
    _locked = false;
#endif

    if(::ReleaseMutex(_hMutex) == 0) {
      assert(0);
      throw Synchronization_Exception();
    }

  }
    
  bool tryAcquire(unsigned long timeout=0) {

    switch(::WaitForSingleObject(_hMutex, 0)) {
    case WAIT_OBJECT_0:

#ifndef NDEBUG

    // Simulate deadlock to provide consistent behavior. This
    // will help avoid errors when porting. Avoiding situations
    // where a FastMutex mistakenly behaves as a recursive lock.

    while(_locked)
      ThreadOps::yield();

  _locked = true;

#endif

      return true;
    case WAIT_TIMEOUT:
      return false;
    default:
      break;
    }

    assert(0);
    throw Synchronization_Exception();

 }
  
}; /* FastLock */

} // namespace ZThread

#endif
