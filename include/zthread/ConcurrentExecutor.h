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

#ifndef __ZTCONCURRENTEXECUTOR_H__
#define __ZTCONCURRENTEXECUTOR_H__

#include "zthread/DefaultThreadFactory.h"
#include "zthread/Executor.h"
#include "zthread/MonitoredQueue.h"
#include "zthread/CountedPtr.h"
#include "zthread/Singleton.h"
#include "zthread/Thread.h"

#include <assert.h>

namespace ZThread {

/**
 * @class ConcurrentExecutor
 *
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-06-19T08:46:14-0400>
 * @version 2.2.2
 *
 * This is an Executor that will run submitted tasks in another 
 * thread. Only one thread is used per ConcurrentExecutor. The
 * PoolExecutor will allow you to create an Executor that will
 * service tasks with one or more threads.
 *
 * @see Executor
 */
template <
  class LockType, 
  class FactoryType = DefaultThreadFactory,
  class QueueType = MonitoredQueue<RunnableHandle*, LockType>, 
  typename RefType = CountedPtr<QueueType> 
>
class ConcurrentExecutor : public Executor {

  //! Reference to the Queue 
  RefType _queue;

  //! Thread to dispatch the work to
  Thread* _thread;

  //! Helper class
  class Worker : public Runnable {

    RefType _queue;

  public:

    //! Create a Worker that draws upon the given Queue
    Worker(RefType& q) : _queue(q) { }
    
    //! Destroy the Worker
    virtual ~Worker() throw() { }

    //! Run until interrupted
    virtual void run() throw() {

      RunnableHandle* task = 0;

      try {
        
        while(!Thread::canceled()) { 
          
          // Draw tasks from the queue
          task = _queue->next();
          (*task)->run();

          delete task;
          task = 0;

        } 

      } catch(Interrupted_Exception&) {
        // Thread canceled while drawing from the Queue
      } catch(Cancellation_Exception&) { 
        // Queue has emptied.
      } catch(...) {
        assert(0);
      }

      if(task) 
        delete task;

    }

  }; // Worker

public:

  //! Create a new ConcurrentExecutor
  ConcurrentExecutor()
    /* throw(Synchronization_Exception) */ {

    _thread = Singleton<FactoryType>::instance()->create();
    _thread->setDaemon(true);

    // Assign the daemon thread ownership of a Worker task.
    _thread->run( RunnablePtr(new Worker(_queue)) );

  }

  //! Destroy a new ConcurrentExecutor
  virtual ~ConcurrentExecutor() throw() { 

    delete _thread;

  }

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

    // Canceled Executors will not accept new tasks
    if(_queue->isCanceled()) 
      throw Cancellation_Exception();

    // Enqueue the task
    _queue->add(new RunnableHandle(task)); 

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
      
      _queue->cancel(); 

    }

  /**
   * @see Executor::isCancel()
   */
  virtual bool isCanceled()
    /* throw(Synchronization_Exception) */ { 

      return _queue->isCanceled(); 

    }
 
  /**
   * @see Executor::wait()
   */
  virtual void wait() 
    /* throw(Synchronization_Exception) */ { 
      
    _queue->empty();
      
  }

  /**
   * @see Executor::wait(unsigned long)
   */
  virtual bool wait(unsigned long timeout) 
    /* throw(Synchronization_Exception) */ { 

      return _queue->empty(timeout); 

    }
      
};

} // namespace ZThread

#endif // __ZTCONCURRENTEXECUTOR_H__




