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

#ifndef __ZTFASTRECURSIVELOCK_H__
#define __ZTFASTRECURSIVELOCK_H__

#include "zthread/Exceptions.h"
#include "zthread/NonCopyable.h"
#include <windows.h>
#include <assert.h>

namespace ZThread {


/**
 * @class FastRecursiveLock
 *
 * @author Eric Crahen <crahen at code-foo dot com>
 * @date <2002-12-21T08:44:02-0500>
 * @version 2.2.11
 *
 * This FastRecursiveLock implementation is based on a Win32 Mutex
 * object. This will perform better under high contention, 
 * but will not be as fast as the spin lock under reasonable
 * circumstances.
 */ 
class FastRecursiveLock : private NonCopyable {

  HANDLE _hMutex;
  volatile unsigned int _count;

  public:
  
  /**
   * Create a new FastRecursiveLock
   */
    FastRecursiveLock() : _count(0) { 

    _hMutex = ::CreateMutex(0, 0, 0);
    assert(_hMutex != NULL);
    if(_hMutex == NULL)
      throw Initialization_Exception();

  }

  
  ~FastRecursiveLock() {
    ::CloseHandle(_hMutex);
  }

  
  void acquire();

  void release();

  bool tryAcquire(unsigned long);

}; /* FastRECURSIVELock */

void FastRecursiveLock::acquire() {

    if(::WaitForSingleObject(_hMutex, INFINITE) != WAIT_OBJECT_0) {
      assert(0);
      throw Synchronization_Exception();
    }

  }

void FastRecursiveLock::release() {

  if(::ReleaseMutex(_hMutex) == 0) {
    assert(0);
    throw Synchronization_Exception();
  }
  
}
    
bool FastRecursiveLock::tryAcquire(unsigned long) {

  switch(::WaitForSingleObject(_hMutex, 0)) {
    case WAIT_OBJECT_0:
      return true;
    case WAIT_TIMEOUT:
      return false;
    default:
      break;
    }

    assert(0);
    throw Synchronization_Exception();

}


} // namespace ZThread

#endif
