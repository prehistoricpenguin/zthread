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

#ifndef __ZTCANCELABLETASK_H__
#define __ZTCANCELABLETASK_H__

#include "zthread/Cancelable.h"
#include "zthread/Guard.h"
#include "zthread/FastMutex.h"
#include "zthread/Runnable.h"
#include "zthread/Thread.h"

namespace ZThread {


/**
 * @class CancelableTask
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-02T11:24:17-0400>
 * @version 2.2.1
 *
 * A CancelableTask can be used to create a task with its own Cancelable 
 * semantics. Instead of implementing the run() method as you normally
 * would for a Runnable object, implement the doRun() method instead.
 *
 * <b>Disabling</b>
 * 
 * A cancel()ed CancelableTask will not execute if is is canceled prior to 
 * being run(). If it is canceled while it is running, the thread executing 
 * the task will be interrupted. This gives the executing task a chance to 
 * respond to being canceled.
 *
 * <b>Exiting</b>
 *
 * A canceled task should exit cleanly and quickly. Depending on the task 
 * this may involve rolling back to some previous state, updating a log, etc.
 * Its up to the specific task to decide how to best handle being canceled
 * during execution.
 *
 * @see cancel()
 * @see Cancelable
 */
class CancelableTask : public Runnable, public Cancelable {

  volatile bool _canceled;

  FastMutex _lock;
  Thread::Reference* _current;

 public:
  
  //! Create a CancelableTask
  CancelableTask() : _canceled(false), _current(0) { }

  //! Destroy the CancelableTask
  virtual ~CancelableTask() throw() {  }

  /**
   * Cancel this task. This will be successful only once, cancel()ing multiple
   * times will result in an exception. 
   *
   * A task that has been canceled will not run.
   * A task that is executing will be interrupted.
   * 
   * @exception InvalidOp_Exception thrown if the task is canceled more than
   * once.
   *
   * @post it is possible for an interrupted task not to clear the executing
   * thread's <i>interrupted</i> status (it may not check Thread::interrupted()
   * or it may not use blocking operations). Executors will not be affected by
   * this by design; a good way to avoid problems this may cause is to check
   * Thread::canceled() if you plan on repeated running tasks. This allows the 
   * executing thread to distinguish between an interrupt from being cancel()ed
   * and an interrupt() left over from a CancelableTask. 
   */
  virtual void cancel()
    /* throw(Synchronization_Exception) */ {

    if(_canceled)
      throw InvalidOp_Exception();

    Guard<FastMutex> g(_lock);

    if(_canceled)
      throw InvalidOp_Exception();

    _canceled = true;

    if(_current)
      _current->interrupt();

  }


  /**
   * Check the canceled status of this task.
   *
   * @return bool true if the task has been canceled
   */
  virtual bool isCanceled()
    /* throw(Synchronization_Exception) */ {

    if(_canceled)
      return true;

    Guard<FastMutex> g(_lock);
    return _canceled;

  }

  /**
   * A CancelableTask will run only if it has not been canceled prior
   * to calling this function. If the task has not been canceled at 
   * the time this function is called, then doRun() is executed.
   */
  virtual void run() throw() {

    Guard<FastMutex> g1(_lock);
    
    // Abort the task if canceled
    if(_canceled) 
        return;
    
      // Make a pointer to the Thread running this task available to
      // another context, only for the duration of this task.
    Thread::Reference self(Thread::current());
    _current = reinterpret_cast<Thread::Reference*>
      (&const_cast<char&>(reinterpret_cast<const volatile char &>(self)));
    
    {
      
      Guard<FastMutex, UnlockedScope> g2(g1);
      
      doRun();
      
    }
    
    _current = 0;
    
  }

 protected:

  /**
   * Specializations of this class must define this method. It is 
   * called when a CancelableTask that has not been canceled is run().
   * 
   * @see Runnable::run() 
   */
  virtual void doRun() throw() = 0;

};
 
}; 

#endif // __ZTCANCELABLETASK_H__



