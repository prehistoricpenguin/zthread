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

#ifndef __ZTPOOLEXECUTOR_H__
#define __ZTPOOLEXECUTOR_H__

#include "zthread/DefaultThreadFactory.h"
#include "zthread/Executor.h"
#include "zthread/MonitoredQueue.h"
#include "zthread/CountedPtr.h"
#include "zthread/Singleton.h"
#include "zthread/Thread.h"
#include "zthread/Mutex.h"

#include <assert.h>

namespace ZThread {

/**
 * @class PoolExecutor
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2003-06-30T08:14:12-0400>
 * @version 2.2.2
 *
 * This is an Executor that will run submitted tasks using a group 
 * of threads.
 *
 * Submitting a NullTask will allow you to wait() for all real tasks 
 * being executed to complete; and not just to be serviced (started).
 *
 * @see Executor
 */
template <
  class LockType = Mutex, 
  class QueueType = MonitoredQueue<RunnableHandle*, LockType>, 
  typename RefType = CountedPtr<QueueType> 
>
class PoolExecutor : public Executor {

  //! Typedef
  typedef typename std::deque<Thread*> WorkerList;

  //! Worker threads
  WorkerList _activeWorkers;

  //! Serialize the thread queue
  LockType _lock;

  //! Reference to the Queue 
  RefType _queue;

  //! Minimum
  volatile unsigned int _min;

  //! Maximum
  volatile unsigned int _max;

  //! Canceled
  volatile bool _canceled;

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

  /**
   * Create a new PoolExecutor
   *
   * @param min - minimum number of threads to service tasks with
   */
  PoolExecutor(unsigned int min)
    /* throw(Synchronization_Exception) */ 
    : _min(min), _max(min), _canceled(false) {}

  /**
   * Create a new PoolExecutor
   *
   * @param min - minimum number of threads to service tasks with
   * @param max - maximum number of threads to service tasks with
   */
  PoolExecutor(unsigned int min, unsigned int max)
    /* throw(Synchronization_Exception) */
    : _min(min), _max(max), _canceled(false) {}

  /**
   * Change the maximum number of threads in the pool. As tasks are
   * added to the Executor, more threads will be added as needed -
   * up to the maximum number of worker threads set.
   *
   * @param unsigned int - new maximum number of worker threads.
   *
   * @exception Interrupted_Exception
   */
  void setMax(unsigned int max)
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    _max = max;

  }

  /**
   * Change the minimum number of threads in the pool. When currently
   * running tasks complete, and the Thread executing the task completes
   * it can be removed. A Thread will never be instantly canceled because
   * the minimum number of threads has changed. It is a gradual decline
   * in the number of worker threads, they are canceled lazily.
   *
   * @param unsigned int - new minimum number of worker threads.
   *
   * @exception Interrupted_Exception
   */
  void setMin(unsigned int)
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    _min = min;

  }

  /**
   * Get the current maximum number of threads
   *
   * @return unsigned int - current maximum
   *
   * @exception Interrupted_Exception
   */
  unsigned int getMax()
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    return _max;

  }

  /**
   * Get the current minimum number of threads
   *
   * @return unsigned int - current minimum
   *
   * @exception Interrupted_Exception
   */
  unsigned int getMin()
    /* throw(Synchronization_Exception) */ {

    Guard<LockType> g(_lock);
    return _min;

  }

  //! Destroy a new PoolExecutor
  virtual ~PoolExecutor() throw() { 

    for(typename WorkerList::iterator i = _activeWorkers.begin(); i != _activeWorkers.end(); ++i) 
      delete *i;

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

    {
      
      Guard<LockType> g(_lock);
      
      // Canceled Executors will not accept new tasks
      if(_canceled)
        throw Cancellation_Exception();
      
      // Increase threads working threads to minimum
      while(_activeWorkers.size() < _min)
        addWorker();

      // Remove any threads over the maximum
      if(_activeWorkers.size() > _max)
        removeWorker();

    }

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

    Guard<LockType> g(_lock);

    _queue->cancel(); 
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
      
    _queue->empty();
      
  }

  /**
   * @see Executor::wait(unsigned long)
   */
  virtual bool wait(unsigned long timeout) 
    /* throw(Synchronization_Exception) */ { 

      return _queue->empty(timeout); 

    }


private:

  
  void addWorker() {

    Thread* t = Singleton<FactoryType>::instance()->create();

    // Assign the daemon thread ownership of a Worker task.
    t->run( RunnablePtr(new Worker(_queue)) );

    _activeWorkers.push_back(t);

  }

  void removeWorker() {

    Thread* t =_activeWorkers.front();
    _activeWorkers.pop_front();

    t->cancel();
    delete t;

  }
      
};

} // namespace ZThread

#endif // __ZTPOOLEXECUTOR_H__




