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

#ifndef __ZTRECURSIVEMUTEXIMPL_H__
#define __ZTRECURSIVEMUTEXIMPL_H__

#include "zthread/Exceptions.h"

#include "FastLock.h"

#include <vector>

namespace ZThread {

class Monitor;

/**
 * @class RecursiveMutexImpl
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-05-26T09:16:04-0400>
 * @version 2.1.6
 *
 * This synchronization object provides serialized access 
 * through an acquire/release protocol. 
 */
class ZTHREAD_API RecursiveMutexImpl {
  
  typedef std::vector<Monitor*> List;  

  //! List of Events that are waiting for notification 
  List _waiters;

  //! Serialize access to this Mutex
  FastLock _lock;

  //! Current owning Event object
  Monitor* _owner;

  //! Entry count 
  size_t _count;

  public:
   
  RecursiveMutexImpl() 
    /* throw(Synchronization_Exception) */;

  virtual ~RecursiveMutexImpl()
    throw();
  
  void acquire() 
    /* throw(Synchronization_Exception) */;
  
  bool tryAcquire(unsigned long) 
    /* throw(Synchronization_Exception) */;
  
  void release() 
    /* throw(Synchronization_Exception) */;

}; /* RecursiveMutexImpl */


}; 

#endif
