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


#ifndef __ZTTHREADIMPL_H__
#define __ZTTHREADIMPL_H__

#include "zthread/Handle.h"
#include "zthread/Exceptions.h"
#include "zthread/IntrusivePtr.h"

#include "Monitor.h"
#include "TSS.h"
#include "ThreadOps.h"
#include "State.h"
#include "ThreadLocalMap.h"

namespace ZThread {

/**
 * @class ThreadImpl
 * @author Eric Crahen <crahen at code-foo dot com>
 * @date <2002-12-21T04:55:36-0500>
 * @version 2.2.11
 */
class ThreadImpl : public IntrusivePtr<ThreadImpl, FastLock>, public ThreadOps {

  //! TSS to store implementation to current thread mapping.
  static TSS<ThreadImpl*> _threadMap;

  //! The Monitor for controlling this thread
  Monitor _monitor;
  
  //! Current state for the thread
  State _state;

  //! Joining thread, if any.
  ThreadImpl* _joiner;

  //! Mapping of the ThreadLocal associations
  ThreadLocalMap _localValues;

  //! Cached thread priority
  Priority _priority;

 public:

  //! Create a new ThreadImpl
  ThreadImpl() 
    /* throw(Synchronization_Exception) */;

  //! Destroy a new ThreadImpl
  ~ThreadImpl() throw();  

  //! Get a reference to this threads Monitor
  Monitor& getMonitor();

  //! Set the CANCELED status of the monitor
  void cancel() throw();

  //! Set the INTERRUPTED status of the monitor
  bool interrupt() throw();

  //! Check & clear the INTERRUPTED status of the monitor
  bool isInterrupted() throw();

  //! Check the CANCELED status of the monitor
  bool isCanceled() throw();

  //! Get the current priority, not serialized, should be replaced
  //! with an atomic operation
  Priority getPriority() const;

  //! Get a reference to the ThreadLocalMap
  ThreadLocalMap& getThreadLocalMap();
  
  bool join(unsigned long) 
    /* throw(Synchronization_Exception) */;
  
  void run(const RunnableHandle& task) 
    /* throw(Synchronization_Exception) */;

  void setPriority(Priority);

  bool isActive() throw();

  bool isDaemon() throw();
  
  //! Test for a reference thread 
  bool isReference() throw();

  void setDaemon(bool) 
    /* throw(Synchronization_Exception) */;

  static void sleep(unsigned long) 
    /* throw(Synchronization_Exception) */;

  static void yield() throw();
  
  static ThreadImpl* current() throw();

  static void dispatch(ThreadImpl*, ThreadImpl*, const RunnableHandle&);

};

} // namespace ZThread 

#endif // __ZTTHREADIMPL_H__
