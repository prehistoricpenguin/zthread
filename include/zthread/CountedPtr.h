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

#ifndef __ZTCOUNTEDPTR_H__
#define __ZTCOUNTEDPTR_H__

#include "zthread/AtomicCount.h"
#include "zthread/Guard.h"

namespace ZThread {
  
/**
 * @class CountedPtr
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-10T09:14:50-0400>
 * @version 2.2.5
 *
 * Thread-safe wrapper to implement a counted reference, this
 * is limited in order to keep the lock allocated on the heap and
 * to keep it from causing trouble with static destructors. This
 * is immutable in the sense that it reassignment is not allowed.
 * A CountedPtr will always refer to the same object.
 */
template <class T>
class CountedPtr {

  CountedPtr& operator=(const CountedPtr& ptr);

  AtomicCount* _count;

 protected:

  T*  _instance;

 public:

  //! Create a CountedPtr
  CountedPtr() : _count(new AtomicCount) {
    
    _instance = new T;
    
  };

  //! Create a CountedPtr
  explicit CountedPtr(T* ptr) : _count(new AtomicCount) {
    
    _instance = ptr;
    
  };
  
  //! Create a CountedPtr
  CountedPtr(const CountedPtr& ptr) {
    
    ptr._count->increment();

    _instance  = ptr._instance;
    _count = ptr._count;
    
  }
  
  /**
   * Destroy the CountedPtr and check for release of the implementation
   */
  virtual ~CountedPtr() {

    if(_count->decrement()) {

      delete _instance;
      delete _count;

    }
    
  };
  
  /**
   * Get a reference to the underlying implementation
   */
  T* operator->() throw() { return _instance; }
  
  /**
   * Get a reference to the underlying implementation
   */
  const T* operator->() const throw() { return _instance; }
 
  
}; 

} // namespace ZThread

#endif // __ZTCOUNTEDPTR_H__
