/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2001, 2002 Eric Crahen, See LGPL.TXT for details
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

#ifndef __ZTTHREADQUEUE_H__
#define __ZTTHREADQUEUE_H__

#include "zthread/Singleton.h"
#include "zthread/Guard.h"
#include "FastLock.h"
#include "ThreadImpl.h"
#include "DeferredInterruptionScope.h"

#include <deque>

namespace ZThread {

class ThreadImpl;
  
/**
 * @class ThreadQueue
 * @version 2.2.11
 * @author Eric Crahen <crahen at code-foo dot com>
 * @date <2002-12-21T09:29:28-0500>
 *
 * A ThreadQueue accumulates references to daemon and reference threads.
 * These are threads that are running outside the scope of the Thread
 * object that created them. ZThreads doesn't have a central manager for
 * all threads (partly why I renamed the ThreadManager to someting more 
 * appropriate). Instead, ZThreads will discover threads it did not create
 * and create a reference thread that allows ZThreads to interact with it.
 * Non daemon threads that are created by the user never have to touch the
 * ThreadQueue.
 */
class ThreadQueue : public Singleton<ThreadQueue> {

  //! Type for the ThreadList
  typedef std::deque<ThreadImpl*> List;

  //! List of daemon threads that need to be released
  List _daemonThreads;

  //! List of reference threads that need to be released
  List _referenceThreads;

  //! Serilize access to the thread list
  FastLock _lock;

  //! Reference thread waiting to cleanup any daemon & reference threads
  ThreadImpl* _waiter;

  //! Number of daemon threads that are known to be executing.
  size_t _pending;

 public:

  ThreadQueue() : _waiter(0), _pending(0) { }

  /**
   * The thread destroys a ThreadQueue will be a reference thread,
   * probably the main thread; but it could be another thread that
   * started and loaded the library.
   */
  ~ThreadQueue() {

    // Make sure the current thread is mapped. Not always the case at
    // this point if threads were started by something other than ZThreads.
    ThreadImpl* impl = ThreadImpl::current();

    bool threadsPending = false;

    { // Check the queue to for pending threads

      Guard<FastLock> g(_lock);

      _waiter = impl;
      threadsPending = _pending != 0;
      
    }


    // Wait for the pending threads to exit
    if(threadsPending) {
      
      Monitor& monitor = _waiter->getMonitor();

      // Defer interruption while this thread waits for a signal from
      // the last pending daemon thread
      Guard<Monitor, CompoundScope<DeferredInterruptionScope, LockedScope> > g(monitor);

      // Reference threads can't be interrupted or otherwise 
      // manipulated. The only signal this monitor will recieve
      // at this point will be from the last pending thread.
      if(monitor.wait() != Monitor::SIGNALED) {
        assert(0);
      }

      // Join those pending threads
      poll();
      
    }

    // Cleanout the reference threads, no need to lock. This should
    // be the last action that happens. If other threads begin starting
    // after this object is destructed, that is probably because another
    // destructor is starting threads - which isn't a valid thing to do.
    pollReferenceThreads(); 

  }

  /**
   * Insert reference thread. Reference threads are not removed until
   * the ThreadQueue goes out of scope.
   *
   * @param ThreadImpl* 
   *
   * @pre The ThreadImpl should already have raised its reference count
   * before being added to the queue.
   */
  void insertReferenceThread(ThreadImpl *impl) {

    Guard<FastLock> g(_lock);
    _referenceThreads.push_back(impl);

  }

  /**
   * Insert a daemon thread that has been completed into this queue. The 
   * underlying native thread will be joined, and the reference count 
   * will be updated (allowing the the thread to be deleted) when the
   * queue is poll()ed.
   *
   * @param ThreadImpl* 
   *
   * @pre The ThreadImpl should already have raised its reference count
   * before being added to the queue.
   */
  void insert(ThreadImpl* impl) {

    Guard<FastLock> g(_lock);
    _daemonThreads.push_back(impl);

    // If this is the last pending thread, signal the waiter
    // if it is exists.
    if(--_pending == 0 && _waiter)
      _waiter->getMonitor().notify();

  }

  /**
   * Poll the queue, joining any daemon threads that have completed. This
   * operation should be relatively quick, the threads present in the queue
   * are known to have exited. If no threads are waiting, the function returns
   * immediately.
   *
   * @post the reference count is updated.
   */
  void poll();

 private:

  void pollReferenceThreads();

};

  void ThreadQueue::poll() {

    Guard<FastLock> g(_lock);

    // Join the threads waiting in the queue
    for(List::iterator i = _daemonThreads.begin(); i != _daemonThreads.end();) {

      ThreadImpl* impl = (ThreadImpl*)*i;
      ThreadOps::join(impl);
      
      // Update the reference count
      impl->delReference();
      
      i = _daemonThreads.erase(i);

    }

    // All threads started are going to poll() before they 
    // execute. 
    _pending++;

  }

  void ThreadQueue::pollReferenceThreads() {

    // Flush the reference threads once they are no longer neccessary
    for(List::iterator i = _referenceThreads.begin(); i != _referenceThreads.end(); ++i) {

      ThreadImpl* impl = (ThreadImpl*)*i;
      impl->delReference();

    }
    
  }

} // namespace ZThread

#endif  // __ZTTHREADQUEUE_H__
