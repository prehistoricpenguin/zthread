/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2000-2002 Eric Crahen, See LGPL.TXT for details
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

#ifndef __ZTNULLLOCKABLE_H__
#define __ZTNULLLOCKABLE_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread { 
  
/**
 * @class NullLockable
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-05-30T19:56:19-0400>
 * @version 2.2.0
 *
 * A NullLockable is an application of the <i>Null Object</i> pattern. It implements a
 * Lockable object that does nothing. This type can be subsitituted into many of
 * ZThreads parametized classes to create single implementation that perform no real
 * locking.
 */
  class NullLockable : public Lockable { //, private NonCopyable {
  public:

  //! Create a new NullLockable
  inline NullLockable() /* throw(Synchronization_Exception) */ {}

  //! Destroy a NullLockable
  inline virtual ~NullLockable() throw() {}
  
  /**
   * Do nothing
   * 
   * @exception Synchronization_Exception never thrown
   */
  inline virtual void acquire() 
    /* throw(Synchronization_Exception) */ {}

  /**
   * Do nothing
   * 
   * @return bool false, always.
   *
   * @exception Synchronization_Exception never thrown
   */
  inline virtual bool tryAcquire(unsigned long) 
    /* throw(Synchronization_Exception) */ { return false; }
  
  /**
   * Do nothing
   * 
   * @exception Synchronization_Exception never thrown
   */
  inline virtual void release() 
    /* throw(Synchronization_Exception) */ {}
  
}; 


} // namespace ZThread

#endif // __ZTNULLLOCKABLE_H__
