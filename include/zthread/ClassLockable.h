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

#ifndef __ZTCLASSLOCKABLE_H__
#define __ZTCLASSLOCKABLE_H__

#include "zthread/Lockable.h"

namespace ZThread { 

/**
 * @class ClassLockable
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-05-30T17:08:19-0400>
 * @version 2.2.0
 *
 * The ClassLockable template creates a Lockable object that delegates
 * to an different static instance of a Lockable object for each type.
 * It is used to create a Lockable shared among all instances of the class 
 * specified in the template parameter. 
 *
 */
template <typename ClassType, class LockType>
class ClassLockable : public Lockable {

  static LockType _instance;

 public:  
  
  //! Create a ClassLockable
  ClassLockable() {} 
  
  //! Destroy the ClassLockable
  virtual ~ClassLockable() {} 
  
  //! acquire() the ClassLockable
  virtual void acquire() 
    /* throw(Synchronization_Exception) */ { _instance.acquire(); }

  //! tryAcquire() the ClassLockable
  virtual bool tryAcquire(unsigned long timeout) 
    /* throw(Synchronization_Exception) */
    { return _instance.tryAcquire(timeout); }

  //! release() the ClassLockable
  virtual void release() 
    /* throw(Synchronization_Exception) */ { _instance.release(); }

};

template <typename ClassType, class LockType>
LockType ClassLockable<ClassType, LockType>::_instance;

} // namespace ZThread

#endif // __ZTCLASSLOCKABLE_H__
 
