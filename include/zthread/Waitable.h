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

#ifndef __ZTWAITABLE_H__
#define __ZTWAITABLE_H__

#include "zthread/Exceptions.h"

namespace ZThread { 

/**
 * @class Waitable
 *
 * @author Eric Crahen <crahen at code-foo dot com>
 * @date <2002-12-21T04:50:52-0500>
 * @version 2.2.11
 *
 * The Waitable interface defines a common method of adding general <i>wait</i> semantics
 * to an object. This is simply defined as waiting for some condition to occur,
 * however the definition of that conditon is fluid in that it is left to
 * specialization of this class to define.
 * 
 * The following text describes the specific meaning of <i>wait</i> semantics in more detail. 
 *
 * <b>Waiting</b>
 *
 * An object implementng the Waitable interface externalizes a mechanism for testing
 * some internal condition. Another object may <i>wait()</i>s for a Waitable object; 
 * in doing so, it wait()s for that condition to become true by blocking the caller 
 * while the condition is false.
 *
 * Waitable objects may <b>never</b> <i>wait</i> for themselves. For example, 
 * an Waitable object that calls its own wait() method in its constructor or 
 * destructor would be considered to be waiting for itself. This may or may not lead 
 * to deadlock (depending on how an object extends <i>wait</i> semantics), but it should 
 * never be neccessary. A Waitable object is already privy to the condition it is 
 * exposing a waiting mechanism for; in other words, the information is already available
 * within the object so it does not need to wait.
 *
 * Other than the previous rule, Waiting semantics do <i>not</i> neccessarily provide 
 * any garuntees regarding nor does it place any restrictions on thread safety, 
 * preconditions, postconditions or even what constitutes the condition being wait()ed for
 * or how a wait() is accomplised. It merly establishes the base for framework of more specialized
 * meanings for <i>wait</i>.
 *
 * The details of these semantics are refined further by specializaions of this 
 * class, but the general conotation remains intact. 
 *
 * For example, a Condition is Waitable object that extends <i>wait</i> semantics
 * so that wait()ing means a thread is blocked until some external stimulus 
 * specifically performs an operation on the Condition to make its internal condition true. 
 * (serialization aside)
 *
 * A Future extends <i>wait</i> semantics so that wait()ing means that a task completed 
 * somewhere and that some action be taken prior to the wait.
 *
 * A Barrier extends <i>wait</i> semantics so that wait()ing mean waiting for other 
 * waiters, and may include automatically resetting the condition once a wait is complete.
 *
 * Combining the Cancelable interface with the Waitable interface creates a method 
 * wait for an Excutor to complete its responisiblities, executing, which really means waiting 
 * for a number of things all at once.
 */
class Waitable {
  public:  
  
  //! Destroy a Waitable object.  
  virtual ~Waitable() throw() {}

  /**
   * Wait()ing on this object will cause the calling thread to be blocked
   * for some indefinite period of time. The thread executing will not proceed
   * any further until the Waitable object releases it unless and exception
   * is thrown.
   *
   * The semantics for how and when a Waitable object decides to release a 
   * blocked thread are particular to each specialization of this class. The
   * Waitable interface itself does not imply any garuntees about the state 
   * of the environment (serial or otherwise); however specializations of
   * this interface may do so.
   *
   * @exception Interrupted_Exception 
   * This exception may be thrown if the wait is aborted prematurely do to
   * the thread being interrupt()ed before it begins or during the wait.
   * @exception Synchronization_Exception 
   * Thrown if the wait cannot be accomplished because of some other error.
   */
  void wait() 
    /* throw(Synchronization_Exception) */;


  /**
   * Wait()ing on this object with a timeout will cause the calling thread 
   * to be blocked for, at most, some definite period of time. The thread 
   * executing will not proceed any further until the Waitable object releases 
   * it unless and exception is thrown.
   *
   * The semantics for how and when a Waitable object decides to release a 
   * blocked thread are particular to each specialization of this class. The
   * Waitable interface itself does not imply any garuntees about the state 
   * of the environment (serial or otherwise); however specializations of
   * this interface may do so.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   *
   * @return bool, true if the wait() completed before the timeout expired.
   * false otherwise.
   *
   * @exception Interrupted_Exception 
   * This exception may be thrown if the wait is aborted prematurely do to
   * the thread being interrupt()ed before it begins or during the wait.
   * @exception Synchronization_Exception 
   * Thrown if the wait cannot be accomplished because of some other error.
   */
  bool wait(unsigned long) 
    /* throw(Synchronization_Exception) */;

  
};


} // namespace ZThread

#endif //  __ZTWAITABLE_H__
