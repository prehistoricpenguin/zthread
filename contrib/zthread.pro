INCLUDEPATH	= include
win32-borland:INCLUDEPATH += $(BCB)/include $(BCB)/include/vcl
DEFINES         =  ZTHREAD_STATIC NDEBUG
win32:DEFINES   += ZT_WIN32 _WIN32 _LIB WIN32  DISABLE_IO_INTERRUPT
win32-msvc:DEFINES += HAVE_WIN32_FTIME
win32-borland:DEFINES += HAVE_FTIME
unix:DEFINES    += _POSIX REENTRANT
unix:TMAKE_CXXFLAGS = -fexceptions
unix:LIBS       = -lpthread
TEMPLATE	= lib
DESTDIR         = lib
win32-msvc:TMAKE_CXXFLAGS +=/EHsc
win32-borland:TMAKE_CXXFLAGS += -P -xp -WM
CONFIG          = warn_off release staticlib
VERSION         = 2.1.2

HEADERS		= \
		  include/zthread/AbstractExecutor.h \
		  include/zthread/AbstractThreadLocal.h \
		  include/zthread/AsyncExecutor.h \
		  include/zthread/Barrier.h \
		  include/zthread/BiasedRWLock.h \
		  include/zthread/BlockingQueue.h \
		  include/zthread/BufferedQueue.h \
		  include/zthread/Cancelable.h \
		  include/zthread/Condition.h \
		  include/zthread/CountingSemaphore.h \
		  include/zthread/Executor.h \
		  include/zthread/FairRWLock.h \
		  include/zthread/FastMutex.h \
		  include/zthread/FastRecursiveMutex.h \
		  include/zthread/FastThreadLocal.h \
		  include/zthread/Guard.h \
		  include/zthread/InheritableThreadLocal.h \
		  include/zthread/IntrusivePtr.h \
		  include/zthread/Latch.h \
		  include/zthread/Lockable.h \
		  include/zthread/LockedQueue.h \
		  include/zthread/MonitoredQueue.h \
		  include/zthread/Mutex.h \
		  include/zthread/NullMutex.h \
		  include/zthread/Observable.h \
		  include/zthread/PoolExecutor.h \
		  include/zthread/Queue.h \
		  include/zthread/RWLock.h \
		  include/zthread/RecursiveMutex.h \
		  include/zthread/Semaphore.h \
		  include/zthread/SharedInstance.h \
		  include/zthread/SimpleQueue.h \
		  include/zthread/Singleton.h \
		  include/zthread/SmartPtr.h \
		  include/zthread/SyncExecutor.h \
		  include/zthread/SynchronizationExceptions.h \
		  include/zthread/SynchronizationPolicy.h \
		  include/zthread/Thread.h \
		  include/zthread/ThreadLocal.h \
		  include/zthread/Timer.h \
		  include/zthread/TimerThread.h \
		  include/zthread/TypedLockable.h \
		  include/zthread/Worker.h \
		  include/zthread/ZThread.h \
		  src/State.h \
		  src/Time.h \
		  src/ConditionImpl.h \
		  src/MutexImpl.h \
		  src/SemaphoreImpl.h \
		  src/State.h \
		  src/ThreadFactory.h \
		  src/ThreadImpl.h \
		  src/ThreadManager.h \
		  src/Time.h \
		  src/TimeOps.h \
		  src/TSSMap.h \
		  src/TSSValue.h 
		  
win32:HEADERS  += src/WIN32_Monitor.h \
		  src/WIN32_FastLock.h \
		  src/WIN32_FastRecursiveLock.h \
		  src/WIN32_Monitor.h \
		  src/WIN32_ThreadOps.h \
		  src/WIN32_TSS.h 


unix:HEADERS	+= \
		  src/POSIX_Fastlock.h \
		  src/POSIC_FastRecurisveLock.h \
		  src/POSIX_Monitor.h \
		  src/POSIX_ThreadOps.h \
		  src/POSIX_TSS.h 
	



SOURCES		= \
		  src/AbstractThreadLocal.cxx \
		  src/Condition.cxx \
		  src/ConditionImpl.cxx \
		  src/CountingSemaphore.cxx \
		  src/FastMutex.cxx \
		  src/Mutex.cxx \
		  src/MutexImpl.cxx \
		  src/RWLock.cxx \
		  src/RecursiveMutex.cxx \
		  src/Semaphore.cxx \
		  src/SemaphoreImpl.cxx \
		  src/SynchronizationExceptions.cxx \
		  src/Thread.cxx \
		  src/ThreadImpl.cxx \
		  src/ThreadManager.cxx \
		  src/Time.cxx \
		  src/Timer.cxx \
		  src/TSSMap.cxx
win32:SOURCES  += \
		  src/WIN32_ThreadOps.cxx \ 
		  src/WIN32_Monitor.cxx


unix:SOURCES   += \
		  src/POSIX_FastRecursiveMutex.cc \
		  src/POSIX_Monitor.cc \
		  src/POSIX_ThreadOps.cc 
		   

INTERFACES	=
TARGET = ZThread
