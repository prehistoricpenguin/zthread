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
AbstractThreadLocal.cxx \
AtomicCount.cxx \
Condition.cxx \
CountingSemaphore.cxx \
FastMutex.cxx \
FastRecursiveMutex.cxx \
Mutex.cxx \
RecursiveMutexImpl.cxx \
RecursiveMutex.cxx \
Monitor.cxx \
PriorityCondition.cxx \
PriorityInheritanceMutex.cxx \
PriorityMutex.cxx \
PrioritySemaphore.cxx \
Semaphore.cxx \
Thread.cxx \
ThreadImpl.cxx \
ThreadLocalMap.cxx \
Time.cxx \
ThreadOps.cxx

