dnl
dnl Enables AM_DETECT_PTHREAD to test for 
dnl pthreads support
dnl
dnl --with-pthread
dnl
dnl If support is available, then HAVE_POSIX_THREADS 
dnl will be set
dnl
ifdef(AM_DETECT_PTHREAD,,[

NO_PTHREAD_ERROR=<<"EOF"
Please install/update your POSIX threads (pthreads) library. Updates
should be available from either your system vendor, or, for GNU/Linux
systems, go to http://pauillac.inria.fr/~xleroy/linuxthreads/.
GNU Pth can also be used if it was configured with --enable-pthread.
EOF

AC_DEFUN(AM_DETECT_PTHREAD,
[

pthread_explicit="no"

 AC_ARG_ENABLE(pthread,
        AC_HELP_STRING([--enable-pthread],
                       [use pthreads [default=detect]]),
 [pthread_explicit="yes"], 
 [enable_pthread="yes"])


 AC_ARG_WITH(pthread-prefix,
  [  --with-pthread-prefix   POSIX threads library prefix (optional)],
  [

   if test x$pthread_prefix != x ; then

     PTHREAD_LIBS="-L$withval/lib"
     PTHREAD_CXXFLAGS="-I$withval/include"

   fi

   pthread_explicit="yes"
   enable_pthread="yes"

  ], 
  [PTHREAD_LIBS=""
   PTHREAD_CXXFLAGS=""
  ])

  PTHREAD_CXXFLAGS="-DREENTRANT $PTHREAD_CXXFLAGS"

  if test x$enable_pthread != xno; then

   ac_save_LIBS="$LIBS" 
   ac_save_CXXFLAGS="$CXXFLAGS"
 
   AC_CHECK_HEADER([pthread.h],
   [AC_MSG_CHECKING([for linker option -pthread])

    LIBS="ac_save_LIBS $PTHREAD_LIBS -pthread"
    CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"

    AC_TRY_LINK([#include <pthread.h>],
    [
      pthread_create((pthread_t*) 0,(pthread_attr_t*) 0, 0, 0);
    ],
    [AC_MSG_RESULT(yes)
     AC_DEFINE(HAVE_POSIX_THREADS,,[defined when pthreads is available])
     PTHREAD_LIBS="$PTHREAD_LIBS -pthread" 

    ],
    [AC_MSG_RESULT(no)
     AC_MSG_CHECKING(for linker option -lpthread)

     LIBS="$ac_save_LIBS $PTHREAD_LIBS -lpthread"

     AC_TRY_LINK([#include <pthread.h>],
     [
       pthread_create((pthread_t*) 0,(pthread_attr_t*) 0, 0, 0);
     ], 
     [AC_MSG_RESULT(yes)
      AC_DEFINE(HAVE_POSIX_THREADS,,[defined when pthreads is available])
      PTHREAD_LIBS="$PTHREAD_LIBS -lpthread"
     ],
     [AC_MSG_RESULT(no)
    
     if test $pthread_explicit = "yes"; then
       AC_MSG_ERROR(${NO_PTHREAD_ERROR})
     fi

    ])
   ])
  ])

  AC_SUBST(PTHREAD_LIBS)
  AC_SUBST(PTHREAD_CXXFLAGS)

  CXXFLAGS="$ac_save_CXXFLAGS"
  LIBS="$ac_save_LIBS"

 fi

])
])


dnl Eric Crahen <crahen at code-foo dot com>
