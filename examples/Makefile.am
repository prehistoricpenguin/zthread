CXXFLAGS = @COMPILER_OPTIONS@
INCLUDES = -I$(top_srcdir)/include
LDADD = @LINKER_OPTIONS@ $(top_builddir)/src/.libs/libZThread.a 
SUBDIRS=.

EXTRA_DIST=\
SEMAPHORE_EXAMPLE.dsp    THREAD_EXAMPLE.dsp \
CONDITION_EXAMPLE.dsp  SERVICE_EXAMPLE.dsp      TIMEOUT_EXAMPLE.dsp \
EXECUTOR_EXAMPLE.dsp   SINGLETON_EXAMPLE.dsp   \
MUTEX_EXAMPLE.dsp \
THREADLOCAL_EXAMPLE.dsp 

noinst_PROGRAMS = \
interruption1 \
interruption2 \
interruption3 \
executor1 \
ConditionExample \
MutexExample \
SingletonExample \
SemaphoreExample \
ServiceExample \
ThreadExample \
ThreadLocalExample \
ExecutorExample 

interruption1_SOURCES = interruption1.cxx
interruption2_SOURCES = interruption2.cxx
interruption3_SOURCES = interruption3.cxx
executor1_SOURCES     = executor1.cxx

ExecutorExample_SOURCES = ExecutorExample.cxx 
ConditionExample_SOURCES = ConditionExample.cxx
MutexExample_SOURCES = MutexExample.cxx
SingletonExample_SOURCES = SingletonExample.cxx
SemaphoreExample_SOURCES = SemaphoreExample.cxx
ServiceExample_SOURCES = ServiceExample.cxx
ThreadExample_SOURCES = ThreadExample.cxx
ThreadLocalExample_SOURCES = ThreadLocalExample.cxx
