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

#ifndef __ZTHANDLE_H__
#define __ZTHANDLE_H__

#include "zthread/CountedPtr.h"
#include "zthread/Runnable.h"

namespace ZThread {

//!
typedef CountedPtr<Runnable> RunnableHandle;

/**
 * @class Handle
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-10T09:15:24-0400>
 * @version 2.2.5
 *
 * Handles are used to reference count Runnable objects using a
 * CountedPtr. Handles are immutable, once created you cannot change
 * the object they are assigned to. This helps to significantly reduce 
 * any overhead involved in managing the reference count. If assignment is 
 * desireable, a user could develop a smart pointer class that work with 
 * Handles.
 *
 * Handles can only be used with Runnable classes. The compiler will
 * not allow you use a Handle with a class that does not inherit from 
 * Runnable.
 */
template <typename T>
class Handle : public CountedPtr<Runnable> {

  Handle();

  public:

  //! Create a Handle
  explicit Handle(T* ptr) : CountedPtr<Runnable>(ptr) { }
  
  //! Create a Handle
  Handle(const Handle& h) : CountedPtr<Runnable>(h) { }

  virtual ~Handle() throw() { }

  //!
  T* operator->() throw() { return reinterpret_cast<T*>(_instance); }

  //!
  const T* operator->() const throw() { return reinterpret_cast<T*>(_instance); }

};

//! Generate a Handle for some Runnable object
template<typename T>
Handle<T> RunnablePtr(T* t) { 
  return Handle<T>(t); 
}

}; 

#endif // __ZTHANDLE_H__



