/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2001 Eric Crahen, See LGPL.TXT for details
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


#include "zthread/RecursiveMutex.h"
#include "RecursiveMutexImpl.h"

namespace ZThread {

RecursiveMutex::RecursiveMutex() 
  /* throw(Synchronization_Exception) */ {
  
  _impl = new RecursiveMutexImpl();
  
}

RecursiveMutex::~RecursiveMutex() 
  throw() {

  if(_impl != (RecursiveMutexImpl*)0 ) 
    delete _impl;

}


void RecursiveMutex::acquire() 
  /* throw(Synchronization_Exception) */{

  _impl->acquire(); 

}


bool RecursiveMutex::tryAcquire(unsigned long ms) 
  /* throw(Synchronization_Exception) */ {

  return _impl->tryAcquire(ms); 

}

void RecursiveMutex::release() 
  /* throw(Synchronization_Exception) */ {

  _impl->release(); 

}

} // namespace ZThread
