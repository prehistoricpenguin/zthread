AM_CXXFLAGS = @COMPILER_OPTIONS@
INCLUDES = -I$(top_srcdir)/include
LDADD = @LINKER_OPTIONS@ $(top_builddir)/src/.libs/libZThread.a
SUBDIRS=.

MutexTests_SOURCES = MutexTests.cxx
DaemonThreadTests_SOURCES = DaemonThreadTests.cxx
ThreadTests_SOURCES = ThreadTests.cxx
StatusTests_SOURCES = StatusTests.cxx
GuardTests_SOURCES = GuardTests.cxx
TimeTests_SOURCES = TimeTests.cxx
PriorityTests_SOURCES = PriorityTests.cxx
ExecutorTests_SOURCES = ExecutorTests.cxx
SemaphoreTests_SOURCES = SemaphoreTests.cxx
FutureTests_SOURCES = FutureTests.cxx
ReadWriteTests_SOURCES = ReadWriteTests.cxx

noinst_PROGRAMS = \
MutexTests \
DaemonThreadTests \
ThreadTests \
StatusTests \
GuardTests \
PriorityTests \
TimeTests \
ExecutorTests \
SemaphoreTests \
FutureTests \
ReadWriteTests

EXTRA_DIST = \
DAEMONTHREAD_TESTS.dsp \
THREAD_TESTS.dsp \
THREADLOCAL_TESTS.dsp \
MUTEX_TESTS.dsp \
STATUS_TESTS.dsp
