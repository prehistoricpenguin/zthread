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

#ifndef __ZTREADWRITELOCK_H__
#define __ZTREADWRITELOCK_H__

#include "zthread/Lockable.h"
#include "zthread/NonCopyable.h"

namespace ZThread {
  
/**
 * @class ReadWriteLock
 * 
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-19T07:39:55-0400>
 * @version 2.2.7
 *
 * Read/Write lock provide a set of coordinated Lockable object that
 * allow many Readers while there is no update in progress, but only 1 
 * Writer & 0 Readers during an update
 *
 * Because the general case where an ReadWriteLock would be used is one where 
 * you have more readers than writers, this lock will have a bias toward
 * readers
 */
  
class ReadWriteLock : public NonCopyable {
 public:

  /**
   * Create a new ReadWriteLock
   *
   * @exception Initialization_Exception - thrown if resources could 
   * not be allocated
   */
  ReadWriteLock() {}
    /* throw(Synchronization_Exception) */;
  
  /**
   * Destroy this ReadWriteLock
   */
  virtual ~ReadWriteLock() throw() {} 
  
  /**
   * Get a reference to the read lock
   *
   * @return Lockable& read lock
   */
  virtual Lockable& getReadLock() = 0;

  /**
   * Get a reference to the write lock
   *
   * @return Lockable& write lock
   */
  virtual Lockable& getWriteLock() = 0;

      
}; /* ReadWriteLock */


}; // __ZTREADWRITELOCK_H__

#endif







