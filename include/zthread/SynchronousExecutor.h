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

#ifndef __ZTSYNCHRONOUSEXECUTOR_H__
#define __ZTSYNCHRONOUSEXECUTOR_H__

#include "zthread/Executor.h"
#include "zthread/Guard.h"
#include "zthread/Thread.h"

namespace ZThread {


/**
 * @class SynchronousExecutor
 *
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-29T07:57:31-0700>
 * @version 2.2.2
 *
 * A SynchronousExecutor runs tasks in the current thread, but 
 * allows only one thread to do so at any given time.
 *
 * Submitting a NullTask will allow you to wait() for all real tasks 
 * being executed to complete; and not just to be serviced (started).
 *
 * @see Executor.
 */
template <class LockType>
class SynchronousExecutor : public Executor {
  
  //! Serialize access
  LockType _lock;

  //! Cancellation flag
  bool _canceled;

  public:

  //! Create a new SynchronousExecutor
  SynchronousExecutor() : _canceled(false) {}


  //! Destroy a SynchronousExecutor
  virtual ~SynchronousExecutor() throw() {}

  /**
   * Submit a light wieght task to an Executor. This will not
   * block the calling thread very long. The submitted task will
   * be executed at some later point by another thread.
   * 
   * @exception Cancellation_Exception thrown if a task is submited when 
   * the executor has been canceled.
   * @exception Synchronization_Exception thrown is some other error occurs.
   *
   * @see Executor::execute(RunnableHandle&)
   */
  virtual void execute(const RunnableHandle& task)
    /* throw(Synchronization_Exception) */ {

    
    // Canceled Executors will not accept new tasks, quick 
    // check to avoid excessive locking in the canceled state
    if(_canceled) 
      throw Cancellation_Exception();

    Guard<LockType> g(_lock);

    if(_canceled) // Double check
      throw Cancellation_Exception();

    // Run and destroy the task.
    RunnableHandle local(task);
    local->run();

  }
  
  /**
   * Convience method
   *
   * @see Executor::execute(const RunnableHandle&)
   */
  void execute(Runnable* task)
    /* throw(Synchronization_Exception) */ {

    execute( RunnablePtr(task) );

  }


  /**
   * @see Executor::cancel()
   */
  virtual void cancel() 
    /* throw(Synchronization_Exception) */ {
      
    Guard<LockType> g(_lock);
    _canceled = true;
    
  }
  
  /**
   * @see Executor::isCanceled()
   */
  virtual bool isCanceled()
    /* throw(Synchronization_Exception) */ {
    
    Guard<LockType> g(_lock);
    return _canceled;

  }
 
  /**
   * @see Executor::wait()
   */
  virtual void wait() 
    /* throw(Synchronization_Exception) */ { 

    if(Thread::interrupted())
      throw Interrupted_Exception();
    
    Guard<LockType> g(_lock);

  }

  /**
   * @see Executor::wait(unsigned long)
   */
  virtual bool wait(unsigned long) 
    /* throw(Synchronization_Exception) */ { 

    if(Thread::interrupted())
      throw Interrupted_Exception();

    Guard<LockType> g(_lock);
    return true;

  }


};

} // namespace ZThread

#endif // __ZTSYNCHRONOUSEXECUTOR_H__
