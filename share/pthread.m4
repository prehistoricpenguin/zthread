# Select a pthread library & setup some build variables
# by Eric Crahen <crahen@cs.buffalo.edu>

NO_PTHREAD_ERROR="
Please install/update your POSIX threads (pthreads) library. Updates
should be available from either your system vendor, or, for GNU/Linux
systems, go to http://pauillac.inria.fr/~xleroy/linuxthreads/.
GNU Pth can also be used if it was configured with --enable-pthread.
"

ifdef(AM_WITH_PTHREAD,,[

AC_DEFUN(AM_WITH_PTHREAD,
[

AC_ARG_WITH(pthread-prefix,
[  --with-pthread-prefix   POSIX threads library prefix (optional)],
pthread_prefix="$withval", pthread_prefix="")

ac_orig_LIBS="$LIBS" 
ac_orig_CFLAGS="$CFLAGS"

pthread_lib_prefix=""
pthread_inc_prefix=""

if test x$pthread_prefix != x ; then

  pthread_lib_prefix="$pthread_prefix/lib"
  LIBS="-L$pthread_lib_prefix"

  pthread_inc_prefix="$pthread_prefix/include"
  CFLAGS="-I$pthread_inc_prefix"

fi

CFLAGS="$CFLAGS -DREENTRANT" 

ac_save_LIBS="$LIBS"
ac_save_CFLAGS="$CFLAGS"

AC_MSG_CHECKING(for pthread_create in -lpthread)

LIBS="$LIBS -lpthread"

AC_TRY_LINK(
[#include <pthread.h>],
[pthread_create((pthread_t*) 0,(pthread_attr_t*) 0, 0, 0);],
[ AC_MSG_RESULT(yes)] ,
[ AC_MSG_RESULT(no)

  AC_MSG_CHECKING(for pthread_create in -pthread)
  LIBS="$ac_save_LIBS -pthread"

  AC_TRY_LINK(
  [#include <pthread.h>],
  [pthread_create((pthread_t*) 0,(pthread_attr_t*) 0, 0, 0);],
  AC_MSG_RESULT(yes),
  [ AC_MSG_RESULT(no)
    AC_MSG_ERROR(${NO_PTHREAD_ERROR})
  ])

])

AC_MSG_CHECKING(for compiler with -mt flag)
ac_save_CFLAGS="$CFLAGS"
CFLAGS="-mt $CFLAGS"
AC_TRY_LINK(, return 0;, AC_MSG_RESULT(yes), 
[ AC_MSG_RESULT(no) 
  CFLAGS="$ac_save_CFLAGS"
])

PTHREAD_LIBS="$LIBS"
PTHREAD_CFLAGS="$CFLAGS"

AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_LIBS)

LIBS="$ac_orig_LIBS"
CFLAGS="$ac_orig_CFLAGS"

])
])

dnl
dnl Perform some informational checks. As well as locating pthreads.
dnl Checks what actually enabled on your library, it can vary on
dnl some platforms
dnl
dnl PTHREAD_LIBS defined
dnl PTHREAD_CFLAGS defined
dnl
dnl Defines:
dnl
dnl HAVE_SCHED_RT
dnl HAVE_SCHED_YIELD
dnl
dnl HAVE_PTHREAD_SIGMASK
dnl
dnl HAVE_PTHREADKEY_CREATE
dnl HAVE_PTHREADKEYCREATE
dnl
dnl HAVE_MUTEXATTR_SETKIND_NP
dnl HAVE_MUTEXATTR_SETTYPE
dnl
dnl HAVE_CONDATTR_SETKIND_NP
dnl HAVE_CONDATTR_SETTYPE
dnl
AC_DEFUN(AM_WITH_PTHREAD_INFO,
[

ac_orig_LIBS="$LIBS" 
ac_orig_CFLAGS="$CFLAGS"

AM_WITH_PTHREAD

LIBS="$PTHREAD_LIBS"
CFLAGS="$PTHREAD_CFLAGS"


dnl Check for SunOS thread library style 	
AC_CHECK_LIB(rt, sched_get_priority_max, 
[
 AC_DEFINE(HAVE_SCHED_RT,,[Defined if -lrt is needed for RT scheduling])
 LIBS="$LIBS -lrt"
]
)

PTHREAD_LIBS="$LIBS"

dnl Check for pthread_yield
AC_MSG_CHECKING(for pthread_yield);
AC_TRY_LINK([#include <pthread.h>],
[ pthread_yield(); ], 
[ AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_PTHREAD_YIELD,,[Defined if pthread_yield() is available]) 
], AC_MSG_RESULT(no))

dnl Check for sched_yield
AC_MSG_CHECKING(for sched_yield);
AC_TRY_LINK([#include <sched.h>],
[ sched_yield(); ], 
[ AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_SCHED_YIELD,,[Defined if sched_yield() is available]) 
], AC_MSG_RESULT(no))


dnl Check for pthread_sigmask
AC_MSG_CHECKING(for pthread_sigmask);
AC_TRY_LINK([#include <pthread.h>],
[ pthread_sigmask(0,0,0); ], 
[ AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_PTHREAD_SIGMASK,,[Defined if pthread_sigmask() is available]) 
], AC_MSG_RESULT(no))


dnl Check for pthread_key_create
AC_MSG_CHECKING(for pthread_key_create)
AC_TRY_LINK([#include <pthread.h>],
[ pthread_key_create(0, 0);],
[ AC_MSG_RESULT(yes) 
  AC_DEFINE(HAVE_PTHREADKEY_CREATE,,[Defined if pthread_key_create() is available]) ],
[ AC_MSG_RESULT(no)

  AC_MSG_CHECKING(for pthread_keycreate)

  AC_TRY_LINK([#include <pthread.h>],
  [ pthread_keycreate(0,0); ], 
  [ AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_PTHREADKEYCREATE,,[Defined if pthread_keycreate() is available]) 
  ], AC_MSG_RESULT(no))

])


dnl Check for mutex attributes
AC_MSG_CHECKING(for pthread_mutexattr_setkind_np);
pthread_xtra_defs=""
AC_TRY_LINK([#include <pthread.h>],
[ pthread_mutexattr_setkind_np(0, 0);],
[ AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_MUTEXATTR_SETKIND_NP,,[Defined if pthread_setkind_np() is available]) ],
[ dnl now try pthread_mutexattr_settype

  AC_MSG_RESULT(no)
  AC_MSG_CHECKING(for pthread_mutexattr_settype);

  ac_save_CFLAGS="$CFLAGS"

  if test x$pthread_xtra_defs = x ; then    
    CFLAGS="$CFLAGS -D__USE_UNIX98 -D_XOPEN_SOURCE=500"
    pthread_xtra_defs="yes"
  fi

  AC_TRY_LINK([#include <pthread.h>],
  [ pthread_mutexattr_settype(0, 0); ],
  [ AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_MUTEXATTR_SETTYPE,,[Defined if pthread_mutexattr_settype() is available])
  ],[ 
    AC_MSG_RESULT(no) 
     pthread_xtra_defs=""
    CFLAGS="$ac_save_CFLAGS"  
  ])

])

dnl Check for cond attributes
AC_MSG_CHECKING(for pthread_condattr_setkind_np);
pthread_xtra_defs=""
AC_TRY_LINK([#include <pthread.h>],
[ pthread_condattr_setkind_np(0, 0);],
[ AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_CONDATTR_SETKIND_NP,,[Defined if pthread_condattr_setkind_np() is available]) ],
[ dnl now try pthread_condattr_settype

  AC_MSG_RESULT(no)
  AC_MSG_CHECKING(for pthread_condattr_settype);

  ac_save_CFLAGS="$CFLAGS"

  if test x$pthread_xtra_defs = x ; then    
    CFLAGS="$CFLAGS -D__USE_UNIX98 -D_XOPEN_SOURCE=500"
    pthread_xtra_defs="yes"
  fi

  AC_TRY_LINK([#include <pthread.h>],
  [ pthread_condattr_settype(0, 0); ],
  [ AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_CONDATTR_SETTYPE,,[Defined if pthread_condattr_settype() is available])
  ],[ 
    AC_MSG_RESULT(no) 
     pthread_xtra_defs=""
    CFLAGS="$ac_save_CFLAGS"  
  ])

])


PTHREAD_LIBS="$LIBS"
PTHREAD_CFLAGS="$CFLAGS"

AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_LIBS)

LIBS="$ac_orig_LIBS"
CFLAGS="$ac_orig_CFLAGS"

])

