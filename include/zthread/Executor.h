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

#ifndef __ZTEXECUTOR_H__
#define __ZTEXECUTOR_H__

#include "zthread/Cancelable.h"
#include "zthread/NonCopyable.h"
#include "zthread/Handle.h"
#include "zthread/Waitable.h"

namespace ZThread {


/**
 * @class Executor
 *
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-04T07:49:11-0400>
 * @version 2.2.2
 *
 * Execeutors are an implementation of the Executor pattern. This is
 * a more versatile construct than a thread pool. I have written a short 
 * paper describing this pattern.
 *
 * Executors abstract the knowledge of <i>how</i>, <i>where</i> and 
 * <i>when</i> to run a task, from the task itself. They are quite
 * versatile objects. Runnable objects can be submitted to an Executor
 * and they will be run by the Executor at some point. 
 * 
 * Executors do not need to be joined. They simply need to be cancel()ed 
 * at some point before the termination of your program.
 *
 * An Executor implements the Cancelable interface, giving you some control
 * over the lifetime of the Executor.
 * 
 * <b>Disabling</b>
 *
 * Cancelling an Executor has the effect of preparing the Executor 
 * to shutdown, <i>disabling</i> it. An Executor that has been cancel()ed 
 * will no longer accept new tasks.
 *
 * <i>Disabling</i> an Executor does not discard the tasks it has previously
 * been assigned. Those tasks will all be executed and are not lost.
 *
 * <b>Exiting</b>
 *
 * An Executor will exit only after it has been cancel()ed. When a cancel()ed 
 * Executor depletes the tasks in its Queue, it will allow its threads to exit.
 * If it is not cancel()ed, its threads will run indefnently, which will 
 * interfere with the clean termination of your program.
 *
 * @see Cancelable
 *
 * An Executor implements the Waitable interface, giving you a way to interact
 * with it. 
 *
 * <b>Waiting</b>
 * 
 * For an Executor, <i>waiting</i> means waiting for the current batch of tasks
 * to be serviced. A thread that wait()s on an Executor will be blocked until 
 * all of its enqueued tasks are being serviced.
 *
 * An Executor can be wait()ed on many times. The completion of a wait() does
 * not neccessarily mean the Executor is done running tasks and its lifetime is
 * about to come to an end. It simply means that it is about to become idle.
 *
 * @see Waitable
 *
 * Cancel()ing and Wait()ing on an Executor will provide a way for an application
 * to detect when an Executor is about to come to the end of its lifetime. A
 * cancel()ed Executor won't accept more tasks, so when the wait() completes 
 * you will know that it's pretty much done doing it job.
 */
class Executor : public Cancelable, public Waitable, private NonCopyable {
public:

  //! Destroy an Executor
  virtual ~Executor() throw() {}

  /**
   * Submit a light wieght task to an Executor. 
   *
   * @param task - command that will be executed 
   *
   * @exception Synchronization_Exception thrown is some other error occurs.
   */
  virtual void execute(const RunnableHandle& task)
    /* throw(Synchronization_Exception) */ = 0;
  
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
   * Stop the execution of additonal tasks by this Executor.
   * A cancel()ed Executor will complete the task that are pending,
   * or executing, but it will not accept any new tasks. 
   *
   * @exception InterruptedException 
   * 
   * @post the Executor no longer accepts new tasks.
   */
  virtual void cancel() 
    /* throw(Synchronization_Exception) */ = 0;


  /**
   * Wait for all pending and currently executing tasks to complete.
   * This method will block the calling thread, for an indefinite amount
   * of time, until no more tasks are available and the Executor is 
   * about to become idle. 
   *
   * However, this method does not garuntee that an Executor is finished 
   * executing tasks. It's possible for more tasks to be submitted after 
   * a wait()ing thread has been awakened. In order to wait() until an 
   * Executor is done running tasks for good, the Executor should first be 
   * cancel()ed.
   *
   * @exception Interrupted_Exception a wait() can be aborted prematurely 
   * by interrupt()ing a wait()ing thread.
   * @exception Synchronization_Exception throw if there is some other
   * error.
   */
  virtual void wait()
    /* throw(Synchronization_Exception) */ = 0;

  /**
   * Wait for all pending and currently executing tasks to complete or
   * or the given amount of time to expire. This method will block the 
   * calling thread, for an definite amount of time, until no more tasks 
   * are available and the Executor is about to become idle. 
   *
   * However, this method does not garuntee that an Executor is finished 
   * executing tasks. It's possible for more tasks to be submitted after 
   * a wait()ing thread has been awakened. In order to wait() until an 
   * Executor is done running tasks for good, the Executor should first be 
   * cancel()ed.
   *
   * @param timeout unsigned long - maximum amount of time (milliseconds)
   * to wait() for an Executor. 
   * 
   * @return true if the wait() completes in the given amount of time; 
   * otherwise false.
   *
   * @exception Interrupted_Exception a wait() can be aborted prematurely 
   * by interrupt()ing a wait()ing thread.
   * @exception Synchronization_Exception throw if there is some other
   * error.
   */
  virtual bool wait(unsigned long)
    /* throw(Synchronization_Exception) */ = 0;

};

} // namespace ZThread

#endif // __ZTEXECUTOR_H__
