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

#ifndef __ZTABSTRACTTHREADLOCAL_H__
#define __ZTABSTRACTTHREADLOCAL_H__

#include "zthread/Config.h"
#include "zthread/NonCopyable.h"

namespace ZThread {

/**
 * @class AbstractThreadLocal
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-05-30T17:33:30-0400>
 * @version 2.2.0
 *
 * This defines a common base class that helps to support the behavior 
 * ThreadLocal objects.
 * 
 * @see ThreadLocal
 * @see InheritableThreadLocal
 */
class ZTHREAD_API AbstractThreadLocal : private NonCopyable {
 public:

  //! Create a new AbstractThreadLocal
  AbstractThreadLocal();
  
  //! Destroy this AbstractThreadLocal
  virtual ~AbstractThreadLocal() throw();
  

  /** 
   * Invoked by the framework the first time get() is invoked by the
   * current thread, if no child value has been propgated. The value 
   * return is associated with the current thread and this object. 
   */
  virtual void* initialValue() const throw() = 0;

  /** 
   * Invoked by the framework whenever the value associated with current
   * thread and object is about to propogate to a child thread. This 
   * method is not invoked if there is no value associated with the
   * current thread and this object.
   *
   * @param void* - related initialization value
   */
  virtual void* childValue(void* parentValue) const throw() {
    return (void*)0;
  }


  /** 
   * Invoked by the framework whenever it needs to be determined wether or not 
   * this object should propogate values to child threads.
   *
   * @param bool - true if this value should propgate, otherwise false. Unless
   * overidden, this method will always return false
   */
  virtual bool propogateValue() const throw() {
    return false;
  }

  /**
   * Invoked by the framework when a thread that has set a value for a ThreadLocal
   * is about to exit. The initialValue() or childValue() methods will set values 
   * implicitly and therefore cause this method to be invoked. This acts as a 
   * point to insert some some last chance operation for a ThreadLocal, and is often 
   * useful as a cleanup point.
   *
   * @param currentValue - value associated with the current thread and this 
   * object. 
   */
  virtual void destroyValue(void* currentValue) const throw() {
    // Do nothing by default
  }

 protected:

  /**
   * Get the value associated with the current thread and this object via fetch().
   * If no association exists, then initialValue() is invoked.
   *
   * @exception Synchronization_Exception - thrown if there is an error
   * allocating native thread local storage
   */
  void* get() const throw();
  
  /**
   * Set the value associated with the current thread and this object. This value 
   * can only be retrieved from the current thread.
   *
   * @exception Synchronization_Exception - thrown if there is an error
   * allocating native thread local storage
   */
  void* set(void*) const throw();

};


} // __ZTABSTRACTTHREADLOCAL_H__

#endif

