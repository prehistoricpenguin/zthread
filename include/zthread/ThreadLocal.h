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

#ifndef __ZTTHREADLOCAL_H__
#define __ZTTHREADLOCAL_H__

#include "zthread/AbstractThreadLocal.h"

namespace ZThread {

/**
 * @class ThreadLocal
 *
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-05-30T17:50:36-0400>
 * @version 2.2.0
 *
 * Provides a method to access the local storage of each thread. No matter
 * what thread access this object, it will always store values unique to
 * each thread. This is done using the local storage allocated by the OS
 * for each thread. It doesn't require any locking and is very fast
 *
 * The first time a ThreadLocal veriable is accessed by a thread the initialValue()
 * method will be invoked. This allows subclasses to perfrom any special 
 * actions they might need to when a new thread uses one of these variables.
 *
 * The destroyValue() method is invoked when a thread that has 
 * used a ThreadLocal is about to exit. 
 *
 * The recommended usage of this object would be something like in the
 * example shown below
 *
 * @code
 *
 * class MyClass {
 * protected:
 *
 * static ThreadLocal<int> _threadKey;
 *
 * public:
 * 
 * int getValue() const {
 *   return _threadKey.get();
 * }
 * 
 * int setValue(int n) const {
 *   return _threadKey.set(n);
 * }
 *
 * };
 *
 * @endcode
 *
 * The ThreadLocal object itself acts as the key, instead of some arbitrary
 * integer that needed to be defined by the programmer before. This is a
 * much more elegant solution.
 *
 * @see AbstractThreadLocal
 */
template <class T>
class ThreadLocal : protected AbstractThreadLocal {
 public:

  //! Destroy this ThreadLocal.
  virtual ~ThreadLocal() throw() { }

  /**
   * Get the value associated with the current thread and this object via fetch().
   * If no association exists, then initialValue() is invoked.
   *
   * @return T associated value
   *
   * @exception Synchronization_Exception - thrown if there is an error
   * allocating native thread local storage
   */
  inline T get() const throw() {
    return reinterpret_cast<T>( AbstractThreadLocal::get() );
  }
  
  /**
   * Set the value associated with the current thread and this object. This value 
   * can only be retrieved from the current thread.
   *
   * @param T new associated value
   * @return T old associated value, if no association exists, then initialValue() 
   * is invoked.
   *
   * @exception Synchronization_Exception - thrown if there is an error
   * allocating native thread local storage
   */
  inline T set(T val) const throw() {
    return reinterpret_cast<T>( AbstractThreadLocal::set((void*)val) );
  }

  protected:

  /**
   * Invoked by the framework the first time a get() or set() is invoked on a 
   * ThreadLocal variable from a particular thread. 
   *
   * @return void* - value that will be set for the executing thread
   */
  inline virtual void* initialValue() const throw() {   
    return 0;
  }

};


} // namespace ZThread

#endif // __ZTTHREADLOCAL_H__
