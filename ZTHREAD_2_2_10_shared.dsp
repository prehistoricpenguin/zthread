# Microsoft Developer Studio Project File - Name="ZTHREAD_2_2_10_shared" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ZTHREAD_2_2_10_shared - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZTHREAD_2_2_10_shared.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZTHREAD_2_2_10_shared.mak" CFG="ZTHREAD_2_2_10_shared - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZTHREAD_2_2_10_shared - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ZTHREAD_2_2_10_shared - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZTHREAD_2_2_10_shared - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_shared"
# PROP Intermediate_Dir "Release_shared"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZTHREAD_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZTHREAD_EXPORTS" /D "ZT_WIN32" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib /nologo /dll /debug /machine:I386 /out:"Release_shared/ZThread.dll" /implib:"ZThread-2.2.10.lib" /opt:ref,icf
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ZTHREAD_2_2_10_shared - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ZTHREAD_2_2_10_shared___Win32_Debug"
# PROP BASE Intermediate_Dir "ZTHREAD_2_2_10_shared___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_shared"
# PROP Intermediate_Dir "Debug_shared"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZTHREAD_2_2_10_shared_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /I "include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZTHREAD_EXPORTS" /D "ZT_WIN32" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib /nologo /dll /pdb:none /debug /machine:I386 /nodefaultlib:"cd" /out:"Debug_shared/ZThread.dll" /implib:ZThread-2.2.10.lib"

!ENDIF 

# Begin Target

# Name "ZTHREAD_2_2_10_shared - Win32 Release"
# Name "ZTHREAD_2_2_10_shared - Win32 Debug"
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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
