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

#ifndef __ZTINHERITABLETHREADLOCAL_H__
#define __ZTINHERITABLETHREADLOCAL_H__

#include "zthread/ThreadLocal.h"

namespace ZThread {

/**
 * @class InheritableThreadLocal
 *
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-05-30T17:50:21-0400>
 * @version 2.2.0
 *
 * This object acts exactly like its super class ThreadLocal with one subtle
 * difference. That difference is that parent threads values can automatically
 * be propogated to child threads as they are created.
 *
 * The childValue() method allows subclasses to alter values for ThreadLocals
 * that propogated from parent threads to thier children created.
 *
 * @see ThreadLocal
 */
template <class T>
class ZTHREAD_API InheritableThreadLocal : public ThreadLocal<T> {
  public:

  //! Create a new InheritableThreadLocal object
  InheritableThreadLocal() throw() { }

  //! Destroy this InheritableThreadLocal object
  virtual ~InheritableThreadLocal() throw() { }

  protected:

  /**
   * This method will be invoked by the framework when a child thread is 
   * created. If there is a value associated with the parent thread and this 
   * object, then this method is invoked from the childs context, after
   * an implicit call to initialValue().
   *
   * @param parentValue - parent threads value for this ThreadLocal
   * @return value to associate with the current thread (new child) and
   * this object.
   *
   * @pre The parent must have accessed the ThreadLocal at some point, either 
   * with get() or set(), for this method to be invoked. Otherwise, there is 
   * no association with the parent thread and this object - and there is 
   * no value to propogate.
   */
  virtual void* childValue(void* parentValue) const throw() {   
    return parentValue;
  }

  /**
   * Inform the framework this value does propogate to child threads
   *
   * @return true, always 
   */
  virtual bool propogateValue() const throw() {
    return true;
  }

};


} // namespace ZThread

#endif //  __ZTINHERITABLETHREADLOCAL_H__
