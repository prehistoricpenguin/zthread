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
 */

#include "Debug.h"
#include "zthread/PrioritySemaphore.h"
#include "SemaphoreImpl.h"

namespace ZThread {

class PrioritySemaphoreImpl : public SemaphoreImpl<priority_list> { 
public:

  PrioritySemaphoreImpl(int count, unsigned int maxCount) /* throw(Synchronization_Exception) */ 
    : SemaphoreImpl<priority_list>(count, maxCount, true) { }

};

/**
 * Create a new semaphore of a given size with a given count
 *
 * @param initialCount initial count to assign this semaphore
 * @param maxCount maximum size of the semaphore count
 */
PrioritySemaphore::PrioritySemaphore(int count, unsigned int maxCount) 
  /* throw(Synchronization_Exception) */ {
  
  _impl = new PrioritySemaphoreImpl(count, maxCount);
  
}

PrioritySemaphore::~PrioritySemaphore() 
  throw() {

  if(_impl != 0)
    delete _impl;

}

void PrioritySemaphore::wait() 
  /* throw(Synchronization_Exception) */ {

  _impl->acquire();

}


bool PrioritySemaphore::tryWait(unsigned long ms) 
  /* throw(Synchronization_Exception) */ {

  return _impl->tryAcquire(ms);

}

void PrioritySemaphore::post() 
  /* throw(Synchronization_Exception) */ {

  _impl->release();

}

int PrioritySemaphore::count() 
  throw() {

  return _impl->count();

}

///////////////////////////////////////////////////////////////////////////////
// Locakable compatibility
//

void PrioritySemaphore::acquire() 
  /* throw(Synchronization_Exception) */ {

  _impl->acquire();

}

bool PrioritySemaphore::tryAcquire(unsigned long ms) 
  /* throw(Synchronization_Exception) */ {

  return _impl->tryAcquire(ms);


}

void PrioritySemaphore::release() 
  /* throw(Synchronization_Exception) */ {

  _impl->release();

}

} // namespace ZThread






