# Microsoft Developer Studio Project File - Name="ZTHREAD_2_2_8_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ZTHREAD_2_2_8_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZTHREAD_2_2_8_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZTHREAD_2_2_8_static.mak" CFG="ZTHREAD_2_2_8_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZTHREAD_2_2_8_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ZTHREAD_2_2_8_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZTHREAD_2_2_8_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_static"
# PROP Intermediate_Dir "Release_static"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ZTHREAD_STATIC" /D "ZT_WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release_static/ZThread.lib"

!ELSEIF  "$(CFG)" == "ZTHREAD_2_2_8_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_static"
# PROP Intermediate_Dir "Debug_static"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ZTHREAD_STATIC" /D "ZT_WIN32" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug_static/ZThread.lib"

!ENDIF 

# Begin Target

# Name "ZTHREAD_2_2_8_static - Win32 Release"
# Name "ZTHREAD_2_2_8_static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\AbstractThreadLocal.cxx
# End Source File
# Begin Source File

SOURCE=.\src\AtomicCount.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Condition.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ConditionImpl.h
# End Source File
# Begin Source File

SOURCE=.\src\config.h
# End Source File
# Begin Source File

SOURCE=.\src\CountingSemaphore.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Debug.h
# End Source File
# Begin Source File

SOURCE=.\src\DeferredInterruptionScope.h
# End Source File
# Begin Source File

SOURCE=.\src\FastLock.h
# End Source File
# Begin Source File

SOURCE=.\src\FastMutex.cxx
# End Source File
# Begin Source File

SOURCE=.\src\FastRecursiveLock.h
# End Source File
# Begin Source File

SOURCE=.\src\FastRecursiveMutex.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Holder.h
# End Source File
# Begin Source File

SOURCE=.\src\Monitor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Monitor.h
# End Source File
# Begin Source File

SOURCE=.\src\Mutex.cxx
# End Source File
# Begin Source File

SOURCE=.\src\MutexImpl.h
# End Source File
# Begin Source File

SOURCE=.\src\PriorityCondition.cxx
# End Source File
# Begin Source File

SOURCE=.\src\PriorityInheritanceMutex.cxx
# End Source File
# Begin Source File

SOURCE=.\src\PriorityMutex.cxx
# End Source File
# Begin Source File

SOURCE=.\src\PrioritySemaphore.cxx
# End Source File
# Begin Source File

SOURCE=.\src\RecursiveMutex.cxx
# End Source File
# Begin Source File

SOURCE=.\src\RecursiveMutexImpl.cxx
# End Source File
# Begin Source File

SOURCE=.\src\RecursiveMutexImpl.h
# End Source File
# Begin Source File

SOURCE=.\src\RWLock.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Scheduling.h
# End Source File
# Begin Source File

SOURCE=.\src\Semaphore.cxx
# End Source File
# Begin Source File

SOURCE=.\src\SemaphoreImpl.h
# End Source File
# Begin Source File

SOURCE=.\src\State.h
# End Source File
# Begin Source File

SOURCE=.\src\Status.h
# End Source File
# Begin Source File

SOURCE=.\src\Thread.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ThreadFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\ThreadImpl.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ThreadImpl.h
# End Source File
# Begin Source File

SOURCE=.\src\ThreadLocalMap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ThreadLocalMap.h
# End Source File
# Begin Source File

SOURCE=.\src\ThreadOps.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ThreadOps.h
# End Source File
# Begin Source File

SOURCE=.\src\ThreadQueue.h
# End Source File
# Begin Source File

SOURCE=.\src\Time.cxx
# End Source File
# Begin Source File

SOURCE=.\src\TimeStrategy.h
# End Source File
# Begin Source File

SOURCE=.\src\TSS.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\zthread\AbstractExecutor.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\AbstractThreadLocal.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\AsyncExecutor.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Barrier.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\BiasedRWLock.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\BlockingQueue.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\BufferedQueue.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Cancelable.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\CheckedMutex.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Condition.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\CountingSemaphore.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Executor.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Factory.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\FairRWLock.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\FastMutex.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\FastRecursiveMutex.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\FastThreadLocal.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Guard.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\InheritableThreadLocal.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\IntrusivePtr.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Latch.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Lockable.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\LockedQueue.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\MonitoredQueue.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Mutex.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\NullMutex.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Observable.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\PoolExecutor.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Queue.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\RecursiveMutex.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\RWLock.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Semaphore.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\SharedInstance.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\SimpleQueue.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\SmartPtr.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\SyncExecutor.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\SynchronizationExceptions.h
# End Source File
# Begin Source File

SOURCE=.\src\Synchronized.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Thread.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\ThreadLocal.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Timer.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\TimerThread.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\Worker.h
# End Source File
# Begin Source File

SOURCE=.\include\zthread\ZThread.h
# End Source File
# End Group
# End Target
# End Project
