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

#ifndef __ZTTHREAD_H__
#define __ZTTHREAD_H__

#include "zthread/Cancelable.h"
#include "zthread/Handle.h"
#include "zthread/Priority.h"
#include "zthread/NonCopyable.h"

namespace ZThread {

class ThreadImpl;

/**
 * @class Thread
 *
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-07-02T16:56:37-0400>
 * @version 2.2.6
 *
 * A Thread is a special kind of Runnable object that represents 
 * a thread of execution in a program. It allows tasks to be
 * started an run concurrently. 
 *
 * There are two kinds of threads. Non-daemon threads and daemon threads.
 * Non-daemon threads must be joined. Non-daemon threads must be joined,
 * the lifetime of its task must be enclosed by the lifetime of the Thread
 * object.
 *
 * @code
 *
 * {
 *   MyThread t;         // Thread object's lifetime begins
 *   t.run();            // Task's effective lifetime begins
 *   t.join();           // Task's effective lifetime ends 
 * }                     // Thread object's lifetime begins 
 *
 * @endcode
 *
 * Daemon threads cannot be joined, they have the special property of allowing
 * the effective lifetime of its task to exceed the lifetime of the Thread object.
 *
 * @code
 *
 * {
 *   MyThread t;         // Thread object's lifetime begins
 *   t.setDaemon(true);
 *   t.run();            // Task's effective lifetime begins   
 * }                     // Thread object's lifetime begins 
 *
 *                       // Task's effective lifetime ends 
 * 
 * @endcode
 * 
 * <b>Using:</b>
 *
 * There are two way to use threads. The first is to extend the Thread class.
 * This creates a heavy-wieght task; because the task is always associated with
 * the overhead of a specific thread. Afterall, it is a Thread.
 *
 * @code 
 *
 * class MyThread : public Thread {
 * public:
 *
 *  virtual ~MyThread() throw() { }
 *
 *  virtual void run() throw() {
 *
 *    // perform task
 *
 *  }
 *
 * };
 *
 * int main() {
 *
 *   try {
 *
 *     MyThread t;
 *     t.start();
 *
 *   } catch(Synchronization_Exception&) {  
 *     std::cerr << "error starting thread!" << std::endl;
 *   }
 *
 * }
 * 
 * @endcode
 *
 * Such a thread can be run using the start() function.
 *
 * @code
 *
 * MyThread t;             // Create the thread
 * t.start();              // run the task
 * t.join();               // wait for completion
 *
 * @endcode
 *
 * The second way is create extend the Runnable class. This creates a 
 * light-weight task; because it is not associated with any one specific
 * thread. This is an application of the Command pattern.
 *
 * @code 
 * 
 * class MyRunnable : public Runnable {
 * public:
 *
 *   virtual ~MyRunnable() throw() { }
 *
 *  virtual void run() throw() {
 *
 *    // perform task
 *
 *  }
 *
 * };
 * 
 * @endcode
 *
 * When Runnable objects are be submitted to Threads for execution they 
 * are wrapped with a reference counting object (called Handle) to simplify 
 * the task of keeping track of them. These wrappers are created for you 
 * automatically using the RunnablePtr() function.
 * 
 * @code
 *
 * // Submit a series of tasks to different threads
 *
 * Thread t[NUM_THREADS];
 *
 * for(int i=0; i<NUM_THREADS; ++i)
 *   task.run( RunnablePtr(new MyTask()) );
 *
 * @endcode
 *
 * An instance of a Runnable class can also be shared and submitted to several threads;
 * rather than creating a new Runnable object for each thread.
 * 
 * @code
 *
 * // Submit a single task to different threads
 *
 * Thread t[NUM_THREADS];
 *
 * Handle<MyTask> task = RunnablePtr(new MyTask());
 *
 * for(int i=0; i<NUM_THREADS; ++i)
 *   task.run( task );
 *
 * @endcode
 *
 * @see Handle
 * @see CancelableTask
 *
 * <b>Interrupting:</b>
 *
 * Threads are interruptible. Each thread has an <i>interrupted</i> status 
 * associated with it. This is set with the interrupt() method. A thread with
 * an <i>interrupted</i> status will throw an Interrupted_Exception if it 
 * attempts a blocking operation (sleep(), Lockable::acquire(), Waitable::wait(), ...)
 * and its <i>interrupted</i> status will be reset. Calling Thread::interrupted() 
 * will allow the currently executing thread to check and reset its <i>interrupted</i> 
 * status in a single operation.
 *
 * <b>Canceling:</b>
 *
 * Threads are Cancelable objects. A Thread extends the usual cancelation semantics
 * to have the following meaning.
 *
 * <b>Disabling</b> 
 *
 * A cancel()ed thread is placed into both <i>interrupted</i> and <i>canceled</i>
 * status. This allows interrupt sensative code to respond to cancel() naturally.
 * The Thread::canceled() function will report the <i>canceled</i> status of a 
 * thread and clear its <i>interrupted</i> status. However, <i>canceled</i> status
 * can never be removed.
 *
 * <b>Exiting</b>
 *
 * A thread that has been cancel()ed is not forced to exit. It behaves as thread
 * that has been interrupted would. By canceling a thread, it is sent a specific
 * message that a request for it to exit has arrived. The thread may then respond
 * by performing a graceful shutdown, performing whatever work is neccessary to do 
 * so.
 *
 * By implementing the Cancelable interface to work with the interruption mechanism,
 * it is possible to write very elegant code and to create some flexible task oriented
 * frameworks.
 */
class ZTHREAD_API Thread : private NonCopyable, public Cancelable, public Runnable { 
 public:

  /**
   * @class Reference
   *
   * The Thread::Reference class is a class that lives on 
   * the local stack. It's used to manipulate the current thread 
   * without affecting the reference count. 
   */
  class Reference {

    friend class Thread;

    //! Delegate
    ThreadImpl* _impl;

    // Restrict heap allocation
    static void * operator new(size_t size);
    static void * operator new[](size_t size);
    
    Reference();
    Reference& operator=(const Reference&);

    Reference(ThreadImpl* impl) : _impl(impl) { }
    
  public:

    Reference(const Reference& r) : _impl(r._impl) { }

    void interrupt() throw();

    void setPriority(Priority p) throw();

    Priority getPriority() throw();
  
  }; /* Reference */

 private:
  
  //! Used to provide a hook for other libraries
  //static ThreadLocal<void*> _interruptKey;
  
  //! Delegate
  ThreadImpl* _impl;
  
  public:

  /**
   * Create a new thread object that can execute tasks in another thread
   * of execution
   *
   * @exception Initialization_Exception - thrown if there are not 
   * enough resources to do this
   */
  Thread() /* throw(Synchronization_Exception) */;
  
  //! Destroy a Thread
  virtual ~Thread() throw();
  
  //! Comparison operator
  inline bool operator==(const Thread& t) const {
    return _impl == t._impl;
  }
  
  //! Comparison operator
  inline bool operator!=(const Thread& t) const {
    return !(*this == t);
  }
    
  //! Comparison operator
  inline bool operator==(const Reference& r) const {
    return _impl == r._impl;
  }
  
  //! Comparison operator
  inline bool operator!=(const Reference& r) const {
    return !(*this == r);
  }
  
 
  /**
   * Wait for the thread represented by this object to exit.
   * Only one thread can wait on any other thread.
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block.
   * A timeout of 0 will block indefinently.
   *
   * @exception Deadlock_Exception thrown if thread attempts to join itself
   * @exception InvalidOp_Exception thrown if the thread cannot be joined
   * @exception Interrupted_Exception thrown if the joining thread has been interrupt()ed
   */
  bool join(unsigned long timeout = 0) 
    /* throw(Synchronization_Exception) */;

  /**
   * This can be implemented by subclasses to create a thread with 
   * a built in task. The default behavior is for the task to just
   * return immediately, doing nothing.
   */
  virtual void run() throw(); 
  
  /**
   * From the context of this Thread, execute the task defined by the given
   * Runnable object.
   *
   * @exception InvalidOp_Exception thrown if a thread has already
   * been assigned a task and is running; or if this thread has already 
   * been used to run a task.
   * @exception Synchronization_Exception thrown if there is some other error 
   * starting the thread
   */
  void run(const RunnableHandle& task)
    /* throw(Synchronization_Exception) */;

  /**
   * Convience method
   *
   * @see Thread::run(const RunnableHandle&)
   */
  void run(Runnable* task)
    /* throw(Synchronization_Exception) */ {

    run( RunnablePtr(task) );

  }


  /**
   * From the context of this Thread, this method will run the Threads run() method.
   * The default implementation for the run() method will throw an exception
   * and exit immediately. However, subclasses can provide thier own, more useful,
   * implementation of that method.
   *
   * @exception Synchronization_Exception thrown if there is an error starting the thread
   */
  void start() 
    /* throw(Synchronization_Exception) */;

  /**
   * Change the priority of this Thread. This will change the actual
   * priority of the thread when the OS supports it. If there is no
   * real priority support, it's simulated.
   *
   * @param p - new Priority
   */
  void setPriority(Priority p) throw();

  /**
   * Get the priority of this Thread. 
   * 
   * @return Priority
   */
  Priority getPriority() throw();

  /** 
   * Interrupts this thread.
   *
   * If this thread is blocked when this method is called, the thread will
   * abort that blocking operation with an Interrupted_Exception.
   *
   * Otherwise, the <i>interrupted</i> status of the thread is set. This status
   * is cleared by one of two methods. The first is by attempting another 
   * blocking operation; this will clear the <i>interrupted</i> status and 
   * immediately abort the operation with an Interrupted_Exception. The second
   * is to call isInterrupted() from the context of this thread.
   *
   * A thread is never started in an interrupted state. The interrupted status
   * of a thread will be discarded when the thread starts.
   *
   * Interrupting a thread that is no longer running will have no effect other 
   * than setting the interrupt status permanently. When a thread exits, that
   * status can no longer be cleared.
   */
  void interrupt() throw();
    
  /**
   * Tests whether the current Thread has been interrupt()ed, clearing
   * its interruption status.
   *
   * @return bool true if the Thread was interrupted, otherwise false
   */
  static bool interrupted() throw();

  /**
   * Tests whether the current Thread has been canceled, and clears the 
   * interrupted status.
   *
   * @return bool true only if the Thread::cancel() has been invoked.
   */
  static bool canceled() throw();

  /**
   * Tests whether this thread has been canceled. If called from the context 
   * of this thread, the interrupted status is cleared.
   *
   * @return bool true if the Thread was canceled, otherwise false
   *
   * @post There is no serlization garuntee with this method, Its possible
   * for a thread to be canceled immediately after this functions returns.
   */
  virtual bool isCanceled() 
    /* throw(Synchronization_Exception) */;

  /**
   * Set the cancelation and interruption status of this thread.
   *
   * @exception InvalidOp_Exception thrown if a thread attempts to cancel itself
   */
  virtual void cancel()
    /* throw(Synchronization_Exception) */;

  /**
   * Tests if this thread is a daemon thread.
   *
   * @return bool - true if this thread is a daemon thread.
   */
  bool isDaemon() throw();

  /**
   * Change the status of this thread so that is now treated as a daemon
   * thread. A daemon thread should not be joined, it will execute normally
   * and its destructor can safely be invoked without first join()ing it.
   * It will be join()ed by ZThreads when the program exits.
   *
   * @param bool - flag, set to daemon if true, otherwise set back to a
   * normal user thread that can be joined.
   *
   * @exception InvalidOp_Exception thrown if the status cannot be changed.
   */
  void setDaemon(bool flag) /* throw(Synchronization_Exception) */;

  /** 
   * Put the currently executing thread to sleep for a given amount of
   * time. 
   *
   * @param timeout - maximum amount of time (milliseconds) this method could block
   *
   * @exception Interrupted_Exception thrown if this wait was interrupt()ed
   */
  static void sleep(unsigned long timeout)
     /* throw(Synchronization_Exception) */;
  
  /**
   * Cause the currently executing thread to yield, allowing the scheduler
   * to assign some execution time to another thread.
   */
  static void yield() throw();

  /** 
   * Get a reference to the currently executing thread.
   *
   * @return Reference - object representing the current thread.
   */
  static Reference current() throw();

  /**
   * Get a reference to a default ThreadLocal key. This key will always
   * contain some information that is specific to a particular platform.
   *
   * This is used to develop software that wants to react to
   * a Thread::interrupt() invocation.
   *
   * WIN32 details:
   *
   * The Win32 system provides a rich set of function that work with event
   * handles. Because there are so many ways to use these handles creatively
   * to handle various situation, the interruptKey() on a Win32 system will
   * contain the value of a HANDLE. This handle can be used with functions
   * like WaitForMutlipleObjects() or WSAEventSelect(), it will be 
   * notified with SetEvent() as a thread was interrupted.
   *
   * POSIX details:
   *
   * SIG_ALRM will be generated and should interrupt any i/o that is 
   * in progress. Most POSIX functions will return EINTR in such a 
   * situation. Example:
   *
   * @code
   *
   * char buf[64];
   * size_t sz = 64;
   * 
   * do {
   * 
   *   ssize_t n = read(fd, buf, sz);
   *
   *   if(n < 0 && errno == EINTR && Thread::current()->isInterrupted()) {
   *     // Take some action
   *     throw SomeKindOfInterruptedException();
   *   }
   *
   * } while(n < 0 && errno == EINTR);
   * 
   * @endcode
   * 
   * To write some piece of code that can be notified of a Thread::interrupt()
   * if it is performing some action that does not return EINTR another
   * method is provided. The interruptKey() will contain the address of a
   * sigjmp_buf. If the user sets this value to something other than null,
   * the Thread::interrupt() will use a siglongjmp after invoking the signal
   * The threads interrupt() method will also clear this address after it has
   * been used so that double notifications do not occur.
   * Example:
   *
   * @code
   *
   *  // Implement for a function that ignores EINTR
   *  sigjmp_buf timeout_jmp;
   *  if(sigsetjmp(timeout_jmp, 1) != 0) {
   *    // Take some action
   *    throw SomeKindOfInterruptedException();
   *  }
   *
   *  // Enable the timeout
   *  ZThread::Thread::interruptKey().set(&timeout_jmp);
   *
   *  // Do some lengthy operation.
   *
   *  // Disable the timeout
   *  ZThread::Thread::interruptKey().set(NULL);
   *
   * @endcode
   *
   * @return const FastThreadLocal&
  inline static const ThreadLocal<void*>& interruptKey() throw() {
    return _interruptKey;
  }
   */

}; /* Thread */
  
} // naemspace ZThread

#endif // __ZTTHREAD_H__



