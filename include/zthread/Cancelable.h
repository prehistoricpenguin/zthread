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

#ifndef __ZTCANCELABLE_H__
#define __ZTCANCELABLE_H__

#include "zthread/Exceptions.h"

namespace ZThread {

/**
 * @class Cancelable
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T09:13:37-0400>
 * @version 2.2.0
 *
 * The Cancelable interface defines a common method of adding general <i>disable-and-exit</i>
 * semantics to some object. By cancel()ing a Cancelable object, a request is
 * made to disable that object. A simple description of this interface might be that 
 * it can be used to define an object that can be elegantly shutdown. 
 *
 * The following text describes the specific meaning of <i>disable-and-exit</i> 
 * semantics in more detail. 
 *
 * <b>Disabling</b>
 *
 * A cancel()ed object may not neccessarily abort it work immediately. Often, it much more
 * elegant for a cancel()ed object to complete handling whatever responsiblies have 
 * been assigned to it, but it will <i>not</i> take on any new responsiblity. 
 *
 * <b>Exiting</b>
 *
 * A cancel()ed <i>must</i> complete its responsibilites as soon as possible.
 * Canceling is not just a request to stop taking on new responsibility, and to
 * complete its current responsibility. Its also a request to complete dealing with its 
 * current responsibilites <i>quickly</i>. This should happen in a determinent amount of time
 * and may involve handing that responsibility off to some other object. 
 *
 * The details of these semantics are refined further by specializaions of this 
 * class, but the general conotation remains intact.
 *
 * Typically, the Cancelable interface is used to create a way to shutdown and 
 * exit elegantly. For example, transactions previously assigned to cancel()ed Database 
 * entity would not lost, they would be commited; but new transactions can not be
 * submitted. Similarly, Executor objects can be cancel()ed so that they will finish
 * run()ing thier current tasks, but won't accept new one.
 *
 * Combining the Cancelable interface with the Waitable interface creates a method 
 * for disabling some object, and then waiting for that object to exit (to complete 
 * doing whatever it was doing as soon as possible).
 */
class Cancelable {
public:

  //! Destroy a Cancelable object.
  virtual ~Cancelable() throw() {}

  /**
   * Canceling a Cancelable object is an indication that some part of its 
   * operation should be disabled. Canceling a Cancelable object more than
   * once has no effect.
   *
   * @exception Interrupted_Exception should <i>not</i> be thrown. 
   * @exception Synchronization_Exception thrown if there is some error in 
   * cancel()ing the object.
   *
   * @post The Cancelable object will have permanently transitioned to a 
   * canceled state. 
   */
  virtual void cancel()
    /* throw(Synchronization_Exception) */ = 0;

  /**
   * Determine if a Cancelaeble object has been cancel()ed.
   *
   * @return bool true if cancel() was called prior to this method, otherwise false.
   *
   * @exception Interrupted_Exception should <i>not</i> be thrown. 
   * @exception Synchronization_Exception thrown if there is some error in 
   * cancel()ing the object.
   */
  virtual bool isCanceled()
    /* throw(Synchronization_Exception) */ = 0;

};

} // namespace ZThread

#endif // __ZTCANCELABLE_H__
