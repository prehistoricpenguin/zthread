dnl Find doxygen if it is available using the paramaeter path;
dnl otherwise just check the user path. If Doxygen is found the 
dnl executable is assigned to the DOXYGEN variable.

dnl Eric Crahen <zthread@code-foo.com>

dnl AM_WITH_DOXYGEN
AC_DEFUN([AM_WITH_DOXYGEN], 
[

  AC_ARG_WITH(doxygen,[  --with-doxygen=PATH     Path to doxygen (optional)], 
  [ AC_CHECK_PROG(DOXYGEN, doxygen, $withval/doxygen,,$withval) ],
  [ AC_PATH_PROG(DOXYGEN, doxygen,,$PATH) ])

])

NO_DOXYGEN_ERROR=<<EOT
This configuration requires Doxygen, by Dimitri van Heesch.
Visit www.stack.nl/~dimitri/doxygen/ for information about how to
obtain a copy. 

If you already have Doxygen and it was not detected you should try the
--with-doxygen option to specifiy the path to your doxygen executable.
EOT

dnl AM_WITH_DOXYGEN_REQUIRED
AC_DEFUN([AM_WITH_DOXYGEN_REQUIRED], 
[

  AM_WITH_DOXYGEN	
  if test x$DOXYGEN == x; then
    AC_MSG_ERROR(${NO_PTHREAD_ERROR})
  fi

])

