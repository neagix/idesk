dnl Initialize autoconf and automake
AC_INIT(src/Main.cpp)
VERSION=0.7.8
PACKAGE=idesk
dnl Output files
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE($PACKAGE,"$VERSION")

dnl Determine default prefix
if test "x$prefix" = "xNONE"; then
  prefix="/usr/local"
fi
if test "x$exec_prefix" = "xNONE"; then
 exec_prefix=$prefix
fi

dnl Check for various flavors of UNIX(r)
dnl AC_AIX
AC_ISC_POSIX
AC_LANG_CPLUSPLUS

dnl Locate required external software
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl Check for system header files
AC_CHECK_HEADERS(string.h unistd.h dirent.h stdlib.h sys/time.h time.h sys/wait.h)

AC_CHECK_HEADERS(sstream, ,
        [ AC_CHECK_HEADERS(strstream,,
            [AC_MSG_ERROR([Your libstdc++ doesn't have the sstream, fstream or strstream classes])]
        )]
)


AC_CHECK_HEADERS(cassert cctype cerrno cmath cstdarg cstdio cstdlib cstring ctime)

dnl Check for X headers and libraries
AC_PATH_X([X11], [X11/Xlib.h], [XOpenDisplay(NULL)])
AC_PATH_XTRA

test x$no_x = "xyes" && AC_MSG_ERROR([Idesk requires the X Window System libraries and headers.])

test x$x_includes = "x" && x_includes="/usr/include"
test x$x_libraries = "x" && x_libraries="/usr/lib"

CFLAGS="$CFLAGS $X_CFLAGS"
CXXFLAGS="$CXXFLAGS $X_CFLAGS"
LIBS="$LIBS $X_LIBS"
LDFLAGS="$LDFLAGS $LIBS $X_PRE_LIBS"

dnl Check for required functions in -lX11
AC_CHECK_LIB(X11, XOpenDisplay, LIBS="$LIBS -lX11",[
  echo "ERROR:  You need libX11 to build Idesk.  Verify that you have libX11.a or";
  echo "        libX11.so installed and that it is located in the X libraries";
  AC_MSG_ERROR([Fatal:  libX11 not found.])])

LIBS="$LIBS $X_EXTRA_LIBS"

dnl Check for XShape extension support and proper library files.
SHAPE=""
AC_MSG_CHECKING([whether to build support for the XShape extension])
AC_ARG_ENABLE(
  shape, [  --enable-shape          enable support of the XShape extension [default=yes]])

: ${enableval="yes"}
if test x$enableval = "xyes"; then
  AC_MSG_RESULT([yes])
  AC_CHECK_LIB(Xext, XShapeCombineShape,
    AC_MSG_CHECKING([for X11/extensions/shape.h])
    AC_TRY_LINK(
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
, long foo = ShapeSet,
      AC_MSG_RESULT([yes])
      SHAPE="-DSHAPE"; Xext_lib="-lXext",
      AC_MSG_RESULT([no])
    )
  )
else
  AC_MSG_RESULT([no])
fi
AC_SUBST(SHAPE)
CXXFLAGS="$CXXFLAGS $SHAPE"
LIBS="$LIBS $Xext_lib"

dnl Imlib2 detection

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([IMLIB], [imlib2 > 1.0])

CXXFLAGS="$CXXFLAGS $IMLIB_CFLAGS"
LIBS="$LIBS $IMLIB_LIBS"


dnl Check for libsn Startup Notification options
STARTUP=""
AC_MSG_CHECKING([whether to include Startup Notification ])
AC_ARG_ENABLE(
  libsn, [  --enable-libsn        include code for LIBSN [default=no]])

if test -n "$enable_libsn"; then
                    PKG_CHECK_MODULES(LIBSN, libstartup-notification-1.0 >= 0.5)
                    AC_MSG_RESULT([yes])
                    STARTUP="-DHAVE_STARTUP_NOTIFICATION -DSN_API_NOT_YET_FROZEN=1",
else
  AC_MSG_RESULT([no])
fi

AM_CONDITIONAL(HAVE_STARTUP_NOTIFICATION, test x$LIBSN = xtrue)
AC_SUBST(STARTUP)
AC_SUBST(LIBSN_CFLAGS)
AC_SUBST(LIBSN_LIBS)
CXXFLAGS="$CXXFLAGS $LIBSN_CFLAGS $STARTUP"
LIBS="$LIBS $LIBSN_LIBS"

dnl Check for librsvg
AC_MSG_CHECKING([whether to include SVG support via librsvg2 and gdk-pixbuf-2.0 ])
AC_ARG_ENABLE(
  svg, [  --enable-svg           include support for SVG [default=no]])

if test -n "$enable_svg"; then
  AC_MSG_RESULT([yes])
  AC_CHECK_LIB(rsvg-2, rsvg_init, ,[
    echo "ERROR:  You need librsvg-2.0 to build Idesk with SVG support.  Verify that you have librsvg-2.so.2";
    AC_MSG_ERROR([Fatal:  librsvg-2.0 not found.])])

  LIBRSVG_LIBS=`pkg-config --libs librsvg-2.0`
  LIBRSVG_CXXFLAGS=`pkg-config --cflags librsvg-2.0`
  AC_SUBST(LIBRSVG_LIBS)
  AC_SUBST(LIBRSVG_CXXFLAGS)
  CXXFLAGS="$CXXFLAGS $LIBRSVG_CXXFLAGS"
  LIBS="$LIBS $LIBRSVG_LIBS"

  dnl Check for libgdk-pixbuf
  AC_CHECK_LIB(gdk_pixbuf-2.0, gdk_pixbuf_get_pixels, ,[
    echo "ERROR:  You need libgdk-pixbuf-2.0 to build Idesk.";
    AC_MSG_ERROR([Fatal:  libgdk-pixbuf-2.0 not found.])])

  LIBGDK_PIXBUF_LIBS=`pkg-config --libs gdk-pixbuf-2.0`
  LIBGDK_PIXBUF_CXXFLAGS=`pkg-config --cflags gdk-pixbuf-2.0`
  AC_SUBST(LIBGDK_PIXBUF_LIBS)
  AC_SUBST(LIBGDK_PIXBUF_CXXFLAGS)
  CXXFLAGS="$CXXFLAGS $LIBGDK_PIXBUF_CXXFLAGS -DHAVE_SVG=1"
  CFLAGS="$CFLAGS -DHAVE_SVG=1"
  LIBS="$LIBS $LIBGDK_PIXBUF_LIBS"
else
  AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([whether to have Xft support])
AM_PATH_XFT(yes,
	XFT=true,
	XFT=false
)

if test "x$XFT" = "xtrue" ; then
	AC_TRY_LINK([
			#include <X11/Xft/Xft.h>
		], [ XftFontClose(0, 0); return 1; ], 
	[
		
		AC_DEFINE(USE_XFT, 1, "antialias support")
	    AC_MSG_CHECKING([Xft UTF-8 support])
   	     AC_TRY_LINK([
			#include <X11/Xft/Xft.h>
		], [ XftDrawStringUtf8(0, 0, 0, 0, 0, 0, 0); return 0; ],
		AC_DEFINE(HAVE_XFT_UTF8_STRING, 1, "Xft UTF8 support")
		AC_MSG_RESULT(yes),
		AC_MSG_RESULT(no)
		)
	],
	[	AC_MSG_RESULT([Could not link with Xft. Install Xft if you want support for it.])
		XFT=false
	])
fi

AM_CONDITIONAL(XFT, test x$XFT = xtrue)

dnl Determine if maintainer portions of the Makefiles should be included.
dnl AM_MAINTAINER_MODE

dnl default prefix
DEFAULT_PREFIX="$prefix"
AC_SUBST(DEFAULT_PREFIX)

dnl Print results
AC_MSG_RESULT([])
AC_MSG_RESULT([	$PACKAGE version $VERSION configured successfully.])
AC_MSG_RESULT([])
AC_MSG_RESULT([Using '$prefix' for installation.])
AC_MSG_RESULT([Using '$CXX' for C++ compiler.])
AC_MSG_RESULT([Building with '$CXXFLAGS' for C++ compiler flags.])
AC_MSG_RESULT([Building with '$LIBS' for linker flags.])
AC_MSG_RESULT([])


AC_OUTPUT(Makefile
src/Makefile
)
