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

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "ThreadImpl.h"

namespace ZThread {
  
/**
 * @class Holder
 *
 * @author Eric Crahen <crahen at code-foo dot com>
 * @date <2002-12-21T08:49:58-0500>
 * @version 2.2.11
 *
 * Holders are used to create submit tasks to a thread without
 * transfering thier ownership to the enclosing Handle(s).
 */
template <typename T>
class Holder : public Runnable {

  Runnable& _task;
  
 public:
  
  /**
   * Create a new Holder, wrapping the given reference.
   *
   * @param task Runnable& reference to wrap
   */
  explicit Holder(Runnable& task) : _task(task) { }
  
  //!
  virtual ~Holder() throw();
  
  //!
  virtual void run() throw();

}; // Holder
 

template<typename T>
Holder<T>::~Holder() throw() { }

template<typename T>
void Holder<T>::run() throw() {
  _task.run();
}

//! Generate a Handle and a Holder for some Runnable object
template<typename T>
Handle< Holder<T> > RunnablePtr(T& t) { 
  return Handle< Holder<T> >(new Holder<T>(t)); 
}

  
void Thread::Reference::interrupt() 
  throw() {

  _impl->interrupt();

}


void Thread::Reference::setPriority(Priority p) 
  throw() {

  _impl->setPriority(p);

}


Priority Thread::Reference::getPriority() 
  throw() {

  return _impl->getPriority();
  
}

  
//ThreadLocal<void*> Thread::_interruptKey;

Thread::Thread() : _impl(new ThreadImpl) { }

Thread::~Thread() 
  throw() {

  // Decrement the reference count
  _impl->delReference();

}

bool Thread::join(unsigned long timeout) 
  /* throw(Synchronization_Exception) */ {

  return _impl->join(timeout);

}

void Thread::run() throw() { /* NOOP */ }

void Thread::start() {

  run( RunnablePtr(*this) );

}

void Thread::run(const RunnableHandle& task) {

  _impl->run(task);

}

Thread::Reference Thread::current() 
  throw() {

  return Reference( ThreadImpl::current() );

}


bool Thread::interrupted() 
  throw() {

  return ThreadImpl::current()->isInterrupted();

}


bool Thread::canceled() 
  throw() {

  return ThreadImpl::current()->isCanceled();

}

void Thread::setPriority(Priority n) 
  throw() {

  _impl->setPriority(n);

}


Priority Thread::getPriority() 
  throw() {

  return _impl->getPriority();

}

bool Thread::interrupt() 
  throw() {

  return _impl->interrupt();

}
  
void Thread::cancel() 
  /* throw(Synchronization_Exception) */ {

  if(ThreadImpl::current() == _impl)
    throw InvalidOp_Exception();

  _impl->cancel();

} 

bool Thread::isCanceled() 
  /* throw(Synchronization_Exception) */ {

  return _impl->isCanceled();

}

bool Thread::isDaemon() throw() {

  return _impl->isDaemon();

}

void Thread::setDaemon(bool flag)
  /* throw(Synchronization_Exception) */ {

  // Throw right away if a thread tries to set its own daemon
  // status.
  if(ThreadImpl::current() == _impl)
    throw InvalidOp_Exception();

  _impl->setDaemon(flag);

}


void Thread::sleep(unsigned long ms) 
 /* throw(Synchronization_Exception) */ {

  ThreadImpl::sleep(ms);

}


void Thread::yield() 
  throw() {

  ThreadImpl::yield();

}

} // namespace ZThread 
