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

#ifndef __ZTTHROWABLE_H__
#define __ZTTHROWABLE_H__

namespace ZThread { 

/**
 * @class Throwable
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-04T08:47:37-0400>
 * @version 2.2.0
 * 
 * Throwables are used to propogate a generic exception from the context of 
 * one thread to another.
 *
 * @see Future
 */
class Throwable {
public:

  //! Destroy a Throwable
  virtual ~Throwable() {}

  /**
   * Propogate the exception by throwing it.
   *
   * @exception this will always throw, but it can throw
   * anything as an exception.
   */
  virtual void propogateException() = 0;

};


/**
 * @class ThrowableFactory
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-04T08:47:37-0400>
 * @version 2.2.0
 *
 * The ThrowableFactory is used to create a Throwable wrappers for 
 * any exception whose type is known. This allows it to be transfered 
 * to the context of another thread.
 *
 * @see Future
 * @see Throwable
 */
class ThrowableFactory {

  //! Helper class
  template <class T>
  class Holder : public Throwable {
    
    T t;
    
  public:

    /**
     * Create a new Holder for an exception.
     *
     * @param x object of type T that will be wrapped as 
     * a Throwable object.
     */
    Holder(const T& x) : t(x) { }
    
    /**
     * Propogate the exception by throwing it.
     *
     * @exception an exception of type T will be thrown.
     */
    virtual void propogateException() {
      throw t;
    }
    
  };
  
public:

  /**
   * Generate a Throwable object that will throw the parameter
   * passed to this method when propogateException() is called
   * on the resulting Throwable.
   *
   * @param x object to throw
   */
  template <class T>
  static Throwable* create(const T& x) {
    return new Holder<T>(x);  
  }

};

} // namespace ZThread

#endif // __ZTTHROWABLE_H__
