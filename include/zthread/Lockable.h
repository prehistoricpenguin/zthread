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

#ifndef __ZTLOCKABLE_H__
#define __ZTLOCKABLE_H__

#include "zthread/Exceptions.h"

namespace ZThread { 

/**
 * @class Lockable
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-05-30T15:28:51-0400>
 * @version 2.2.0
 *
 * The Lockable interface defines a common method of adding general <i>acqurie-release</i> 
 * semantics to an object. These <i>acquire-release</i> create a protocol that does <b>not</b> 
 * neccessarily imply exclusive access. 
 *
 * The following text describes the specific meaning of <i>acqurie-release</i> 
 * semantics in more detail. 
 * 
 * <b>Acquiring/Releasing</b> 
 * 
 * Acquiring a Lockable object means that a check point has been reached. If execution
 * can continue, the Lockable object may be acquire()d. 
 *
 * Releasing a Lockable object means that a check point is being left. 
 *
 * The details of these semantics are refined further by specializaions of this 
 * class, but the general conotation remains intact. 
 *
 * For example, A Mutex class implements the Lockable interface to provide a strict protocol 
 * for <i>acquire()</i>ing and <i>release()</i>ing mutually exclusive access to something. 
 *
 * A Semaphore class implements the Lockable interface to define semantics for <i>acquire()</i>ing 
 * and <i>release()</i>ing permits for something, which may or may not be exclusive.
 */
class Lockable {
public:  
  
  //! Destroy a Lockable object.
  virtual ~Lockable() throw() {};

  /** 
   * Acquire the Lockable object. 
   *
   * This method may or may not block the caller for an indefinite amount
   * of time. Those details are defined by specializations of this class.
   *
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Synchronization_Exception thrown if some error occurs while
   * attempting to acquire this Lockable.
   *
   * @post The Lockable is safely acquired if no exception is thrown. 
   */
  virtual void acquire() 
    /* throw(Synchronization_Exception) */ = 0;


  /** 
   * Attempt to acquire the Lockable object. 
   *
   * This method may or may not block the caller for a definite amount
   * of time. Those details are defined by specializations of this class;
   * however, this method includes a timeout value that can be used to 
   * limit the maximum amount of time that a specialization <i>could</i> block. 
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   *
   * @return bool true if the operation completes and the Lockable is acquired before 
   * the timeout expires. Otherwise false is returned and the Lockable is not acquired.
   * 
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Synchronization_Exception  thrown if some error occurs while
   * attempting to acquire this Lockable.
   */
  virtual bool tryAcquire(unsigned long) 
    /* throw(Synchronization_Exception) */ = 0;
  
  /** 
   * Release the Lockable object.
   *
   * This method may or may not block the caller for an indefinite amount
   * of time. Those details are defined by specializations of this class.
   *
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Synchronization_Exception  thrown if some error occurs
   * while releasing this Lockable.
   *
   * @post The Lockable is safely released if no exception is thrown. 
   */    
  virtual void release() 
    /* throw(Synchronization_Exception) */ = 0;

};

} // namespace ZThread

#endif // __ZTLOCKABLE_H__
