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

#ifndef __ZTFUTURE_H__
#define __ZTFUTURE_H__

#include "zthread/Condition.h"
#include "zthread/CountedPtr.h"
#include "zthread/Executor.h"
#include "zthread/FastMutex.h"
#include "zthread/Guard.h"
#include "zthread/Throwable.h"

namespace ZThread {

/**
 * @class Future
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-10T09:21:16-0400>
 * @version 2.2.3
 *
 * A Future acts as a placeholder for a result that is being calculated, or 
 * a result that will be calculated. Think of this as an IOU for some value.
 * Futures also allow you to take an exception thrown in one context (the 
 * context of the thread executing the task thats supposed to procude a result)
 * and will propogate it to all thread wait()ing for a result from the Future.
 *
 * <b>Waiting</b>
 *
 * Waiting for a Future means to wait for the completion of a Callable task.
 * When the Callable tasks reports a result or an exception, wait() will no 
 * longer block. If an exception was set, it will be thrown. If a result was
 * set, it will be obtainable.
 *
 * @see Waitable
 *
 * Currently, there is an example in tests/FutureTests.cxx 
 */
template <typename ResultType>
class Future : public Waitable {
 public:

  /**
   * @class Result
   *
   * An instance of Result is used to recieve and eventually hold the value 
   * or the exception produced by a Callable. 
   */
  class Result {
    
    friend class Future;

    ResultType _value;
    bool _haveResult;

    FastMutex _lock;
    Condition _cond;

    Throwable* _exception;

  public:
    
    //! Create the Result
    Result() : _haveResult(false), _cond(_lock), _exception(0) { }

    //! Destroy the Result
    ~Result() {

      if(_exception)
        delete _exception;

    }

    /**
     * Setting the result will release all waiters.
     *
     * @param value - made available to the waiters with 
     * Future::getResult()
     */
    void setResult(const ResultType& value) {

      Guard<FastMutex> g(_lock);

      _value = value;

      _haveResult = true;
      _cond.broadcast();

    }


    /**
     * Setting the exception will release all waiters.
     *
     * @param t - exception to propogate to threads waiting
     * for results.
     *
     * @post thread blocked on Future::getResult() will 
     * awaken with an exception.
     */
    template <typename T>
    void setException(const T& t) {
      
      Guard<FastMutex> g(_lock);

      _exception = ThrowableFactory::create(t);

      _haveResult = true;
      _cond.broadcast();

    }
 
    /**
     * Get the result or throw the exeption.
     *
     * @return ResultType
     */
    ResultType getResult() {

      Guard<FastMutex> g(_lock);

      if(!_haveResult)
        _cond.wait();

      if(_exception)
        _exception->propogateException();

      return _value;

    }

  }; // Result

  //! Reference counting holder for results
  typedef CountedPtr<Result> ResultRef;

  /**
   * @class Callable
   *
   * The Callable is the portion of the Future that is responible for actually
   * performing that work, and setting the results.
   */
  class Callable {
  public:

    //! Destroy the Callable
    virtual ~Callable() throw() {}

    /**
     * Invoked to perform the work involved in obtaining a value for a Future.
     * 
     * @param result - Result object used to receive values or exceptions from
     * this Callable. As soom as the result is used to set a value, or exception, 
     * any threads waiting on the Future::getResult() function will be awakened
     * and will either have a value or will throw an exception.
     */
    virtual void call(ResultRef& result) = 0;

  }; // Callable

  //! Reference counting holder for results
  typedef CountedPtr<Callable> CallableRef;

 private:


  //! shared between the Future, held by the user, and the Result 
  ResultRef _result;

  //typedef void (Callable::*CallableType)(ResultType&);

  /**
   * @class Task
   *
   * An instance of FutureTask encloses a Callable object and is 
   * submitted to an Executor.
   */
  class Task : public Runnable {

    CallableRef _callable;
    ResultRef _result;

  public:
    
    //!
    Task(const CallableRef& c, const ResultRef& result) : 
      _callable(c), _result(result) { }

    //!
    virtual ~Task() throw() { }

    //! Delegate to the callable object
    virtual void run() throw() {
      
      _callable->call(_result);

    }

  }; // Task

 public:

  /**
   * Create a Future that will Execute the given Callable object by 
   * wrapping it with a Runnable and submitting it to an Executor.
   *
   * @param c - Callable to execute
   * @param executor - Executor to submit the task to.
   *
   * @pre Currently, callables are passed by value.
   */
  Future(const CallableRef& c, Executor& executor) {

    executor.execute( RunnablePtr(new Task(c, _result)) );

  }

  //! Destroy the Future
  virtual ~Future() throw() { }
  
  /**
   * Wait for the Future's Callable to finish producing a result or
   * an excetion.
   *
   * @see Waitable::wait()
   */
  virtual void wait() 
    /* throw(Synchronization_Exception) */ {

    _result->_cond.wait();

  }

  /**
   * Wait for the Future's Callable to finish producing a result or
   * an excetion. If the timeout expires before the result is ready,
   * return false.
   *
   * @see Waitable::wait(unsigned long)
   */
  virtual bool wait(unsigned long timeout) 
    /* throw(Synchronization_Exception) */ {

    return _result->_cond.wait(timeout);

  }

  /**
   * Get the result from the future, this will block the caller until
   * a result is ready. If the Callable has set an exception, that exception
   * with be propogated to all waiters.
   *
   * @return ResultType
   */
  ResultType getResult() {
    return _result->getResult();
  }
  
};

Future<int>::CallableRef CallablePtr(Future<int>::Callable* c) {
  return Future<int>::CallableRef(c);
}

template <typename T>
Future<T>::CallableRef CallablePtr(Future<T>::Callable* c) {
  return Future<T>::CallableRef(c);
}

} // namespace ZThread

#endif //  __ZTFUTURE_H__
