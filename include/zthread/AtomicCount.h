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

#ifndef __ZTATOMICCOUNT_H__
#define __ZTATOMICCOUNT_H__

#include "zthread/NonCopyable.h"

namespace ZThread {

/**
 * @class AtomicCount
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-10T09:09:41-0400> 
 * @version 2.2.5
 *
 * This class provides an inferance to a small integer whose value can be
 * incremented or decremented atomically. It's designed to be as simple and
 * lightwieght as possible so that it can be used cheaply to create reference
 * counts.
 */
class AtomicCount : private NonCopyable {
  
  void* _value;
  
  public:
  
  //! Create a new AtomicCount, initialized to a value of 1
  AtomicCount();

  //! Destroy a new AtomicCount
  ~AtomicCount();
  
  //! Increment and return the current value
  void increment();
  
  //! Decrement and return the current value, return true if the count has reached 0
  bool decrement();
  
};

  
} // namespace ZThread

#endif // __ZTATOMICCOUNT_H__
