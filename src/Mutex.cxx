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

#include "zthread/Mutex.h"
#include "MutexImpl.h"

#include <iostream>

namespace ZThread {

class FifoMutexImpl : public MutexImpl<fifo_list, NullBehavior> { };

Mutex::Mutex() /* throw(Synchronization_Exception) */ {
std::cout << "MTX!" << std::endl;  
  _impl = new FifoMutexImpl();
  
}

Mutex::~Mutex() 
 throw() {

  if(_impl != 0) 
    delete _impl;

}

// P
void Mutex::acquire() 
  /* throw(Synchronization_Exception) */ {

  _impl->acquire(); 

}


// P
bool Mutex::tryAcquire(unsigned long ms) 
  /* throw(Synchronization_Exception) */ {

  return _impl->tryAcquire(ms); 

}

// V
void Mutex::release() 
  /* throw(Synchronization_Exception) */ {

  _impl->release(); 

}



} // namespace ZThread

