AM_CXXFLAGS = @COMPILER_OPTIONS@ @EXTRA_COMPILER_OPTIONS@
INCLUDES = -I$(top_srcdir)/include
SUBDIRS=.

libdir=$(prefix)/lib

lib_LTLIBRARIES = libZThread.la

libZThread_la_LIBADD=@LINKER_OPTIONS@ @EXTRA_LINKER_OPTIONS@
libZThread_la_LDFLAGS= \
        -release $(LT_RELEASE)  \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)


LIBADD=@LINKER_OPTIONS@ @EXTRA_LINKER_OPTIONS@


libZThread_la_SOURCES = \
AtomicCount.cxx \
Condition.cxx \
ConcurrentExecutor.cxx \
CountingSemaphore.cxx \
FastMutex.cxx \
FastRecursiveMutex.cxx \
Mutex.cxx \
RecursiveMutexImpl.cxx \
RecursiveMutex.cxx \
Monitor.cxx \
PoolExecutor.cxx \
PriorityCondition.cxx \
PriorityInheritanceMutex.cxx \
PriorityMutex.cxx \
PrioritySemaphore.cxx \
Semaphore.cxx \
SynchronousExecutor.cxx \
Thread.cxx \
ThreadedExecutor.cxx \
ThreadImpl.cxx \
ThreadLocalImpl.cxx \
ThreadQueue.cxx \
Time.cxx \
ThreadOps.cxx

